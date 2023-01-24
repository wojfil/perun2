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

#ifndef BRACKETS_H
#define BRACKETS_H

#include "tokens.h"

namespace uro
{

struct BracketsInfo
{
public:
   BracketsInfo() : round(0), square(0) { };
   void refresh(const Token& tk);
   _bool isBracketFree() const;

private:
   _int round;
   _int square;
};

void checkBracketsThoroughly(const Tokens& tks);

}

#endif /* BRACKETS_H */

