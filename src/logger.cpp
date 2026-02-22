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
#include "logger.hpp"
#include "perun2.hpp"

namespace perun2
{

Logger::Logger()
    : isSilent(false), 
      flushBuffer(true),
      isMaxPerformance(false) { };

Logger::Logger(const Perun2Process& p2)
    : isSilent(p2.arguments.hasFlag(FLAG_SILENT)),
      flushBuffer(p2.arguments.hasFlag(FLAG_GUI)),
      isMaxPerformance(p2.arguments.hasFlag(FLAG_MAX_PERFORMANCE)) { };

void Logger::print(const p_str& value) const
{
   if (this->isMaxPerformance) {
      return;
   }

   if (this->flushBuffer) {
      p_cout << value << std::endl;
   }
   else {
      p_cout << value << CHAR_NEW_LINE;
   }
}

void Logger::emptyLine() const
{
   if (this->isMaxPerformance) {
      return;
   }
   
   if (this->flushBuffer) {
      p_cout << std::endl;
   }
   else {
      p_cout << CHAR_NEW_LINE;
   }
}

void Logger::write(const p_str& first) const
{
   p_cout << first;
}

}
