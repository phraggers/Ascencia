
/*============================================================
 * Project: Ascencia
 * File: string_helpers.h
 * Author: Phraggers
 * Date: Thu Sep 12 2024
============================================================*/

#ifndef ASCENCIA_STRING_HELPERS_H
#define ASCENCIA_STRING_HELPERS_H

#include <util/types.h>

// print string as smallest name (eg size = 1024 + 512 = "1.5 KB")
void PL_String_ShortFileSize(cstr string, u64 size);

/* print time now in strftime format
    %a      abbrev weekday name
    %A      full weekday name
    %b      abbrev month name
    %B      full month name
    %c      preset (ddd mmm dd hh:mm:ss yyyy)
    %d      day of month
    %H      24h hour
    %I      12h hour
    %j      day of year (001-366)
    %m      month
    %M      minute
    %P      AM/PM
    %S      sec
    %U      week of year starting first sunday (00-53)
    %W      week of year starting first monday (00-53)
    %w      weekday number starting sunday (0-6)
    %x      preset (mm/dd/yy)
    %X      preset (hh:mm:ss)
    %y      2 digit year
    %Y      4 digit year
    %Z      timezone name abbrev */
void PL_String_TimeStampNow(cstr buffer, cstr format);

#endif /* ASCENCIA_STRING_HELPERS_H */