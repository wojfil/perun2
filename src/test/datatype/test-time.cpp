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

#include "test-time.h"
#include "test.h"
#include "../../datatype/time.h"
#include "../../datatype/period.h"
#include <iostream>

using namespace uro;

void test_time()
{
    testCase_timeShift(1, _tim(3, 6, 2000), _per(12, Period::u_Days), _tim(15, 6, 2000));
    testCase_timeShift(2, _tim(29, 6, 2020), _per(5, Period::u_Days), _tim(4, 7, 2020));
    testCase_timeShift(3, _tim(28, 2, 2020), _per(3, Period::u_Days), _tim(2, 3, 2020));
    testCase_timeShift(4, _tim(21, 6, 2022), _per(3, Period::u_Weeks), _tim(12, 7, 2022));
    testCase_timeShift(5, _tim(21, 6, 2022), _per(10, Period::u_Years), _tim(21, 6, 2032));
    testCase_timeShift(6, _tim(29, 2, 2020), _per(1, Period::u_Years), _tim(28, 2, 2021));
    testCase_timeShift(7, _tim(28, 2, 2020), _per(60, Period::u_Days), _tim(28, 4, 2020));
    testCase_timeShift(8, _tim(16, 9, 2021), _per(31, Period::u_Minutes), _tim(17, 9, 2021, 0, 31));
    testCase_timeShift(9, _tim(16, 9, 2021), _per(-31, Period::u_Minutes), _tim(15, 9, 2021, 23, 29));
    testCase_timeShift(10, _tim(21, 11, 2021), _per(4, Period::u_Months), _tim(21, 3, 2022));
    testCase_timeShift(11, _tim(21, 11, 2021, 10, 15), _per(4, Period::u_Hours), _tim(21, 11, 2021, 14, 15));
    testCase_timeShift(12, _tim(21, 11, 2021, 10, 15), _per(23, Period::u_Hours), _tim(22, 11, 2021, 9, 15));
    testCase_timeShift(13, _tim(21, 11, 2021, 10, 15, 16), _per(2, Period::u_Seconds), _tim(21, 11, 2021, 10, 15, 18));
    testCase_timeShift(14, _tim(21, 11, 2021, 10, 15, 0), _per(86340, Period::u_Seconds), _tim(22, 11, 2021, 10, 14, 0));
	
	testCase_timeListSortAndUnique(101, { _tim(3, 6, 2000), _tim(2, 6, 2000) }, { _tim(2, 6, 2000), _tim(3, 6, 2000) });
	testCase_timeListSortAndUnique(102, { _tim(3, 6, 2000), _tim(2, 7, 2007), _tim(28, 2, 2003) }, { _tim(3, 6, 2000), _tim(28, 2, 2003), _tim(2, 7, 2007) });
	testCase_timeListSortAndUnique(103, { _tim(3, 6, 2000), _tim(2, 6, 2000), _tim(2, 6, 2000), _tim(3, 6, 2000), _tim(3, 6, 2000) }, { _tim(2, 6, 2000), _tim(3, 6, 2000) });
	testCase_timeListSortAndUnique(104, { _tim(3, 6, 2000), _tim(6, 2000) }, { _tim(3, 6, 2000) });
	testCase_timeListSortAndUnique(105, { _tim(3, 6, 2000), _tim(3, 6, 2000) }, { _tim(3, 6, 2000) });
	testCase_timeListSortAndUnique(106, { _tim(3, 6, 2000) }, { _tim(3, 6, 2000) });
	testCase_timeListSortAndUnique(107, { _tim(6, 2000), _tim(3, 6, 2000) }, { _tim(3, 6, 2000) });
	testCase_timeListSortAndUnique(108, { _tim(2, 6, 2000), _tim(2, 6, 2000), _tim(3, 6, 2000), _tim(3, 6, 2000) }, { _tim(2, 6, 2000), _tim(3, 6, 2000) });
	testCase_timeListSortAndUnique(109, { _tim(6, 2000), _tim(3, 6, 2000,15, 16) }, { _tim(6, 2000) });
}

void testCase_timeShift(const _int caseId, const _tim& value, const _per& shift, const _tim& expectedResult)
{
    _tim v = value;
    v += shift;
    VERIFY(v == expectedResult, caseId);
}

void testCase_timeListSortAndUnique(const _int caseId, const _tlist& input, const _tlist& expectedResult)
{
	const _tlist result = sortedAndUniqueTimeList(input);
    VERIFY(result == expectedResult, caseId);
}

