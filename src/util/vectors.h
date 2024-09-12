
/*============================================================
 * Project: Ascencia
 * File: vectors.h
 * Author: Phraggers
 * Date: Wed Sep 11 2024
============================================================*/

#ifndef ASCENCIA_UTIL_VECTORS_H
#define ASCENCIA_UTIL_VECTORS_H

#include <util/types.h>

 // ------ v2 ------ //
    
    typedef union
    {
        r32 i[2];
        struct {r32 a,b;};
        struct {r32 x,y;};
        struct {r32 u,v;};
    } v2;
    
    // constructors
    v2 v2i(i32 a, i32 b);
    v2 v2u(u32 a, u32 b);
    v2 v2r(r32 a, r32 b);
    
    // operators
    v2 v2add(v2 a, v2 b);
    v2 v2sub(v2 a, v2 b);
    v2 v2mul(v2 a, r32 v);
    v2 v2div(v2 a, r32 v);
    
    v2 v2perp(v2 a); // get perpendicular vector
    v2 v2hadamard(v2 a, v2 b); // hadamard product
    r32 v2dot(v2 a, v2 b); // dot/inner product
    r32 v2lengthsq(v2 a); // a^2
    r32 v2length(v2 a); // sqrt(a^2)

    // ------ v3 ------ //
    
    typedef union
    {
        r32 i[3];
        union {struct{r32 x,y;}; v2 xy; r32 z;};
        struct {r32 PAD0; v2 yz;};
        struct {r32 u,v,w;};
        struct {r32 r,g,b;};
    } v3;
    
    // constructors
    v3 v3i(i32 a, i32 b, i32 c);
    v3 v3u(u32 a, u32 b, u32 c);
    v3 v3r(r32 a, r32 b, r32 c);
    
    // operators
    v3 v3add(v3 a, v3 b);
    v3 v3sub(v3 a, v3 b);
    v3 v3mul(v3 a, r32 v);
    v3 v3div(v3 a, r32 v);
    
    v3 v3hadamard(v3 a, v3 b); // hadamard product
    r32 v3dot(v3 a, v3 b); // dot/inner product
	r32 v3lengthsq(v3 a); // a^2
    r32 v3length(v3 a); // sqrt(a^2)
    
    // ------ v4 ------ //
    
    typedef union
    {
        r32 i[4];
        union {struct {r32 x,y,z;}; v3 xyz; r32 w;};
        union {struct {r32 r,g,b;}; v3 rgb; r32 a;};
        struct {r32 u,v,s,t;};
    } v4;
    
    // constructors
    v4 v4i(i32 a, i32 b, i32 c, i32 d);
    v4 v4u(u32 a, u32 b, u32 c, u32 d);
    v4 v4r(r32 a, r32 b, r32 c, r32 d);
    
    // operators
    v4 v4add(v4 a, v4 b);
    v4 v4sub(v4 a, v4 b);
    v4 v4mul(v4 a, r32 v);
    v4 v4div(v4 a, r32 v);
    
    v4 v4hadamard(v4 a, v4 b); // hadamard product
    r32 v4dot(v4 a, v4 b); // dot/inner product
	r32 v4lengthsq(v4 a); // a^2
    r32 v4length(v4 a); // sqrt(a^2)

#endif /* ASCENCIA_UTIL_VECTORS_H */