
/*============================================================
 * Project: Ascencia [SHARED]
 * File: config.h
 * Author: Phraggers
 * Date: Mon Sep 09 2024
============================================================*/

#ifndef ASCENCIA_UTIL_CONFIG_H
#define ASCENCIA_UTIL_CONFIG_H

#include <util/types.h>

#define ASC_VERSION_RLS 0 // 0:dev 1:alpha 2:beta 3:release
#define ASC_VERSION_MAJ 0
#define ASC_VERSION_MIN 1
#define ASC_VERSION_REV 0

typedef struct sASC_Version
{
    u8 rls, maj, min, rev;
} ASC_Version;

#endif /* ASCENCIA_UTIL_CONFIG_H */