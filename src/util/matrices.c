/*============================================================
 * Project: Ascencia
 * File: matrices.c
 * Author: Phraggers
 * Date: Wed Sep 18 2024
============================================================*/

#include <util/matrices.h>

v2 v2mulmat2(v2 v, mat2 m)
{
    v2 result =
    {
        m.i[0][0] * v.i[0] + m.i[0][1] * v.i[1],
        m.i[1][0] * v.i[0] + m.i[1][1] * v.i[1]
    };

	return result;
}

v3 v3mulmat3(v3 v, mat3 m)
{
    v3 result =
    {
        m.i[0][0] * v.i[0] + m.i[0][1] * v.i[1] + m.i[0][2] * v.i[2],
        m.i[1][0] * v.i[0] + m.i[1][1] * v.i[1] + m.i[1][2] * v.i[2],
        m.i[2][0] * v.i[0] + m.i[2][1] * v.i[1] + m.i[2][2] * v.i[2]
    };
	
	return result;
}

mat2 v2orthonormalize(mat2 basis)
{
    v2 v0 =
    {
        basis.i[0][0],
        basis.i[0][1]
    };

    v2 v1 =
    {
        basis.i[1][0],
        basis.i[1][1]
    };

    if(!v2linear_independent(v0,v1))
    {
        return (mat2){0.0f,0.0f,0.0f,0.0f};
    }

    v2 u0 = {v0.i[0], v0.i[1]};
    v2 u1proj = v2project(v1,v0);
    v2 u1 = v2subv(v1, u1proj);

    mat2 result;
    result.v0 = v2norm(u0);
    result.v1 = v2norm(u1);

    return result;
}