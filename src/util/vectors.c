
/*============================================================
 * Project: Ascencia
 * File: vectors.c
 * Author: Phraggers
 * Date: Wed Sep 11 2024
============================================================*/

#include <util/vectors.h>
#include <util/maths.h>

// ------ v2 ------ //

// constructors
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

// maths operators
v2 v2add(v2 a, v2 b)
{
    v2 result = {a.i[0]+b.i[0], a.i[1]+b.i[1]};
    return result;
}
v2 v2sub(v2 a, v2 b)
{
    v2 result = {a.i[0]-b.i[0], a.i[1]-b.i[1]};
    return result;
}
v2 v2mul(v2 a, r32 v)
{
    v2 result = {a.i[0]*v, a.i[1]*v};
    return result;
}
v2 v2div(v2 a, r32 v)
{
    v2 result = {a.i[0]/v, a.i[1]/v};
    return result;    
}
v2 v2perp(v2 a)
{
    v2 result = {-a.y, a.x};
    return result;
}
v2 v2hadamard(v2 a, v2 b)
{
    v2 result = {a.i[0]*b.i[0], a.i[1]*b.i[1]};
    return result;
}
r32 v2dot(v2 a, v2 b)
{
    r32 result = (a.i[0]*b.i[0]) + (a.i[1]*b.i[1]);
    return result;
}
r32 v2lengthsq(v2 a)
{
    r32 result = v2dot(a,a);
    return result;
}
r32 v2length(v2 a)
{
    r32 result = square_root(v2lengthsq(a));
    return result;
}

// ------ v3 ------ //

// constructors
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

// maths operators
v3 v3add(v3 a, v3 b)
{
    v3 result = {a.i[0]+b.i[0], a.i[1]+b.i[1], a.i[2]+b.i[2]};
    return result;
}
v3 v3sub(v3 a, v3 b)
{
    v3 result = {a.i[0]-b.i[0], a.i[1]-b.i[1], a.i[2]-b.i[2]};
    return result;
}
v3 v3mul(v3 a, r32 v)
{
    v3 result = {a.i[0]*v, a.i[1]*v, a.i[2]*v};
    return result;
}
v3 v3div(v3 a, r32 v)
{
    v3 result = {a.i[0]/v, a.i[1]/v, a.i[2]/v};
    return result;    
}
v3 v3hadamard(v3 a, v3 b)
{
    v3 result = {a.i[0]*b.i[0], a.i[1]*b.i[1], a.i[2]*b.i[2]};
    return result;
}
r32 v3dot(v3 a, v3 b)
{
    r32 result = (a.i[0]*b.i[0]) + (a.i[1]*b.i[1]) + (a.i[2]*b.i[2]);
    return result;
}
r32 v3lengthsq(v3 a)
{
    r32 result = v3dot(a,a);
    return result;
}
r32 v3length(v3 a)
{
    r32 result = square_root(v3lengthsq(a));
    return result;
}

// ------ v4 ------ //

// constructors
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

// maths operators
v4 v4add(v4 a, v4 b)
{
    v4 result = {a.i[0]+b.i[0], a.i[1]+b.i[1], a.i[2]+b.i[2], a.i[3]+b.i[3]};
    return result;
}
v4 v4sub(v4 a, v4 b)
{
    v4 result = {a.i[0]-b.i[0], a.i[1]-b.i[1], a.i[2]-b.i[2], a.i[3]+b.i[3]};
    return result;
}
v4 v4mul(v4 a, r32 v)
{
    v4 result = {a.i[0]*v, a.i[1]*v, a.i[2]*v, a.i[3]*v};
    return result;
}
v4 v4div(v4 a, r32 v)
{
    v4 result = {a.i[0]/v, a.i[1]/v, a.i[2]/v, a.i[3]/v};
    return result;    
}
v4 v4hadamard(v4 a, v4 b)
{
    v4 result = {a.i[0]*b.i[0], a.i[1]*b.i[1], a.i[2]*b.i[2]};
    return result;
}
r32 v4dot(v4 a, v4 b)
{
    r32 result = (a.i[0]*b.i[0]) + (a.i[1]*b.i[1]) + (a.i[2]*b.i[2]);
    return result;
}
r32 v4lengthsq(v4 a)
{
    r32 result = v4dot(a,a);
    return result;
}
r32 v4length(v4 a)
{
    r32 result = square_root(v4lengthsq(a));
    return result;
}