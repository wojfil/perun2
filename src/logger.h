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

#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include "datatype/primitives.h"
#include "datatype/text/chars.h"
#include <iostream>


namespace perun2
{

struct p_perun2;

struct Logger
{
public:
   Logger();
   Logger(const p_perun2& p2);

   // print something in a new line
   void print(const p_str& value) const;

   // print command log in a new line
   // all args are strings and are concatenated into one line
   template<typename... Args>
   void log(const Args&... args) const
   {
      if (this->isSilent) {
         return;
      }

      this->write(args...);

      if (this->flushBuffer) {
         p_cout << std::endl;
      }
      else {
         p_cout << CHAR_NEW_LINE;
      }
   }
    
   // print an empty line
   void emptyLine() const;

private:
   template<typename... Args>
   void write(const p_str& first, const Args&... args) const
   {
      p_cout << first;
      write(args...);
   }

   void write(const p_str& first) const;

   // if program was called with -s
   // it runs in silent mode and there are no logs of filesystem commands
   // however, critical error messages and Print should still work
   const p_bool isSilent;

   // if the logger sends messages to the GUI program, flush the buffer every line
   // otherwise (messages go straight to the console), do not do that
   const p_bool flushBuffer;
};

}

#endif // LOGGER_H_INCLUDED
