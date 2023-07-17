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

#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include "datatype/primitives.h"
#include "datatype/chars.h"
#include <iostream>


namespace perun2
{

struct p_perun2;

struct Logger
{
public:
   Logger();
   Logger(const p_perun2& p2);

   // print something
   void print(const _str& value) const;

   // print command log, all args are strings and are concatenated into one line
   template<typename... Args>
   void log(Args const&... args) const
   {
      if (!this->isSilent) {
         using value_type = std::common_type_t<Args const&...>;
         for (auto const& arg : {static_cast<value_type>(args)...}) {
            _cout << arg;
         }
            
         if (this->flushBuffer) {
            _cout << std::endl;
         }
         else {
            _cout << CHAR_NEW_LINE;
         }
      }
   }
    
   // print an empty line
   void emptyLine() const;

private:
   // if program was called with -s
   // it runs in silent mode and there are no logs of filesystem commands
   // however, critical error messages and Print should still work
   const _bool isSilent;

   // if the logger sends messages to the GUI program, flush the buffer every line
   // otherwise (messages go straight to the console), do not do that
   const _bool flushBuffer;
};

}

#endif // LOGGER_H_INCLUDED
