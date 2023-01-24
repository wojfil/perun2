/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COM_H
#define COM_H

#include <memory>


namespace uro
{

struct Command
{
   virtual void run() = 0;
   virtual ~Command() { };
};

typedef std::unique_ptr<Command> _comptr;

}


namespace uro::comm
{

struct C_DoNothing : Command
{
public:
   C_DoNothing() {};

   void run() override;
};



}

#endif /* COM_H */
