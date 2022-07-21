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

#ifndef TEST_TIME_INCLUDED
#define TEST_TIME_INCLUDED

#include "../../datatype/time.h"
#include "../../datatype/period.h"
#include "../../datatype/datatype.h"

void test_time();

void testCase_timeShift(const _int& caseId, const _tim& value, const _per& shift, const _tim& expectedResult);
void testCase_timeListSortAndUnique(const _int& caseId, const _tlist& input, const _tlist& expectedResult);


#endif // TEST_TIME_INCLUDED
