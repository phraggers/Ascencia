/* ==============================================================
   File:    dir.c
   Author:  Phraggers
   Date:    22-04-2025
   ============================================================== */

static char *PL_BasePathString;

char* PL_GetBasePath(void)
{
    if(!PL_BasePathString)
    {
        PL_BasePathString = (char*)PL_Alloc0(0xff);
        GetModuleFileNameA(0, (LPSTR)PL_BasePathString, 0xff);

        while(PL_BasePathString[strlen(PL_BasePathString)-1] != '\\')
        {
            PL_BasePathString[strlen(PL_BasePathString)-1] = 0;
        }
    }

    return PL_BasePathString;
}
