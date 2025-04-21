/* ==============================================================
   File:    state.c
   Author:  Phraggers
   Date:    21-04-2025
   ============================================================== */

bool CORE_StateInit(void)
{
    gState = (CORE_sState*)malloc(sizeof(CORE_sState));
    if(!gState)
    {
        return 0;
    }
    else
    {
        memset(gState, 0, sizeof(CORE_sState));
    }



    return 1;
}
