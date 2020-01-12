#ifndef __SFALL_ROTATORS__
#define __SFALL_ROTATORS__

// Check if current ddraw.dll build is from Rotators
#define sfall_rotators  (read_byte(0x410003) == 0xF4 and sfall_func1("metarule_exist", "rotators"))

// Rotators sfall_funcX macros
#define r_tolower(val)                          sfall_func1("r_tolower", val)
#define r_toupper(val)                          sfall_func1("r_toupper", val)
#define r_set_hotspot_title(zoneX, zoneY, msg)  sfall_func3("r_set_hotspot_title", zoneX, zoneY, msg)

#endif // __SFALL_ROTATORS__ //
