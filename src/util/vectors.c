
/*============================================================
 * Project: Ascencia
 * File: vectors.c
 * Author: Phraggers
 * Date: Wed Sep 11 2024
============================================================*/

#include <util/vectors.h>
#include <util/maths.h>

// ------ v2 ------ //

/* === v2 constructors === */

v2 v2i(i32 a, i32 b)
{
    v2 result = {(r32)a, (r32)b};
    return result;
}

v2 v2u(u32 a, u32 b)
{
    v2 result = {(r32)a, (r32)b};
    return result;    
}

v2 v2r(r32 a, r32 b)
{
    v2 result = {a, b};
    return result;    
}

/* === v2 operators === */

v2 v2addf(v2 v, r32 f)
{
    v2 result = 
    {
        v.i[0]+f, 
        v.i[1]+f
    };
    return result;
}

v2 v2subf(v2 v, r32 f)
{
    v2 result = 
    {
        v.i[0]-f, 
        v.i[1]-f
    };
    return result;
}

v2 v2mulf(v2 v, r32 f)
{
    v2 result = 
    {
        v.i[0]*f, 
        v.i[1]*f
    };
    return result;
}

v2 v2divf(v2 v, r32 f)
{
    v2 result = 
    {
        divide(v.i[0],f), 
        divide(v.i[1],f)
    };
    return result; 
}

/* === v2/v2 operators === */

v2 v2addv(v2 a, v2 b)
{
    v2 result = 
    {
        a.i[0]+b.i[0], 
        a.i[1]+b.i[1]
    };
    return result;
}

v2 v2subv(v2 a, v2 b)
{
    v2 result = 
    {
        a.i[0]-b.i[0], 
        a.i[1]-b.i[1]
    };
    return result;
}

v2 v2mulv(v2 a, v2 b)
{
    v2 result = 
    {
        a.i[0]*b.i[0], 
        a.i[1]*b.i[1]
    };
    return result;
}

v2 v2divv(v2 a, v2 b)
{
    v2 result = 
    {
        divide(a.i[0], b.i[0]),
        divide(a.i[1], b.i[1])
    };
    return result;
}

/* === v2 functions === */

bool v2zero(v2 v)
{
    bool result = 
    (
        ((!v.i[0]) && (!v.i[1])) ? 1:0
    );
    return result;
}

bool v2one(v2 v)
{
    bool result = 
    (
        ((v.i[0]==1.0f) && (v.i[1]==1.0f)) ? 1:0
    );
    return result;
}

v2 v2sign(v2 v)
{
    v2 result = 
    {
        (v.i[0]>=0.0f) ? 1.0f:-1.0f,
        (v.i[1]>=0.0f) ? 1.0f:-1.0f
    };
    return result;
}

v2 v2invert(v2 v)
{
    v2 result = 
    {
        v.i[0] = -v.i[0], 
        v.i[1] = -v.i[1]
    };
    return result;
}

v2 v2perp(v2 v)
{
    v2 result = 
    {
        -v.y, 
         v.x
    };
    return result;
}

v2 v2tangent(v2 v)
{
    v2 result = 
    {
         v.y, 
        -v.x
    };
    return result;
}

v2 v2rotate(v2 v, r32 r)
{
    r32 cs = cos32(r);
    r32 sn = sin32(r);

    v2 result =
    {
        v.i[0] * cs - v.i[1] * sn,
        v.i[0] * sn + v.i[1] * cs
    };

	return result;
}

r32 v2lengthsq(v2 v)
{
    r32 result = v2dot(v,v);
    return result;
}

r32 v2length(v2 v)
{
    r32 result = square_root(v2lengthsq(v));
    return result;
}

v2 v2abs(v2 v)
{
    v2 result = 
    {
        absolute(v.i[0]),
        absolute(v.i[1])
    };
    return result;
}

v2 v2clampf(v2 v, r32 min, r32 max)
{
    v2 result = 
    {
        clamp(v.i[0], min, max),
        clamp(v.i[1], min, max)
    };
    return result;
}

v2 v2clampfmin(v2 v, r32 min)
{
    v2 result = 
    {
        clamp_min(v.i[0], min),
        clamp_min(v.i[1], min)
    };
    return result;   
}

v2 v2clampfmax(v2 v, r32 max)
{
    v2 result = 
    {
        clamp_max(v.i[0], max),
        clamp_max(v.i[1], max)
    };
    return result;  
}

v2 v2floor(v2 v)
{
    v2 result =
    {
        (r32)floor_i32(v.i[0]),
        (r32)floor_i32(v.i[1])
    };
    return result;
}

v2 v2ceil(v2 v)
{
    v2 result =
    {
        (r32)ceil_i32(v.i[0]),
        (r32)ceil_i32(v.i[1])
    };
    return result;
}

v2 v2round(v2 v)
{
    v2 result =
    {
        (r32)round_i32(v.i[0]),
        (r32)round_i32(v.i[1])
    };
    return result;
}

v2 v2norm(v2 v)
{
	r32 len = square_root(v2dot(v,v));
    v2 result =
    {
        divide(v.i[0], len),
        divide(v.i[1], len)
    };
	return result;
}

r32 v2angle(v2 v)
{
    r32 result = atan32(v.i[1], v.i[0]);
	return result;
}

v2 v2snapf(v2 v, r32 f)
{
    v2 result =
    {
        (r32)floor_i32(divide(v.i[0], f) * f),
        (r32)floor_i32(divide(v.i[1], f) * f)
    };
	return result;
}

/* === v2/v2 functions === */

v2 v2clampv(v2 v, v2 minv, v2 maxv)
{
    v2 result =
    {
        clamp(v.i[0], minv.i[0], maxv.i[0]),
        clamp(v.i[1], minv.i[1], maxv.i[1])
    };
    return result;
}

v2 v2clampvmin(v2 v, v2 minv)
{
    v2 result =
    {
        clamp_min(v.i[0], minv.i[0]),
        clamp_min(v.i[1], minv.i[1])
    };
    return result;
}

v2 v2clampvmax(v2 v, v2 maxv)
{
    v2 result =
    {
        clamp_max(v.i[0], maxv.i[0]),
        clamp_max(v.i[1], maxv.i[1])
    };
    return result;
}

bool v2less(v2 a, v2 b)
{
    bool result = 
    (
        ((a.i[0]+a.i[1]) < (b.i[0]+b.i[1])) ? 1:0
    );
    return result;
}

bool v2greater(v2 a, v2 b)
{
    bool result = 
    (
        ((a.i[0]+a.i[1]) > (b.i[0]+b.i[1])) ? 1:0
    );
    return result;
}

bool v2equal(v2 a, v2 b)
{
    bool result = 
    (
        ((a.i[0]==b.i[0]) && (a.i[1]==b.i[1])) ? 1:0
    );
    return result;
}

r32 v2dot(v2 a, v2 b)
{
    r32 result = 
    (
        (a.i[0]*b.i[0]) + (a.i[1]*b.i[1])
    );
    return result;
}

v2 v2snapv(v2 a, v2 b)
{
    v2 result = 
    {
        (divide(a.i[0],b.i[0]) * a.i[0]), 
        (divide(a.i[1],b.i[1]) * a.i[1])
    };
    return result;
}

v2 v2max(v2 a, v2 b)
{
    v2 result = 
    {
        ((a.i[0]>b.i[0]) ? a.i[0] : b.i[0]), 
        ((a.i[1]>b.i[1]) ? a.i[1] : b.i[1])
    };
    return result;
}

v2 v2min(v2 a, v2 b)
{
    v2 result = 
    {
        ((a.i[0]<b.i[0])?a.i[0]:b.i[0]), 
        ((a.i[1]<b.i[1])?a.i[1]:b.i[1])
    };
    return result;
}

v2 v2project(v2 a, v2 b)
{
    r32 d = v2dot(b, b);
    r32 s = divide(v2dot(a, b), d);

    v2 result =
    {
        b.i[0] * s,
        b.i[1] * s
    };

	return result;
}

v2 v2slide(v2 v, v2 vn)
{
    r32 d = v2dot(v, vn);
    
    v2 result =
    {
        v.i[0] - vn.i[0] * d,
        v.i[1] - vn.i[1] * d
    };

	return result;
}

v2 v2reflect(v2 v, v2 vn)
{
    r32 d = 2.0f * v2dot(v, vn);

    v2 result =
    {
        vn.i[0] * d - v.i[0],
        vn.i[1] * d - v.i[1]
    };

	return result;
}

v2 v2lerpf(v2 a, v2 b, r32 f)
{
    v2 result =
    {
        a.i[0] + (b.i[0] - a.i[0]) * f,
        a.i[1] + (b.i[1] - a.i[1]) * f
    };

	return result;
}

v2 v2bez3(v2 a, v2 b, v2 c, r32 f)
{
    v2 result = v2lerpf(v2lerpf(a, b, f), v2lerpf(b, c, f), f);
	return result;
}

v2 v2bez4(v2 a, v2 b, v2 c, v2 d, r32 f)
{
    v2 t1 = v2lerpf(v2lerpf(a, b, f), v2lerpf(b, c, f), f);
    v2 t2 = v2lerpf(v2lerpf(b, c, f), v2lerpf(c, d, f), f);
    v2 result = v2lerpf(t1, t2, f);
	return result;
}

r32 v2distance(v2 a, v2 b)
{
    r32 result = square_root((a.i[0]-b.i[0]) * (a.i[0]-b.i[0]) + (a.i[1]-b.i[1]) * (a.i[1]-b.i[1]));
    return result;
}

r32 v2distancesq(v2 a, v2 b)
{
    r32 result = (a.i[0]-b.i[0]) * (a.i[0]-b.i[0]) + (a.i[1]-b.i[1]) * (a.i[1]-b.i[1]);
    return result;
}

bool v2linear_independent(v2 a, v2 b)
{
    bool result = (a.i[0] * b.i[1] - b.i[0] * a.i[1]) != 0;
    return result;
}

// ------ v3 ------ //

/* === v2 constructors === */

v3 v3i(i32 a, i32 b, i32 c)
{
    v3 result = {(r32)a, (r32)b, (r32)c};
    return result;
}
v3 v3u(u32 a, u32 b, u32 c)
{
    v3 result = {(r32)a, (r32)b, (r32)c};
    return result;    
}
v3 v3r(r32 a, r32 b, r32 c)
{
    v3 result = {a, b, c};
    return result;    
}

/* === v3 operators === */

v3 v3addf(v3 v, r32 f)
{
    v3 result = 
    {
        v.i[0]+f, 
        v.i[1]+f,
        v.i[2]+f
    };
    return result;
}

v3 v3subf(v3 v, r32 f)
{
    v3 result = 
    {
        v.i[0]-f, 
        v.i[1]-f,
        v.i[2]-f
    };
    return result;
}

v3 v3mulf(v3 v, r32 f)
{
    v3 result = 
    {
        v.i[0]*f, 
        v.i[1]*f,
        v.i[2]*f
    };
    return result;
}

v3 v3divf(v3 v, r32 f)
{
    v3 result = 
    {
        divide(v.i[0],f), 
        divide(v.i[1],f),
        divide(v.i[2],f)
    };
    return result; 
}

/* === v3/v3 operators === */

v3 v3addv(v3 a, v3 b)
{
    v3 result = 
    {
        a.i[0]+b.i[0], 
        a.i[1]+b.i[1],
        a.i[2]+b.i[2]
    };
    return result;
}

v3 v3subv(v3 a, v3 b)
{
    v3 result = 
    {
        a.i[0]-b.i[0], 
        a.i[1]-b.i[1],
        a.i[2]-b.i[2]
    };
    return result;
}

v3 v3mulv(v3 a, v3 b)
{
    v3 result = 
    {
        a.i[0]*b.i[0], 
        a.i[1]*b.i[1],
        a.i[2]*b.i[2]
    };
    return result;
}

v3 v3divv(v3 a, v3 b)
{
    v3 result = 
    {
        divide(a.i[0], b.i[0]), 
        divide(a.i[1], b.i[1]),
        divide(a.i[2], b.i[2])
    };
    return result;
}

/* === v3 functions === */

bool v3zero(v3 v)
{
    bool result = 
    (
        ((!v.i[0]) && (!v.i[1]) && (!v.i[2])) ? 1:0
    );
    return result;
}

bool v3one(v3 v)
{
    bool result = 
    (
        ((v.i[0]==1.0f) && (v.i[1]==1.0f) && (v.i[2]==1.0f)) ? 1:0
    );
    return result;
}

v3 v3sign(v3 v)
{
    v3 result = 
    {
        (v.i[0]>=0.0f) ? 1.0f:-1.0f, 
        (v.i[1]>=0.0f) ? 1.0f:-1.0f,
        (v.i[2]>=0.0f) ? 1.0f:-1.0f
    };
    return result;
}

v3 v3invert(v3 v)
{
    v3 result = 
    {
        v.i[0] = -v.i[0],
        v.i[1] = -v.i[1],
        v.i[2] = -v.i[2]
    };
    return result;
}

r32 v3lengthsq(v3 v)
{
    r32 result = v3dot(v,v);
    return result;
}

r32 v3length(v3 v)
{
    r32 result = square_root(v3lengthsq(v));
    return result;
}

v3 v3abs(v3 v)
{
    v3 result = 
    {
        absolute(v.i[0]),
        absolute(v.i[1]),
        absolute(v.i[2])
    };
    return result;
}

v3 v3clampf(v3 v, r32 min, r32 max)
{
    v3 result = 
    {
        clamp(v.i[0], min, max),
        clamp(v.i[1], min, max),
        clamp(v.i[2], min, max)
    };
    return result;
}

v3 v3clampfmin(v3 v, r32 min)
{
    v3 result = 
    {
        clamp_min(v.i[0], min),
        clamp_min(v.i[1], min),
        clamp_min(v.i[2], min)
    };
    return result;   
}

v3 v3clampfmax(v3 v, r32 max)
{
    v3 result = 
    {
        clamp_max(v.i[0], max),
        clamp_max(v.i[1], max),
        clamp_max(v.i[2], max)
    };
    return result;  
}

v3 v3floor(v3 v)
{
    v3 result =
    {
        (r32)floor_i32(v.i[0]),
        (r32)floor_i32(v.i[1]),
        (r32)floor_i32(v.i[2])
    };
    return result;
}

v3 v3ceil(v3 v)
{
    v3 result =
    {
        (r32)ceil_i32(v.i[0]),
        (r32)ceil_i32(v.i[1]),
        (r32)ceil_i32(v.i[2])
    };
    return result;
}

v3 v3round(v3 v)
{
    v3 result =
    {
        (r32)round_i32(v.i[0]),
        (r32)round_i32(v.i[1]),
        (r32)round_i32(v.i[2])
    };
    return result;
}

/* === v3/v3 functions === */

v3 v3clampv(v3 a, v3 min, v3 max)
{
    v3 result =
    {
        clamp(a.i[0], min.i[0], max.i[0]),
        clamp(a.i[1], min.i[1], max.i[1]),
        clamp(a.i[2], min.i[2], max.i[2])
    };
    return result;
}

v3 v3clampvmin(v3 a, v3 min)
{
    v3 result =
    {
        clamp_min(a.i[0], min.i[0]),
        clamp_min(a.i[1], min.i[1]),
        clamp_min(a.i[2], min.i[2])
    };
    return result;
}

v3 v3clampvmax(v3 a, v3 max)
{
    v3 result =
    {
        clamp_max(a.i[0], max.i[0]),
        clamp_max(a.i[1], max.i[1]),
        clamp_max(a.i[2], max.i[2])
    };
    return result;
}

bool v3less(v3 a, v3 b)
{
    bool result = 
    (
        ((a.i[0]+a.i[1]+a.i[2]) < (b.i[0]+b.i[1]+b.i[2])) ? 1:0
    );
    return result;
}

bool v3greater(v3 a, v3 b)
{
    bool result = 
    (
        ((a.i[0]+a.i[1]+a.i[2]) > (b.i[0]+b.i[1]+b.i[2])) ? 1:0
    );
    return result;
}

bool v3equal(v3 a, v3 b)
{
    bool result = 
    (
        ((a.i[0]==b.i[0]) && (a.i[1]==b.i[1]) && (a.i[2]==b.i[2])) ? 1:0
    );
    return result;
}

r32 v3dot(v3 a, v3 b)
{
    r32 result = 
    (
        (a.i[0]*b.i[0]) + (a.i[1]*b.i[1]) + (a.i[2]*b.i[2])
    );
    return result;
}

v3 v3snapv(v3 a, v3 b)
{
    v3 result = 
    {
        (divide(a.i[0],b.i[0]) * a.i[0]), 
        (divide(a.i[1],b.i[1]) * a.i[1]),
        (divide(a.i[2],b.i[2]) * a.i[2])
    };
    return result;
}

v3 v3max(v3 a, v3 b)
{
    v3 result = 
    {
        ((a.i[0]>b.i[0]) ? a.i[0] : b.i[0]), 
        ((a.i[1]>b.i[1]) ? a.i[1] : b.i[1]),
        ((a.i[2]>b.i[2]) ? a.i[2] : b.i[2])
    };
    return result;
}

v3 v3min(v3 a, v3 b)
{
    v3 result = 
    {
        ((a.i[0]<b.i[0])?a.i[0]:b.i[0]), 
        ((a.i[1]<b.i[1])?a.i[1]:b.i[1]),
        ((a.i[2]<b.i[2])?a.i[2]:b.i[2])
    };
    return result;
}

v3 v3cross(v3 a, v3 b)
{
	v3 result =
    {
        a.i[1] * b.i[2] - a.i[2] * b.i[1],
        a.i[2] * b.i[0] - a.i[0] * b.i[2],
        a.i[0] * b.i[1] - a.i[1] * b.i[0]
    };
	return result;
}

// ------ v4 ------ //

/* === v4 constructors === */

v4 v4i(i32 a, i32 b, i32 c, i32 d)
{
    v4 result = {(r32)a, (r32)b, (r32)c, (r32)d};
    return result;
}
v4 v4u(u32 a, u32 b, u32 c, u32 d)
{
    v4 result = {(r32)a, (r32)b, (r32)c, (r32)d};
    return result;    
}
v4 v4r(r32 a, r32 b, r32 c, r32 d)
{
    v4 result = {a, b, c, d};
    return result;    
}

/* === v4 operators === */

v4 v4addf(v4 v, r32 f)
{
    v4 result = 
    {
        v.i[0]+f, 
        v.i[1]+f,
        v.i[2]+f,
        v.i[3]+f
    };
    return result;
}

v4 v4subf(v4 v, r32 f)
{
    v4 result = 
    {
        v.i[0]-f, 
        v.i[1]-f,
        v.i[2]-f,
        v.i[3]-f
    };
    return result;
}

v4 v4mulf(v4 v, r32 f)
{
    v4 result = 
    {
        v.i[0]*f, 
        v.i[1]*f,
        v.i[2]*f,
        v.i[3]*f
    };
    return result;
}

v4 v4divf(v4 v, r32 f)
{
    v4 result = 
    {
        divide(v.i[0],f), 
        divide(v.i[1],f),
        divide(v.i[2],f),
        divide(v.i[3],f)
    };
    return result; 
}

/* === v4/v4 operators === */

v4 v4addv(v4 a, v4 b)
{
    v4 result = 
    {
        a.i[0]+b.i[0], 
        a.i[1]+b.i[1],
        a.i[2]+b.i[2],
        a.i[3]+b.i[3]
    };
    return result;
}

v4 v4subv(v4 a, v4 b)
{
    v4 result = 
    {
        a.i[0]-b.i[0], 
        a.i[1]-b.i[1],
        a.i[2]-b.i[2],
        a.i[3]-b.i[3]
    };
    return result;
}

v4 v4mulv(v4 a, v4 b)
{
    v4 result = 
    {
        a.i[0]*b.i[0], 
        a.i[1]*b.i[1],
        a.i[2]*b.i[2],
        a.i[3]*b.i[3]
    };
    return result;
}

v4 v4divv(v4 a, v4 b)
{
    v4 result = 
    {
        divide(a.i[0], b.i[0]), 
        divide(a.i[1], b.i[1]),
        divide(a.i[2], b.i[2]),
        divide(a.i[3], b.i[3])
    };
    return result;
}

/* === v4 functions === */

bool v4zero(v4 v)
{
    bool result = 
    (
        ((!v.i[0]) && (!v.i[1]) && (!v.i[2]) && (!v.i[3])) ? 1:0
    );
    return result;
}

bool v4one(v4 v)
{
    bool result = 
    (
        ((v.i[0]==1.0f) && (v.i[1]==1.0f) && (v.i[2]==1.0f) && (v.i[3]==1.0f)) ? 1:0
    );
    return result;
}

v4 v4sign(v4 v)
{
    v4 result = 
    {
        (v.i[0]>=0.0f) ? 1.0f:-1.0f, 
        (v.i[1]>=0.0f) ? 1.0f:-1.0f,
        (v.i[2]>=0.0f) ? 1.0f:-1.0f,
        (v.i[3]>=0.0f) ? 1.0f:-1.0f
    };
    return result;
}

v4 v4invert(v4 v)
{
    v4 result = 
    {
        v.i[0]=-v.i[0], 
        v.i[1]=-v.i[1],
        v.i[2]=-v.i[2],
        v.i[3]=-v.i[3]
    };
    return result;
}

r32 v4lengthsq(v4 v)
{
    r32 result = v4dot(v,v);
    return result;
}

r32 v4length(v4 v)
{
    r32 result = square_root(v4lengthsq(v));
    return result;
}

v4 v4abs(v4 v)
{
    v4 result = 
    {
        absolute(v.i[0]), 
        absolute(v.i[1]),
        absolute(v.i[2]),
        absolute(v.i[3])
    };
    return result;
}

v4 v4clampf(v4 v, r32 min, r32 max)
{
    v4 result = 
    {
        clamp(v.i[0], min, max),
        clamp(v.i[1], min, max),
        clamp(v.i[2], min, max),
        clamp(v.i[3], min, max)
    };
    return result;
}

v4 v4clampfmin(v4 v, r32 min)
{
    v4 result = 
    {
        clamp_min(v.i[0], min),
        clamp_min(v.i[1], min),
        clamp_min(v.i[2], min),
        clamp_min(v.i[3], min)
    };
    return result;   
}

v4 v4clampfmax(v4 v, r32 max)
{
    v4 result = 
    {
        clamp_max(v.i[0], max),
        clamp_max(v.i[1], max),
        clamp_max(v.i[2], max),
        clamp_max(v.i[3], max)
    };
    return result;  
}

v4 v4floor(v4 v)
{
    v4 result =
    {
        (r32)floor_i32(v.i[0]),
        (r32)floor_i32(v.i[1]),
        (r32)floor_i32(v.i[2]),
        (r32)floor_i32(v.i[3])
    };
    return result;
}

v4 v4ceil(v4 v)
{
    v4 result =
    {
        (r32)ceil_i32(v.i[0]),
        (r32)ceil_i32(v.i[1]),
        (r32)ceil_i32(v.i[2]),
        (r32)ceil_i32(v.i[3])
    };
    return result;
}

v4 v4round(v4 v)
{
    v4 result =
    {
        (r32)round_i32(v.i[0]),
        (r32)round_i32(v.i[1]),
        (r32)round_i32(v.i[2]),
        (r32)round_i32(v.i[3])
    };
    return result;
}

/* === v4/v4 functions === */

v4 v4clampv(v4 v, v4 minv, v4 maxv)
{
    v4 result =
    {
        clamp(v.i[0], minv.i[0], maxv.i[0]),
        clamp(v.i[1], minv.i[1], maxv.i[1]),
        clamp(v.i[2], minv.i[2], maxv.i[2]),
        clamp(v.i[3], minv.i[3], maxv.i[3])
    };
    return result;
}

v4 v4clampvmin(v4 v, v4 minv)
{
    v4 result =
    {
        clamp_min(v.i[0], minv.i[0]),
        clamp_min(v.i[1], minv.i[1]),
        clamp_min(v.i[2], minv.i[2]),
        clamp_min(v.i[3], minv.i[3])
    };
    return result;
}

v4 v4clampvmax(v4 v, v4 maxv)
{
    v4 result =
    {
        clamp_max(v.i[0], maxv.i[0]),
        clamp_max(v.i[1], maxv.i[1]),
        clamp_max(v.i[2], maxv.i[2]),
        clamp_max(v.i[3], maxv.i[3])
    };
    return result;
}

bool v4less(v4 a, v4 b)
{
    bool result = 
    (
        ((a.i[0]+a.i[1]+a.i[2]+a.i[3]) < (b.i[0]+b.i[1]+b.i[2]+b.i[3])) ? 1:0
    );
    return result;
}

bool v4greater(v4 a, v4 b)
{
    bool result = 
    (
        ((a.i[0]+a.i[1]+a.i[2]+a.i[3]) > (b.i[0]+b.i[1]+b.i[2]+b.i[3])) ? 1:0
    );
    return result;
}

bool v4equal(v4 a, v4 b)
{
    bool result = 
    (
        ((a.i[0]==b.i[0]) && (a.i[1]==b.i[1]) && (a.i[2]==b.i[2]) && (a.i[3]==b.i[3])) ? 1:0
    );
    return result;
}

r32 v4dot(v4 a, v4 b)
{
    r32 result = 
    (
        (a.i[0]*b.i[0]) + (a.i[1]*b.i[1]) + (a.i[2]*b.i[2]) + (a.i[3]*b.i[3])
    );
    return result;
}

v4 v4snapv(v4 a, v4 b)
{
    v4 result = 
    {
        (divide(a.i[0],b.i[0]) * a.i[0]), 
        (divide(a.i[1],b.i[1]) * a.i[1]),
        (divide(a.i[2],b.i[2]) * a.i[2]),
        (divide(a.i[3],b.i[3]) * a.i[3])
    };
    return result;
}

v4 v4max(v4 a, v4 b)
{
    v4 result = 
    {
        ((a.i[0]>b.i[0]) ? a.i[0] : b.i[0]), 
        ((a.i[1]>b.i[1]) ? a.i[1] : b.i[1]),
        ((a.i[2]>b.i[2]) ? a.i[2] : b.i[2]),
        ((a.i[3]>b.i[3]) ? a.i[3] : b.i[3])
    };
    return result;
}

v4 v4min(v4 a, v4 b)
{
    v4 result = 
    {
        ((a.i[0]<b.i[0])?a.i[0]:b.i[0]), 
        ((a.i[1]<b.i[1])?a.i[1]:b.i[1]),
        ((a.i[2]<b.i[2])?a.i[2]:b.i[2]),
        ((a.i[3]<b.i[3])?a.i[3]:b.i[3])
    };
    return result;
}