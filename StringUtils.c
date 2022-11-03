
#include "Ascencia.h"
#include <time.h>

#if _WIN32
#include <direct.h>
// Windows does not define the S_ISREG and S_ISDIR macros in stat.h, so we do.
// We have to define _CRT_INTERNAL_NONSTDC_NAMES 1 before #including sys/stat.h
// in order for Microsoft's stat.h to define names like S_IFMT, S_IFREG, and S_IFDIR,
// rather than just defining  _S_IFMT, _S_IFREG, and _S_IFDIR as it normally does.
// CREDIT: some-guy @ https://stackoverflow.com/questions/11238918/s-isreg-macro-undefined
#define _CRT_INTERNAL_NONSTDC_NAMES 1
#include <sys/stat.h>
#if !defined(S_ISREG) && defined(S_IFMT) && defined(S_IFREG)
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#endif // defined
#if !defined(S_ISDIR) && defined(S_IFMT) && defined(S_IFDIR)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif // defined
#else // not _WIN32
#include <sys/types.h>
#include <sys/stat.h>
#endif // if _WIN32

#define Util_Get16Bits(Data) (*((const u16*)(Data)))

void Util_TimeFormat(char* _Dst, const char* _Fmt)
{
	time_t RawTime = time(0);
#if MSVC
	struct tm sNow;
	struct tm* Now = &sNow;
	localtime_s(Now, &RawTime);
#else
	struct tm* Now = localtime(&RawTime);
#endif

	int TimezoneOffset = 0;
	{
		time_t ZeroTime = 0;

	#if MSVC
		struct tm sLocalNow;
		struct tm* LocalNow = &sLocalNow;
		localtime_s(LocalNow, &ZeroTime);
	#else
		struct tm* LocalTime = localtime(&TimeZero);
	#endif

		int Unaligned = LocalNow->tm_sec + (LocalNow->tm_min + (LocalNow->tm_hour * 60)) * 60;
		TimezoneOffset = LocalNow->tm_mon ? Unaligned - 24 * 60 * 60 : Unaligned;
	}

	char Result[128];
	int FmtLength = 0;

	for (const char* c = _Fmt; *c != '\0'; c++)
	{
		Result[FmtLength] = *c;
		FmtLength++;
	}

	//NOTE: hex 0x30 -> 0x39 = ascii '0' -> '9'	
	for (int iChar = 0; iChar < FmtLength; iChar++)
	{
		if (Result[iChar] == '%' && iChar + 1 < FmtLength)
		{
			if (Result[iChar + 1] == 's')
			{
				Result[iChar] = (char)((Now->tm_sec / 10) + 0x30);
				Result[iChar + 1] = (char)((Now->tm_sec % 10) + 0x30);
			}

			else if (Result[iChar + 1] == 'm')
			{
				Result[iChar] = (char)((Now->tm_min / 10) + 0x30);
				Result[iChar + 1] = (char)((Now->tm_min % 10) + 0x30);
			}

			else if (Result[iChar + 1] == 'h' || Result[iChar + 1] == 'H')
			{
				int Hr = Now->tm_hour;

				if (Result[iChar + 1] == 'H')
				{
					if (Hr == 0) Hr = 12;
					else if (Hr > 12) Hr -= 12;
				}

				Result[iChar] = (char)((Hr / 10) + 0x30);
				Result[iChar + 1] = (char)((Hr % 10) + 0x30);
			}

			else if (Result[iChar + 1] == 'p' || Result[iChar + 1] == 'P')
			{
				if (Now->tm_hour > 11)
				{
					if (Result[iChar + 1] == 'p') Result[iChar] = 'p';
					else Result[iChar] = 'P';
				}

				else
				{
					if (Result[iChar + 1] == 'p') Result[iChar] = 'a';
					else Result[iChar] = 'A';
				}

				if (Result[iChar + 1] == 'p') Result[iChar + 1] = 'm';
				else Result[iChar + 1] = 'M';
			}

			else if (iChar + 2 < FmtLength &&
					 (Result[iChar + 1] == 'w' || Result[iChar + 1] == 'W'))
			{
				char wd[7][4] = {"sun","mon","tue","wed","thu","fri","sat"};

				if (Result[iChar + 1] == 'w' && (Result[iChar + 2] == 'd'))
				{
					for (int i = 0; i < 3; i++) Result[iChar + i] = wd[Now->tm_wday][i];
				}

				else if (Result[iChar + 1] == 'W' && (Result[iChar + 2] == 'd'))
				{
					for (int i = 0; i < 3; i++)
						Result[iChar + i] = wd[Now->tm_wday][i] - ((!i) ? 32 : 0);
				}

				else if (Result[iChar + 1] == 'W' && (Result[iChar + 2] == 'D'))
				{
					for (int i = 0; i < 3; i++)
						Result[iChar + i] = wd[Now->tm_wday][i] - 32;
				}
			}

			else if (Result[iChar + 1] == 'D')
			{
				Result[iChar] = (char)((Now->tm_mday / 10) + 0x30);
				Result[iChar + 1] = (char)((Now->tm_mday % 10) + 0x30);
			}

			else if (iChar + 2 < FmtLength &&
					 (Result[iChar + 1] == 'c' || Result[iChar + 1] == 'C'))
			{
				char cm[12][4] = {"jan","feb","mar","apr","may","jun",
					"jul","aug","sep","oct","nov","dec" };

				if (Result[iChar + 1] == 'c' && (Result[iChar + 2] == 'm'))
				{
					for (int i = 0; i < 3; i++) Result[iChar + i] = cm[Now->tm_mon][i];
				}

				else if (Result[iChar + 1] == 'C' && (Result[iChar + 2] == 'm'))
				{
					for (int i = 0; i < 3; i++)
						Result[iChar + i] = cm[Now->tm_mon][i] - ((!i) ? 32 : 0);
				}

				else if (Result[iChar + 1] == 'C' && (Result[iChar + 2] == 'M'))
				{
					for (int i = 0; i < 3; i++)
						Result[iChar + i] = cm[Now->tm_mon][i] - 32;
				}
			}

			else if (Result[iChar + 1] == 'M')
			{
				Result[iChar] = (char)(((Now->tm_mon + 1) / 10) + 0x30);
				Result[iChar + 1] = (char)(((Now->tm_mon + 1) % 10) + 0x30);
			}

			else if (Result[iChar + 1] == 'Y')
			{
				Result[iChar] = ((Now->tm_year % 100) / 10) + 0x30;
				Result[iChar + 1] = ((Now->tm_year % 100) % 10) + 0x30;
			}

			else if (iChar + 3 < FmtLength && Result[iChar + 1] == 'y' &&
					 Result[iChar + 2] == 'y' && Result[iChar + 3] == 'y')
			{
				Result[iChar] = (char)(((Now->tm_year + 1900) / 1000) + 0x30);
				Result[iChar + 1] = (char)((((Now->tm_year + 1900) / 100) % 10) + 0x30);
				Result[iChar + 2] = (char)(((Now->tm_year % 100) / 10) + 0x30);
				Result[iChar + 3] = (char)(((Now->tm_year % 100) % 10) + 0x30);
			}

			else if (iChar + 4 < FmtLength && Result[iChar + 1] == 'z' &&
					 Result[iChar + 2] == 'z' && Result[iChar + 3] == 'z' && Result[iChar + 4] == 'z')
			{
				Result[iChar] = (TimezoneOffset < 0) ? '-' : '+';

				if (TimezoneOffset < 0)
				{
					TimezoneOffset += (-TimezoneOffset) * 2;
				}

				Result[iChar + 1] = (char)(((TimezoneOffset) / 1000) + 0x30);
				Result[iChar + 2] = (char)((((TimezoneOffset) / 100) % 10) + 0x30);
				Result[iChar + 3] = (char)(((TimezoneOffset % 100) / 10) + 0x30);
				Result[iChar + 4] = (char)(((TimezoneOffset % 100) % 10) + 0x30);
			}
		}
	}

	char* c = _Dst;

	for (int i = 0; i < FmtLength; i++)
	{
		*c = Result[i];
		c++;
	}
}

u32 Util_HashString(const char* _String, u32 _Length)
{
	u32 Hash = _Length;
	u32 Temp;
	int Rem;

	if (_Length <= 0 || _String == 0) return 0;

	Rem = _Length & 3;
	_Length >>= 2;

	for (; _Length > 0; _Length--)
	{
		Hash += Util_Get16Bits(_String);
		Temp = (Util_Get16Bits(_String + 2) << 11) ^ Hash;
		Hash = (Hash << 16) ^ Temp;
		_String += 2 * sizeof(u16);
		Hash += Hash >> 11;
	}

	switch (Rem)
	{
		case 1:
		{
			Hash += (char)*_String;
			Hash ^= Hash << 10;
			Hash += Hash >> 1;
		} break;
		case 2:
		{
			Hash += Util_Get16Bits(_String);
			Hash ^= Hash << 11;
			Hash += Hash >> 17;
		} break;
		case 3:
		{
			Hash += Util_Get16Bits(_String);
			Hash ^= Hash << 16;
			Hash ^= ((char)_String[sizeof(u16)]) << 18;
			Hash += Hash >> 11;
		} break;
	}

	Hash ^= Hash << 3;
	Hash += Hash >> 5;
	Hash ^= Hash << 4;
	Hash += Hash >> 17;
	Hash ^= Hash << 25;
	Hash += Hash >> 6;

	return Hash;
}

int Util_StrCmp(const char* _A, const char* _B)
{
	while (*_A && *_A == *_B)
	{
		++_A;
		++_B;
	}

	return (int)((u8)(*_A)) - (int)((u8)(*_B));
}

u32 Util_StrLen(const char* _String)
{
	if (!_String) return 0;

	u32 Length = 0;

	while (*_String != '\0')
	{
		++Length;
		++_String;
	}

	return Length;
}

bool Util_RemoveFile(const char* _FilePath)
{
	if (!remove(_FilePath))
	{
		ASC_Log(LOGLEVEL_INFO, "UTILS: RemoveFile deleted [%s]", _FilePath);
		return 1;
	}
	
	else
	{
		ASC_Log(LOGLEVEL_ERROR, "UTILS: RemoveFile unable to delete [%s]", _FilePath);
		return 0;
	}
}

bool Util_MkDir(const char* _Path)
{
	int Result = 1;

#if _WIN32
	Result = _mkdir(_Path);
#else
	Result = mkdir(_Path, S_IRWXU | S_IRWXO);
#endif

	if (Result)
	{
		ASC_Log(LOGLEVEL_ERROR, "UTILS: MkDir Unable to make directory [%s]", _Path);
		return 0;
	}

	else
	{
		ASC_Log(LOGLEVEL_INFO, "UTILS: MkDir created [%s]", _Path);
		return 1;
	}
}

bool Util_CopyFile(const char* _SrcFile, const char* _DstFile)
{
	SDL_RWops* SrcFile = SDL_RWFromFile(_SrcFile, "rb");

	if (!SrcFile)
	{
		ASC_Log(LOGLEVEL_ERROR, "UTILS: CopyFile failed to open SrcFile [%s]", _SrcFile);
		return 0;
	}

	SDL_RWops* DstFile = SDL_RWFromFile(_DstFile, "wb");

	if (!DstFile)
	{
		ASC_Log(LOGLEVEL_ERROR, "UTILS: CopyFile failed to open DstFile [%s]", _DstFile);
		return 0;
	}

	i64 Size = SDL_RWsize(SrcFile);

	for (i64 i = 0; i < Size; i++)
	{
		char c;

		if (!SDL_RWread(SrcFile, &c, 1, 1))
		{
			if (i < (Size - 1))
			{
				ASC_Log(LOGLEVEL_ERROR, "UTILS: CopyFile failed to read SrcFile [%s]", _SrcFile);
				SDL_RWclose(SrcFile);
				SDL_RWclose(DstFile);
				return 0;
			}
		}

		if (!SDL_RWwrite(DstFile, &c, 1, 1))
		{
			if (i < (Size - 1))
			{
				ASC_Log(LOGLEVEL_ERROR, "UTILS: CopyFile failed to write DstFile [%s]", _DstFile);
				SDL_RWclose(SrcFile);
				SDL_RWclose(DstFile);
				return 0;
			}
		}
	}

	SDL_RWclose(SrcFile);
	SDL_RWclose(DstFile);

	ASC_Log(LOGLEVEL_INFO, "UTILS: CopyFile copied [%s] to [%s]", _SrcFile, _DstFile);
	return 1;
}

bool Util_DirExists(const char* _Path)
{
	struct stat sb;

	if (stat(_Path, &sb) == 0 && S_ISDIR(sb.st_mode))
	{
		ASC_Log(LOGLEVEL_DEBUG, "UTILS: DirExists: Yes [%s]", _Path);
		return 1;
	}
	else
	{
		ASC_Log(LOGLEVEL_DEBUG, "UTILS: DirExists: No [%s]", _Path);
		return 0;
	}
}