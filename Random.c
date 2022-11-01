
#include "Ascencia.h"
#include "stdlib.h"
#include "time.h"

static bool RandomIsInit = 0;

static inline void Random_Init()
{
	srand((u32)time(0));
	RandomIsInit = 1;
}

i8 Random_s8()
{
	if (!RandomIsInit) Random_Init();
	i8 Result = (i8)(rand() % ((u16)U8_MAX + 1) - I8_MAX);
	if (Random_u8m(0, 1)) Result = -Result;
	return Result;
}

i8 Random_s8m(i8 _Min, i8 _Max)
{
	if (!RandomIsInit) Random_Init();
	i8 Result = (i8)((rand() % ((_Max + 1) - _Min)) + _Min);
	if (Random_u8m(0, 1)) Result = -Result;
	return Result;
}

u8 Random_u8()
{
	if (!RandomIsInit) Random_Init();
	return (u8)(rand() % ((u16)U8_MAX + 1));
}

u8 Random_u8m(u8 _Min, u8 _Max)
{
	if (!RandomIsInit) Random_Init();
	return (u8)((rand() % ((_Max + 1) - _Min)) + _Min);
}

i16 Random_s16()
{
	if (!RandomIsInit) Random_Init();
	i16 Result = (i16)(rand() % ((u32)U16_MAX + 1) - I16_MAX);
	if (Random_u8m(0, 1)) Result = -Result;
	return Result;
}

i16 Random_s16m(i16 _Min, i16 _Max)
{
	if (!RandomIsInit) Random_Init();
	i16 Result = (i16)((rand() % ((_Max + 1) - _Min)) + _Min);
	if (Random_u8m(0, 1)) Result = -Result;
	return Result;
}

u16 Random_u16()
{
	if (!RandomIsInit) Random_Init();
	return (u16)(rand() % ((u32)U16_MAX + 1));
}

u16 Random_u16m(u16 _Min, u16 _Max)
{
	if (!RandomIsInit) Random_Init();
	return (u16)((rand() % ((_Max + 1) - _Min)) + _Min);
}

i32 Random_s32()
{
	if (!RandomIsInit) Random_Init();

	typedef union
	{
		i16 half_num[2];
		i32 whole_num;
	} num;

	num n = { 0 };
	n.half_num[0] = Random_s16();
	n.half_num[1] = Random_s16();

	i32 Result = n.whole_num;
	if (Random_u8m(0, 1)) Result = -Result;
	return Result;
}

i32 Random_s32m(i32 _Min, i32 _Max)
{
	if (!RandomIsInit) Random_Init();
	i32 Result = ((rand() % ((_Max + 1) - _Min)) + _Min);
	if (Random_u8m(0, 1)) Result = -Result;
	return Result;
}

u32 Random_u32()
{
	if (!RandomIsInit) Random_Init();

	typedef union
	{
		u16 half_num[2];
		u32 whole_num;
	} num;

	num n = { 0 };
	n.half_num[0] = Random_u16();
	n.half_num[1] = Random_u16();

	return n.whole_num;
}

u32 Random_u32m(u32 _Min, u32 _Max)
{
	if (!RandomIsInit) Random_Init();
	return (u32)((rand() % ((_Max + 1) - _Min)) + _Min);
}

i64 Random_s64()
{
	if (!RandomIsInit) Random_Init();

	typedef union
	{
		i32 half_num[2];
		i64 whole_num;
	} num;

	num n = { 0 };
	n.half_num[0] = Random_s32();
	n.half_num[1] = Random_s32();

	i64 Result = n.whole_num;
	if (Random_u8m(0, 1)) Result = -Result;
	return Result;
}

i64 Random_s64m(i64 _Min, i64 _Max)
{
	if (!RandomIsInit) Random_Init();
	i64 Result = (i64)((rand() % ((_Max + 1) - _Min)) + _Min);
	if (Random_u8m(0, 1)) Result = -Result;
	return Result;
}

u64 Random_u64()
{
	if (!RandomIsInit) Random_Init();

	typedef union
	{
		u32 half_num[2];
		u64 whole_num;
	} num;

	num n = { 0 };
	n.half_num[0] = Random_u32();
	n.half_num[1] = Random_u32();

	return n.whole_num;
}

u64 Random_u64m(u64 _Min, u64 _Max)
{
	if (!RandomIsInit) Random_Init();
	return (u64)((rand() % ((_Max + 1) - _Min)) + _Min);
}

r32 Random_r32()
{
	if (!RandomIsInit) Random_Init();
	return Random_r32m(R32_MIN, R32_MAX);
}

r32 Random_r32m(r32 _Min, r32 _Max)
{
	if (!RandomIsInit) Random_Init();
	return ((_Max - _Min) * ((r32)rand() / RAND_MAX)) + _Min;
}

r64 Random_r64()
{
	if (!RandomIsInit) Random_Init();
	return Random_r64m(R64_MIN, R64_MAX);
}

r64 Random_r64m(r64 _Min, r64 _Max)
{
	if (!RandomIsInit) Random_Init();
	r64 f = (r64)rand() / RAND_MAX;
	return _Min + f * (_Max - _Min);
}