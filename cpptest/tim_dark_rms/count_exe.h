
#ifndef WIN32
#include <pthread.h>
#include <unistd.h>
#else
#include "pthreadWin32.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "t_executable.h"

#ifndef _COUNT_EXE_H
#define _COUNT_EXE_H

class count_exe: public t_executable
{
public:

    int waittime;
    int id;
    
    count_exe();
    
    void maine(void);
    
    static volatile int cnt;
    static pthread_mutex_t cntlock;
    static volatile bool is_lockinit;
    
};
#endif
