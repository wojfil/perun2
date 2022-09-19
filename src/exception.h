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

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "datatype/primitives.h"


namespace uro
{

class SyntaxException
{
public:
   SyntaxException(const _str& msg, const _int& li);
   _str getMessage() const;

private:
   const _str message;
   const _int line;
};

class UroRuntimeException
{
public:
   UroRuntimeException(const _str& msg);
   _str getMessage() const;

private:
   const _str message;
};

}

#endif /* EXCEPTION_H */
