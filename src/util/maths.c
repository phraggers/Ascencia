
/*============================================================
 * Project: Ascencia
 * File: maths.c
 * Author: Phraggers
 * Date: Wed Sep 11 2024
============================================================*/

#include <util/maths.h>
#include <math.h>

i32 sign(i32 v)
{
    i32 result = (v>=0)? 1 : -1;
    return result;
}

r32 absolute(r32 v)
{
    r32 result = (r32)fabs(v);
    return result;
}

i32 round_i32(r32 v)
{
    i32 result = 0;
    if((v - absolute(v)) > 0.5f)
    {
        result = (i32)ceilf(v);
    }
    else
    {
        result = (i32)floorf(v);
    }
    return result;
}

u32 round_u32(r32 v)
{
    if(v < 0.0f)
    {
        return 0;
    }

    u32 result = 0;
    if((v - absolute(v)) > 0.5f)
    {
        result = (u32)ceilf(v);
    }
    else
    {
        result = (u32)floorf(v);
    }
    return result;
}

i32 trunc_i32(r32 v)
{
    i32 result = (i32)v;
    return result;
}

i32 floor_i32(r32 v)
{
    i32 result = (i32)floorf(v);
    return result;
}

i32 ceil_i32(r32 v)
{
    i32 result = (i32)ceilf(v);
    return result;
}

r32 divide(r32 n, r32 d)
{
    r32 result = 0.0f;

    if(d != 0.0f)
    {
        result = (r32)(n/d);
    }

    return result;
}

r64 divide64(r64 n, r64 d)
{
    r64 result = 0.0;

    if(d != 0.0)
    {
        result = (r64)(n/d);
    }

    return result;
}

r64 norm64(r64 v, r64 min, r64 max)
{
    r64 result = divide64((v-min), (max-min));
    return result;
}

r32 norm32(r32 v, r32 min, r32 max)
{
    r32 result = divide((v-min), (max-min));
    return result;
}

r32 clamp(r32 v, r32 min, r32 max)
{
    r32 result = clamp_min(v, min);
    result = clamp_max(result, max);
    return result;
}

r32 clamp_min(r32 v, r32 min)
{
    r32 result = ((v<min)?min:v);
    return result;
}

r32 clamp_max(r32 v, r32 max)
{
    r32 result = ((v>max)?max:v);
    return result;
}

r32 clamp_norm(r32 v)
{
    r32 result = ((((v<0.0f)?0.0f:v)>1.0f)?1.0f:v);
    return result;
}

i32 clamp_i32(i32 v, i32 min, i32 max)
{
    i32 result = clamp_min_i32(v, min);
    result = clamp_max_i32(result, max);
    return result;
}

i32 clamp_min_i32(i32 v, i32 min)
{
    i32 result = ((v<min)?min:v);
    return result;
}

i32 clamp_max_i32(i32 v, i32 max)
{
    i32 result = ((v>max)?max:v);
    return result;
}

r32 lerp(r32 a, r32 b, r32 t)
{
    r32 result = ((1.0f-t) *a) + (t*b);
    return result;
}

r32 qlerp(r32 a, r32 b, r32 t)
{
    r32 result = (a + t * (b - a));
    return result;
}

r32 square_root(r32 v)
{
    r32 result = (r32)sqrtf(v);
    return result;
}

r32 square(r32 v)
{
    r32 result = (v*v);
    return result;
}

r32 pow32(r32 v, r32 s)
{
    r32 result = (r32)powf(v,s);
    return result;
}

r64 pow64(r64 v, r64 s)
{
    r64 result = (r64)pow(v,s);
    return result;
}

r64 pi64(void)
{
    r64 result = (r64)3.141592653589793238462643383279502884L;
    return result;
}

r32 pi32(void)
{
    r32 result = (r32)3.141592653589793238463f;
    return result;
}

r64 tau64(void)
{
    r64 result = (r64)6.283185307179586476925286766559L;
    return result;
}

r32 tau32(void)
{
    r32 result = (r32)6.2831853071795864769f;
    return result;
}

r32 sin32(r32 a)
{
    r32 result = (r32)sinf(a);
    return result;
}

r32 cos32(r32 a)
{
    r32 result = (r32)cosf(a);
    return result;
}

r32 atan32(r32 y, r32 x)
{
    r32 result = (r32)atan2f(y,x);
    return result;
}

r32 rad32(r32 deg)
{
    r32 result = (deg * pi32()) / 180.0f;
    return result;
}

r64 rad64(r64 deg)
{
    r64 result = (deg * pi64()) / 180.0;
    return result;
}

r32 deg32(r32 rad)
{
    r32 result = (rad * 180.0f) / pi32();
    return result;
}

r64 deg64(r64 rad)
{
    r64 result = (rad * 180.0) / pi64();
    return result;
}

bool equaleps(r32 a, r32 b, r32 e)
{
	if (a == b)
    {
		return 1;
	}

    if (absolute(a-b) <= e)
    {
		return 1;
	}

	return 0;
}