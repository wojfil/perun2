#ifndef TERMINATOR_H_INCLUDED
#define TERMINATOR_H_INCLUDED

#include "datatype/primitives.h"


struct Uroboros;

struct Terminator
{
public:
   Terminator(Uroboros* uro);

private:
   static Uroboros* uroboros;
   static _int HandlerRoutine(_ulong dwCtrlType);
};


#endif // TERMINATOR_H_INCLUDED
