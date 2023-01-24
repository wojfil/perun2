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

#ifndef TEST_LIKE_INCLUDED
#define TEST_LIKE_INCLUDED

#include "../../datatype/generator/gen-like.h"

using namespace uro;

void test_like();
void testCase_like(const _int& caseId, const _str& pattern, const _str& value, const _bool& expectedResult);

#endif // TEST_LIKE_INCLUDED
