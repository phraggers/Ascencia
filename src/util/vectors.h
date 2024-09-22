
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
    
    /* v2 constructors*/
    v2 v2i(i32 a, i32 b);
    v2 v2u(u32 a, u32 b);
    v2 v2r(r32 a, r32 b);
    
    /* v2 operators */
    v2 v2addf(v2 v, r32 f);
    v2 v2subf(v2 v, r32 f);
    v2 v2mulf(v2 v, r32 f);
    v2 v2divf(v2 v, r32 f);

    /* v2/v2 operators */
    v2 v2addv(v2 a, v2 b);
    v2 v2subv(v2 a, v2 b);
    v2 v2mulv(v2 a, v2 b);
    v2 v2divv(v2 a, v2 b);
    
    /* v2 functions */
    bool v2zero(v2 v);
    bool v2one(v2 v);
    v2 v2sign(v2 v);
    v2 v2invert(v2 v);
    v2 v2perpl(v2 v); // left hand perpendicular
    v2 v2perpr(v2 v); // right hand perpendicular
    v2 v2rotate(v2 v, r32 r);
    r32 v2lengthsq(v2 v);
    r32 v2length(v2 v);
    v2 v2abs(v2 v);
    v2 v2clampf(v2 v, r32 min, r32 max);
    v2 v2clampfmin(v2 v, r32 min);
    v2 v2clampfmax(v2 v, r32 max);
    v2 v2floor(v2 v);
    v2 v2ceil(v2 v);
    v2 v2round(v2 v);
    v2 v2norm(v2 v);
    r32 v2angle(v2 v);
    v2 v2snapf(v2 v, r32 f);

    /* v2/v2 functions */
    v2 v2clampv(v2 v, v2 minv, v2 maxv);
    v2 v2clampvmin(v2 v, v2 minv);
    v2 v2clampvmax(v2 v, v2 maxv);
    bool v2less(v2 a, v2 b);
    bool v2greater(v2 a, v2 b);
    bool v2equal(v2 a, v2 b);
    r32 v2dot(v2 a, v2 b);
    v2 v2snapv(v2 a, v2 b);
    v2 v2max(v2 a, v2 b);
    v2 v2min(v2 a, v2 b);
    v2 v2project(v2 a, v2 b);
    v2 v2slide(v2 v, v2 vn);
    v2 v2reflect(v2 v, v2 vn);
    v2 v2lerpf(v2 a, v2 b, r32 f);
    v2 v2bez3(v2 a, v2 b, v2 c, r32 f);
    v2 v2bez4(v2 a, v2 b, v2 c, v2 d, r32 f);
    r32 v2distance(v2 a, v2 b);
    r32 v2distancesq(v2 a, v2 b);
    bool v2linear_independent(v2 a, v2 b);

    // ------ v3 ------ //
    
    typedef union
    {
        r32 i[3];
        union {struct{r32 x,y;}; v2 xy; r32 z;};
        struct {r32 PAD0; v2 yz;};
        struct {r32 u,v,w;};
        struct {r32 r,g,b;};
    } v3;
    
    /* v3 constructors */
    v3 v3i(i32 a, i32 b, i32 c);
    v3 v3u(u32 a, u32 b, u32 c);
    v3 v3r(r32 a, r32 b, r32 c);
    
    /* v3 operators */
    v3 v3addf(v3 v, r32 f);
    v3 v3subf(v3 v, r32 f);
    v3 v3mulf(v3 v, r32 f);
    v3 v3divf(v3 v, r32 f);

    /* v3/v3 operators */
    v3 v3addv(v3 a, v3 b);
    v3 v3subv(v3 a, v3 b);
    v3 v3mulv(v3 a, v3 b);
    v3 v3divv(v3 a, v3 b);
    
    /* v3 functions */
    bool v3zero(v3 v);
    bool v3one(v3 v);
    v3 v3sign(v3 v);
    v3 v3invert(v3 v);
    r32 v3lengthsq(v3 v);
    r32 v3length(v3 v);
    v3 v3abs(v3 v);
    v3 v3clampf(v3 v, r32 min, r32 max);
    v3 v3clampfmin(v3 v, r32 min);
    v3 v3clampfmax(v3 v, r32 max);
    v3 v3floor(v3 v);
    v3 v3ceil(v3 v);
    v3 v3round(v3 v);

    /* v3/v3 functions */
    v3 v3clampv(v3 v, v3 minv, v3 maxv);
    v3 v3clampvmin(v3 a, v3 min);
    v3 v3clampvmax(v3 a, v3 max);
    bool v3less(v3 a, v3 b);
    bool v3greater(v3 a, v3 b);
    bool v3equal(v3 a, v3 b);
    r32 v3dot(v3 a, v3 b);
    v3 v3snapv(v3 a, v3 b);
    v3 v3max(v3 a, v3 b);
    v3 v3min(v3 a, v3 b);
    v3 v3cross(v3 a, v3 b);
    
    // ------ v4 ------ //
    
    typedef union
    {
        r32 i[4];
        union {struct {r32 x,y,z;}; v3 xyz; r32 w;};
        union {struct {r32 r,g,b;}; v3 rgb; r32 a;};
        struct {r32 u,v,s,t;};
    } v4;
    
    /* v4 constructors */
    v4 v4i(i32 a, i32 b, i32 c, i32 d);
    v4 v4u(u32 a, u32 b, u32 c, u32 d);
    v4 v4r(r32 a, r32 b, r32 c, r32 d);
    
    /* v4 operators */
    v4 v4addf(v4 v, r32 f);
    v4 v4subf(v4 v, r32 f);
    v4 v4mulf(v4 v, r32 f);
    v4 v4divf(v4 v, r32 f);

    /* v4/v4 operators */
    v4 v4addv(v4 a, v4 b);
    v4 v4subv(v4 a, v4 b);
    v4 v4mulv(v4 a, v4 b);
    v4 v4divv(v4 a, v4 b);
    
    /* v4 functions */
    bool v4zero(v4 v);
    bool v4one(v4 v);
    v4 v4sign(v4 v);
    v4 v4invert(v4 v);
    r32 v4lengthsq(v4 v);
    r32 v4length(v4 v);
    v4 v4abs(v4 v);
    v4 v4clampf(v4 v, r32 min, r32 max);
    v4 v4clampfmin(v4 v, r32 min);
    v4 v4clampfmax(v4 v, r32 max);
    v4 v4floor(v4 v);
    v4 v4ceil(v4 v);
    v4 v4round(v4 v);

    /* v4/v4 functions */
    v4 v4clampv(v4 v, v4 minv, v4 maxv);
    v4 v4clampvmin(v4 v, v4 minv);
    v4 v4clampvmax(v4 v, v4 maxv);
    bool v4less(v4 a, v4 b);
    bool v4greater(v4 a, v4 b);
    bool v4equal(v4 a, v4 b);
    r32 v4dot(v4 a, v4 b);
    v4 v4snapv(v4 a, v4 b);
    v4 v4max(v4 a, v4 b);
    v4 v4min(v4 a, v4 b);

#endif /* ASCENCIA_UTIL_VECTORS_H */