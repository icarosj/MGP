#include "clog.h"
#include <iostream>
using namespace std;
bool clog::m_bInitialised;
int  clog::m_nLevel;

void clog::Write(int nLevel, const char *szFormat, ...)
{
    CheckInit();
    if (nLevel >= m_nLevel)
    {
        va_list args;
        va_start(args, szFormat);
        vprintf(szFormat, args);
        va_end(args);
    }
}
int clog::GetLevel()
{
  return m_nLevel;
}
void clog::SetLevel(int nLevel)
{
    m_nLevel = nLevel;
    m_bInitialised = true;
}
void clog::CheckInit()
{
    if (!m_bInitialised)
    {
        Init();
    }
}
void clog::Init()
{
    int nDfltLevel(All);
    // Retrieve your level from an environment variable, 
    // registry entry or wherecer
    SetLevel(nDfltLevel);
}



