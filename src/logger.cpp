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
#include "logger.h"
#include "perun2.h"

namespace perun2
{

Logger::Logger()
    : isSilent(false), flushBuffer(true) { };

Logger::Logger(const _p2& p2)
    : isSilent(p2.arguments.hasFlag(FLAG_SILENT)),
      flushBuffer(p2.arguments.hasFlag(FLAG_GUI)) { };

void Logger::print(const _str& value) const
{
   if (this->flushBuffer) {
      _cout << value << std::endl;
   }
   else {
      _cout << value << CHAR_NEW_LINE;
   }
}

void Logger::emptyLine() const
{
   if (this->flushBuffer) {
      _cout << EMPTY_STRING << std::endl;
   }
   else {
      _cout << EMPTY_STRING << CHAR_NEW_LINE;
   }
}

}
