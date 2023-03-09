/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COM_H
#define COM_H

#include <memory>


namespace perun2
{

struct Command
{
   virtual void run() = 0;
   virtual ~Command() { };
};

typedef std::unique_ptr<Command> _comptr;

}


namespace perun2::comm
{

struct C_DoNothing : Command
{
public:
   C_DoNothing() {};

   void run() override;
};



}

#endif /* COM_H */
