#ifndef DIR_H
/* ==============================================================
   File:    dir.h
   Author:  Phraggers
   Date:    22-04-2025
   ============================================================== */

/* returns path to dir where this exe is.
   path always ends with '/' (or '\\' on win32). */
char* PL_GetBasePath(void);

#define DIR_H
#endif
