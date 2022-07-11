/*
    This file is part of Uroboros.
    Uroboros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros. If not, see <http://www.gnu.org/licenses/>.
*/

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
