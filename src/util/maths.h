
/*============================================================
 * Project: Ascencia
 * File: maths.h
 * Author: Phraggers
 * Date: Wed Sep 11 2024
============================================================*/

#ifndef ASCENCIA_UTIL_MATHS_H
#define ASCENCIA_UTIL_MATHS_H

#include <util/types.h>

i32 sign(i32 v); // sign of (+1 or -1)
r32 absolute(r32 v); // absolute value (val without sign)
i32 round_i32(r32 v); // round r32 to nearest i32
u32 round_u32(r32 v); // round r32 to nearest u32
i32 trunc_i32(r32 v); // truncate r32 to i32
i32 floor_i32(r32 v); // floor r32 to i32
i32 ceil_i32(r32 v); // ceil r32 to i32
r32 divide(r32 n, r32 d); // n/d, if d==0, returns 0.0f
r64 divide64(r64 n, r64 d);

r64 norm64(r64 v, r64 min, r64 max); // normalize 0.0 - 1.0 (64bit)
r32 norm32(r32 v, r32 min, r32 max); // normalize 0.0 - 1.0 (32bit)

r32 clamp(r32 v, r32 min, r32 max); // clamp between min and max
r32 clamp_min(r32 v, r32 min); // clamp to >= min
r32 clamp_max(r32 v, r32 max); // clamp to <= max
r32 clamp_norm(r32 v); // clamp to normalized value >= 0.0, <= 1.0

i32 clamp_i32(i32 v, i32 min, i32 max);
i32 clamp_min_i32(i32 v, i32 min);
i32 clamp_max_i32(i32 v, i32 max);

r32 lerp(r32 a, r32 b, r32 t); // linear interpolation
r32 qlerp(r32 a, r32 b, r32 t); // quick lerp (fewer CPU cycles but less precise)

r64 pi64(void); // returns 64bit pi
r32 pi32(void); // returns 32bit pi
r64 tau64(void); // returns 64bit 2*pi
r32 tau32(void); // returns 32bit 2*pi

r32 square_root(r32 v);
r32 square(r32 v); // v*v
r32 pow32(r32 v, r32 s); // v^s
r64 pow64(r64 v, r64 s);
r32 sin32(r32 a);
r32 cos32(r32 a);
r32 atan32(r32 y, r32 x);

r32 rad32(r32 deg); // degrees to radians
r64 rad64(r64 deg);
r32 deg32(r32 rad); // radians to degrees
r64 deg64(r64 rad);

bool equaleps(r32 a, r32 b, r32 e);

#endif /* ASCENCIA_UTIL_MATHS_H */