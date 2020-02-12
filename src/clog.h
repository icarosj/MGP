#ifndef _CLOG_H_
#define _CLOG_H_

#include <cstdio>
#include <cstdarg>

enum { All=0, Minor, Major, Debug, Info, Warning, Error, Fatal, None };

class clog
{
public:
    static void Write(int nLevel, const char *szFormat, ...);
    static void Single(int nLevel, const char *szFormat, ...);
    static void SetLevel(int nLevel);
    static int GetLevel();

protected:
    static void CheckInit();
    static void Init();

private:
    clog();
    static bool m_bInitialised;
    static int  m_nLevel;
};


#endif
