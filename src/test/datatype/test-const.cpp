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

#include "test-const.h"
#include "test.h"
#include "../../datatype/generator/gen-generic.h"
#include <iostream>
#include <vector>


// check, if a generated constant stores correct data
template <typename T>
void testCase_const(const _int& caseId, const T& value)
{
    Generator<T>* v = new Constant<T>(value);
	const _boo result  = v->getValue() == value;
    VERIFY(result, caseId);
    delete v;
}

void test_const()
{
    testCase_const<_str> (1, L"a");
    testCase_const<_str> (2, L"");
    testCase_const<_str> (3, L"Uroboros");
    testCase_const<_str> (4, L"kąty");
    testCase_const<_str> (5, L"Уроборос");
    testCase_const<_num> (6, _num(7LL));
    testCase_const<_num> (7, _num(27LL));
    testCase_const<_num> (8, _num(-7LL));
    testCase_const<_num> (9, _num(7.7L));
    testCase_const<_num> (10, _num(-0.33L));
    testCase_const<_boo> (11, true);
    testCase_const<_boo> (12, false);
    testCase_const<_list> (13, {L"Uro", L"boro", L"s"});
    testCase_const<_nlist> (14, {_num(45LL), _num(3LL), _num(3.2L) });
}
