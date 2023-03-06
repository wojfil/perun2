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

#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include "datatype/primitives.h"
#include "datatype/chars.h"
#include <iostream>


namespace uro
{

struct _uro;

struct Logger
{
public:
   Logger();
   Logger(const _uro& uro);
   Logger(Logger const&) = delete;
   Logger& operator= (Logger const&) = delete;

   // print something
   void print(const _str& value) const;

   // print command log, all args concatenated in one line
   template<typename... Args>
   void log(Args const&... args) const
   {
      if (!this->isSilent) {
         using value_type = std::common_type_t<Args const&...>;
         for (auto const& arg : {static_cast<value_type>(args)...}) {
            std::wcout << arg;
         }
            
         if (this->flushBuffer) {
            std::wcout << std::endl;
         }
         else {
            std::wcout << CHAR_NEW_LINE;
         }
      }
   }
    
   // print an empty line
   void emptyLine() const;

private:
   const _bool isSilent;

   // if the logger sends messages to the GUI program, flush the buffer every line
   // otherwise (messages go straight to the console), do not do that
   const _bool flushBuffer;
};

}

#endif // LOGGER_H_INCLUDED
