//спасибо https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
#include "pch.h"
#include "TCHAR.h"
#include "pdh.h"
#include <string>
#include "GarrysMod\Lua\Interface.h"

#pragma comment(lib,"pdh.lib")

using namespace GarrysMod::Lua;

PDH_HQUERY* cpuQuery;
PDH_HCOUNTER* cpuTotal;
unsigned short numProcessors;


int getCurrentValue(unsigned short core) {
    PDH_FMT_COUNTERVALUE counterVal;

    PdhCollectQueryData(cpuQuery[core]);
    PdhGetFormattedCounterValue(cpuTotal[core], PDH_FMT_DOUBLE, NULL, &counterVal);
    return counterVal.doubleValue;
}


LUA_FUNCTION(GetProcessorLoad)
{
    double first_number = LUA->CheckNumber(1);
    LUA->PushNumber(getCurrentValue(first_number));
    return 1;
}

LUA_FUNCTION(GetProcessorsCount)
{
    LUA->PushNumber(numProcessors);
    return 1;
}

GMOD_MODULE_OPEN()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;

    //обращение к первому индекусу - нулевой процессор, ко второму - первый и тд. Сделано для луа стайла
    std::wstring* strings = new std::wstring[numProcessors];
    for (size_t core = 0; core < numProcessors; core++)
    {
        std::string str = "\\Processor(" + std::to_string(core) + ")\\% Processor Time";
        strings[core] = std::wstring(str.begin(), str.end());
    }
    cpuQuery = new PDH_HQUERY[numProcessors + 1];
    cpuTotal = new PDH_HCOUNTER[numProcessors + 1];

    for (unsigned short core = 1; core <= numProcessors; core++)
    {
        PdhOpenQuery(NULL, NULL, &cpuQuery[core]);
        PdhAddEnglishCounter(cpuQuery[core], strings[core - 1].c_str(), NULL, &cpuTotal[core]);
        PdhCollectQueryData(cpuQuery[core]);
    }

    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->PushCFunction(GetProcessorLoad);
    LUA->SetField(-2, "GetProcessorLoad"); // Set MyFirstFunction in lua to our C++ function
    LUA->Pop();

    LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
    LUA->PushCFunction(GetProcessorsCount);
    LUA->SetField(-2, "GetProcessorsCount"); // Set MyFirstFunction in lua to our C++ function
    LUA->Pop();

    return 0;
}

GMOD_MODULE_CLOSE()
{
    delete[] cpuQuery;
    delete[] cpuTotal;
    delete[] strings;
    return 0;
}
