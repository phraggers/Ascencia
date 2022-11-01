
#ifndef _ASC_STRINGUTILS_H_
#define _ASC_STRINGUTILS_H_

// Takes a format (_Fmt) and fills in string (_Dst) with date/time
	// Format:
	// %s second 00-59
	// %m minute 00-59
	// %h 24hr hour  00-23
	// %H 12hr hour 01-12
	// %p/P AM/am or PM/pm
	// %wd/Wd/WD weekday wed/Wed/WED
	// %D day 01-31
	// %cm/Cm/CM month jan/Jan/JAN
	// %M month 01-12
	// %Y year 00-99
	// %yyy year 1900-9999
	// %zzzz timezone offset (-/+ and 4 digits)
void Util_TimeFormat(char* _Dst, const char* _Fmt);

// Input char string, returns u32 hash code
// CREDIT: Paul Hsieh: www.azillionmonkeys.com/qed/hash.html
u32 Util_HashString(const char* _String, u32 _Length);

// Returns number of chars difference between 2 c-strings, 0 if identical
int Util_StrCmp(const char* _A, const char* _B);

// Returns number of chars in c-string before terminating \0 char
u32 Util_StrLen(const char* _String);

bool Util_RemoveFile(const char* _FilePath);
bool Util_MkDir(const char* _Path);
bool Util_CopyFile(const char* _SrcFile, const char* _DstFile);
bool Util_DirExists(const char* _Path);

#endif //_ASC_STRINGUTILS_H_