
/*============================================================
 * Project: Ascencia
 * File: maths.c
 * Author: Phraggers
 * Date: Wed Sep 11 2024
============================================================*/

#include <util/maths.h>

i32 sign(i32 v)
{
    return (v>=0)? 1 : -1;
}

r32 absolute(r32 v)
{
    return (r32)fabs(v);
}

i32 round_i32(r32 v)
{
    return (i32)floorf(v);
}

u32 round_u32(r32 v)
{
    return (u32)floorf(v);
}

i32 trunc_i32(r32 v)
{
    return (i32)v;
}

i32 floor_i32(r32 v)
{
    return (i32)floorf(v);
}

i32 ceil_i32(r32 v)
{
    return (i32)ceilf(v);
}

r32 divide(r32 n, r32 d)
{
    if(d != 0.0f)
    {
        return n/d;
    }
    
    return 0.0f;
}

r64 norm64(r64 v, r64 min, r64 max)
{
    return ((v-min) / (max-min));
}

r32 norm32(r32 v, r32 min, r32 max)
{
    return ((v-min) / (max-min));
}

r32 clamp(r32 v, r32 min, r32 max)
{
    return ((((v<min)?min:v)>max)?max:v);
}

r32 clamp_min(r32 v, r32 min)
{
    return ((v<min)?min:v);
}

r32 clamp_max(r32 v, r32 max)
{
    return ((v>max)?max:v);
}

r32 clamp_norm(r32 v)
{
    return ((((v<0.0f)?0.0f:v)>1.0f)?1.0f:v);
}

u32 rotl(u32 v, i32 s)
{
    s &= 31;
    return ((v<<s) | (v>>(32-s)));
}

u32 rotr(u32 v, i32 s)
{
    s &= 31;
    return ((v>>s) | (v<<(32-s)));
}

r32 lerp(r32 a, r32 b, r32 t)
{
    return ((1.0f-t) *a) + (t*b);
}

r32 qlerp(r32 a, r32 b, r32 t)
{
    return a + t * (b - a);
}

r32 square_root(r32 v)
{
    return sqrtf(v);
}

r32 square(r32 v)
{
    return v*v;
}

r32 pow32(r32 v, r32 s)
{
    return powf(v,s);
}

r64 pi64(void)
{
    return (r64)3.141592653589793238462643383279502884L;
}

r32 pi32(void)
{
    return (r32)3.141592653589793238463;
}

r64 tau64(void)
{
    return (r64)6.283185307179586476925286766559L;
}

r32 tau32(void)
{
    return (r32)6.2831853071795864769;
}

r32 sin32(r32 a)
{
    return sinf(a);
}

r32 cos32(r32 a)
{
    return cosf(a);
}

r32 atan32(r32 y, r32 x)
{
    return atan2f(y,x);
}