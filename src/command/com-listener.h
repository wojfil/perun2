#ifndef COM_LISTENER_H_INCLUDED
#define COM_LISTENER_H_INCLUDED

#include "com.h"
#include "../uroboros.h"


// this command listens to the state of its running Uroboros instance
struct Command_L : Command
{
public:

   Command_L(Uroboros* uro)
      : uroboros(uro), inner(&(uro->vars.inner)) { };

protected:

   Uroboros* uroboros;
   InnerVariables* inner;

};

#define Listeni


#endif // COM_LISTENER_H_INCLUDED
