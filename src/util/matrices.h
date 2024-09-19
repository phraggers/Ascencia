/*============================================================
 * Project: Ascencia
 * File: matrices.h
 * Author: Phraggers
 * Date: Wed Sep 18 2024
============================================================*/

#ifndef ASCENCIA_MATRICES_H
#define ASCENCIA_MATRICES_H

#include <util/types.h>
#include <util/maths.h>
#include <util/vectors.h>

typedef union
{
    r32 i[2][2];
    struct {v2 v0,v1;};
    struct {r32 
            m00,m10,
            m01,m11;};
} mat2;

typedef union
{
    r32 i[3][3];
    struct {v3 v0,v1;};
    struct {r32 
            m00,m10,m20,
            m01,m11,m21,
            m02,m12,m22;};
} mat3;

typedef union
{
    r32 i[4][4];
    struct {v4 v0,v1;};
    struct {r32 
            m00,m10,m20,m30,
            m01,m11,m21,m31,
            m02,m12,m22,m32,
            m03,m13,m23,m33;};
} mat4;

v2 v2mulmat2(v2 v, mat2 m);
v3 v3mulmat3(v3 v, mat3 m);
mat2 v2orthonormalize(mat2 basis);

#endif /* ASCENCIA_MATRICES_H */