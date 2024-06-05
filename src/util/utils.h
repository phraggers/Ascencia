/*============================================================
 * Project: Ascencia
 * File: utils.h
 * Author: Phraggers
 * Date: Mon May 13 2024
============================================================*/

#ifdef ASC_HEAD

#include <math.h>
#include <openssl/evp.h>

/* Maths Helpers */
i32 ASC_sign(i32 v); // sign of
r32 ASC_abs(r32 v); // absolute value
i32 ASC_round_i32(r32 v); // round r32 to nearest i32
u32 ASC_round_u32(r32 v); // round r32 to nearest u32
i32 ASC_trunc_i32(r32 v); // truncate r32 to i32
i32 ASC_floor_i32(r32 v); // floor r32 to i32
i32 ASC_ceil_i32(r32 v); // ceil r32 to i32
r32 ASC_div(r32 n, r32 d); // divide n/d, if d==0, returns 0.0f
r64 ASC_norm64(r64 v, r64 min, r64 max); // normalize 0.0 - 1.0 (64bit)
r32 ASC_norm32(r32 v, r32 min, r32 max); // normalize 0.0 - 1.0 (32bit)    
r32 ASC_clamp(r32 v, r32 min, r32 max); // clamp between min and max
r32 ASC_clampMin(r32 v, r32 min); // clamp to >= min
r32 ASC_clampMax(r32 v, r32 max); // clamp to <= max
r32 ASC_clampN(r32 v); // clamp to normalized value >= 0.0, <= 1.0
u32 ASC_rotl(u32 v, i32 s); // rotate bits left (s times)
u32 ASC_rotr(u32 v, i32 s); // rotate bits right (s times)
r32 ASC_lerp(r32 a, r32 b, r32 t); // linear interpolation
r32 ASC_qlerp(r32 a, r32 b, r32 t); // quick lerp (fewer CPU cycles but less precise)
r64 ASC_pi64(void); // returns 64bit pi
r32 ASC_pi32(void); // returns 32bit pi
r64 ASC_tau64(void); // returns 64bit 2*pi
r32 ASC_tau32(void); // returns 32bit 2*pi
r32 ASC_sqrt(r32 v); // square root
r32 ASC_square(r32 v); // v*v
r32 ASC_pow32(r32 v, r32 s); // v^s
r32 ASC_sin(r32 a);
r32 ASC_cos(r32 a);
r32 ASC_atan2(r32 y, r32 x);

// get hashed u32 from input (inputSize = sizeof input)
static u32 ASC_Hash32bit(ptr input, u64 inputSize);
// auto sizeof macro (just pass address of var, no cast)
#define ASC_HASH32(v) ASC_Hash32bit((ptr)v, sizeof *(v))

/** hash 64 bytes, input string, get 64 byte char array
 *  Example usage: char hash[64]; ASC_Hash64byte("MySuperSecurePassword", hash); */
static void ASC_Hash64byte(cstr input, char output[64]);

// get psuedo random normalized number (0.0 to 1.0)
static r32 ASC_Rand(void);
// OPTIONAL: seed ASC_Rand using any sized type (ASC_Rand will otherwise seed using time 0)
static void ASC_SeedRand(ptr value, u64 valueSize); // eg: int seed = 1234; ASC_SeedRand(&seed, sizeof(int));
// auto sizeof macro (just pass address of var, no cast)
#define ASC_SEEDRAND(v) ASC_SeedRand((ptr)v, sizeof *(v))
// Rand macro helpers to cast to PL int types 8-32
#define ASC_RANDI8 ((i8)ASC_round_i32(((ASC_Rand()*(r32)UINT8_MAX)-((r32)UINT8_MAX/2))))
#define ASC_RANDU8 ((u8)ASC_round_u32((ASC_Rand()*(r32)UINT8_MAX)))
#define ASC_RANDI16 ((i16)ASC_round_i32(((ASC_Rand()*UINT16_MAX)-((r32)UINT16_MAX/2))))
#define ASC_RANDU16 ((u16)ASC_round_u32((ASC_Rand()*(r32)UINT16_MAX)))
#define ASC_RANDI32 (ASC_round_i32(((ASC_Rand()*UINT32_MAX)-((r32)UINT32_MAX/2))))
#define ASC_RANDU32 (ASC_round_u32((ASC_Rand()*(r32)UINT32_MAX)))

#endif

/*========================================================================================*/

#ifdef ASC_IMPL

i32 ASC_sign(i32 v)
{
    return (v>=0)? 1 : -1;
}

r32 ASC_abs(r32 v)
{
    return (r32)fabs(v);
}

i32 ASC_round_i32(r32 v)
{
    return (i32)floorf(v);
}

u32 ASC_round_u32(r32 v)
{
    return (u32)floorf(v);
}

i32 ASC_trunc_i32(r32 v)
{
    return (i32)v;
}

i32 ASC_floor_i32(r32 v)
{
    return (i32)floorf(v);
}

i32 ASC_ceil_i32(r32 v)
{
    return (i32)ceilf(v);
}

r32 ASC_div(r32 n, r32 d)
{
    if(d != 0.0f)
    {
        return n/d;
    }
    
    return 0.0f;
}

r64 ASC_norm64(r64 v, r64 min, r64 max)
{
    return ((v-min) / (max-min));
}

r32 ASC_norm32(r32 v, r32 min, r32 max)
{
    return ((v-min) / (max-min));
}

r32 ASC_clamp(r32 v, r32 min, r32 max)
{
    return ((((v<min)?min:v)>max)?max:v);
}

r32 ASC_clampMin(r32 v, r32 min)
{
    return ((v<min)?min:v);
}

r32 ASC_clampMax(r32 v, r32 max)
{
    return ((v>max)?max:v);
}

r32 ASC_clampNorm(r32 v)
{
    return ((((v<0.0f)?0.0f:v)>1.0f)?1.0f:v);
}

u32 ASC_rotl(u32 v, i32 s)
{
    s &= 31;
    return ((v<<s) | (v>>(32-s)));
}

u32 ASC_rotr(u32 v, i32 s)
{
    s &= 31;
    return ((v>>s) | (v<<(32-s)));
}

r32 ASC_lerp(r32 a, r32 b, r32 t)
{
    return ((1.0f-t) *a) + (t*b);
}

r32 ASC_qlerp(r32 a, r32 b, r32 t)
{
    return a + t * (b - a);
}

r32 ASC_sqrt(r32 v)
{
    return sqrtf(v);
}

r32 ASC_square(r32 v)
{
    return v*v;
}

r32 ASC_pow32(r32 v, r32 s)
{
    return powf(v,s);
}

r64 ASC_pi64(void)
{
    return (r64)3.141592653589793238462643383279502884L;
}

r32 ASC_pi32(void)
{
    return (r32)3.141592653589793238463;
}

r64 ASC_tau64(void)
{
    return (r64)6.283185307179586476925286766559L;
}

r32 ASC_tau32(void)
{
    return (r32)6.2831853071795864769;
}

r32 ASC_sin(r32 a)
{
    return sinf(a);
}

r32 ASC_cos(r32 a)
{
    return cosf(a);
}

r32 ASC_atan2(r32 y, r32 x)
{
    return atan2f(y,x);
}

// CREDIT: modified version from Paul Hsieh: www.azillionmonkeys.com/qed/hash.html
static u32 ASC_Hash32bit(ptr input, u64 inputSize)
{
    char placeholder[4] = {0};
    for(u8 i=0;i<(u8)inputSize;i++)
    {
        i8 *p=(i8*)input;
        p+=i;
        placeholder[i]=*p;
    }
    i8 *in = (i8*)input;
    if(inputSize<4) in = (i8*)placeholder;
    u32 result = (u32)inputSize;
    u32 tmp;
    u64 len = inputSize;
    int rlen;
    
    rlen = len & 3;
    len >>= 2;
    
    while(len > 0)
    {
        result += ASC_Reinterpret(u16, in);
        tmp = (ASC_Reinterpret(u16, in + 2) << 11) ^ result;
        result = (result << 16) ^ tmp;
        in += 2 * sizeof(u16);
        result += result >> 11;
        len--;
    }
    
    switch (rlen)
    {
        case 1:
        {
            result += *in;
            result ^= result << 10;
            result += result >> 1;
        } break;
        case 2:
        {
            result += ASC_Reinterpret(u16, in);
            result ^= result << 11;
            result += result >> 17;
        } break;
        case 3:
        {
            result += ASC_Reinterpret(u16, in);
            result ^= result << 16;
            result ^= (in[sizeof(u16)]) << 18;
            result += result >> 11;
        } break;
    }
    
    result ^= result << 3;
    result += result >> 5;
    result ^= result << 4;
    result += result >> 17;
    result ^= result << 25;
    result += result >> 6;
    
    return result;
}

static void ASC_Hash64byte(cstr input, char output[64])
{
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    u8 hash[EVP_MAX_MD_SIZE];
    u32 hash_len;

    md = EVP_sha256();
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, 0);
    EVP_DigestUpdate(mdctx, input, strlen(input));
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);
    EVP_MD_CTX_free(mdctx);

    for(u32 hashIndex = 0; 
    hashIndex < hash_len-1; 
    hashIndex++) 
    {
        sprintf(output + (hashIndex*2), "%02x", hash[hashIndex]);
    }

    output[63] = '\0';
}

static b8 ASC_RAND_IS_INIT;
typedef union
{
    u8 rb[4];
    u32 ru32;
} ASC_RAND_UNION;

static r32 ASC_Rand(void)
{
    if(!ASC_RAND_IS_INIT)
    {
        time_t t = time(0);
        srand(ASC_Hash32bit(&t, sizeof(time_t)));
        ASC_RAND_IS_INIT = 1;
    }
    
    ASC_RAND_UNION ru;
    for(int i=0;i<4;i++) {ru.rb[i] = (u8)(rand()%0xff);}
    r32 result = ASC_norm32((r32)ru.ru32, 0.0f, (r32)UINT32_MAX);
    
    return result;
}

static void ASC_SeedRand(ptr value, u64 valueSize)
{
    srand(ASC_Hash32bit(value, valueSize));
    ASC_RAND_IS_INIT = 1;
}

#endif //UTILS_H_