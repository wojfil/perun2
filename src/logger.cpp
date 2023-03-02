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
#include "logger.h"
#include "uroboros.h"

namespace uro
{

Logger::Logger()
    : isSilent(false) { };

Logger::Logger(_uro& uro)
    : isSilent(uro.arguments.hasFlag(FLAG_SILENT)) { };

void Logger::print(const _str& value) const
{
   std::wcout << value << CHAR_NEW_LINE;
}

void Logger::emptyLine() const
{
   std::wcout << EMPTY_STRING << CHAR_NEW_LINE;
}

}