/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COM_LISTENER_H_INCLUDED
#define COM_LISTENER_H_INCLUDED

#include "com.h"
#include "../perun2.h"


namespace perun2::comm
{

// this command listens to the state of its running Perun2 instance
struct Command_L : Command
{
public:
   Command_L(p_perun2& p2) : perun2(p2) { };

protected:
   p_perun2& perun2;

};

}


#endif // COM_LISTENER_H_INCLUDED
