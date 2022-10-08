tested only on server win32 gmod version

Download dll from releases (https://github.com/TheFUlDeep/gmod_cpu_info/releases)

Move dll file into GarrysMod/garrysmod/lua/bin

require("cpu_info")

GetProcessorsCount() - return count of processors

GetProcessorLoad(num) - return utilizations of core in percentage (min num is 1)

example in lua https://github.com/TheFUlDeep/gmod_addons/blob/master/custom_commands/lua/thefuldeep_autorun/sh_cpu_load.lua

how to compile .dll https://wiki.facepunch.com/gmod/Creating_Binary_Modules
