#ifndef SIDE_PROCESS_H_INCLUDED
#define SIDE_PROCESS_H_INCLUDED

#include "datatype/primitives.h"
#include <Windows.h>


struct SideProcess
{
public:
   SideProcess();

   _boo running;
   PROCESS_INFORMATION info;
};


#endif // SIDE_PROCESS_H_INCLUDED
