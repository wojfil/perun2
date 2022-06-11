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

#include "test-time.h"
#include "test.h"
#include "../../datatype/time.h"
#include "../../datatype/period.h"
#include <iostream>



void test_time()
{
    testCase_timeShift(1, _tim(3, 6, 2000), _per(12, Period::u_Days), _tim(15, 6, 2000));
    testCase_timeShift(2, _tim(29, 6, 2020), _per(5, Period::u_Days), _tim(4, 7, 2020));
    testCase_timeShift(3, _tim(28, 2, 2020), _per(3, Period::u_Days), _tim(2, 3, 2020));
    // todo add more
}

void testCase_timeShift(const _int& caseId, const _tim& value, const _per& shift, const _tim& expectedResult)
{
    _tim v = value;
    v += shift;
    VERIFY(v == expectedResult, caseId);
}
