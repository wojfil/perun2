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

#include "test-in.h"
#include "test.h"
#include "../../datatype/generator/gen-bool.h"
#include "../../datatype/generator/gen-generic.h"
#include <iostream>
#include <vector>

using namespace uro;

template <typename T>
void testCase_in(const _int& caseId, const T& value, const std::vector<T>& collection, const _bool& expectedResult)
{
    _genptr<T> v(new gen::Constant<T>(value));
    _genptr<T> v2(new gen::Constant<T>(value));
    _genptr<std::vector<T>> c(new gen::Constant<std::vector<T>>(collection));

    _genptr<_bool> inConst(new gen::InConstList<T>(v, collection));
    _genptr<_bool> inVar(new gen::InList<T>(v2, c));

	const _bool resultConst = inConst->getValue();
	const _bool resultVar   = inVar->getValue();

    VERIFY(resultConst == expectedResult, caseId);
    VERIFY(resultVar == expectedResult, caseId);
}


void test_in()
{
    testCase_in <_str> (1, L"uro", {L"boros", L"uro"}, true);
    testCase_in <_str> (2, L"uro", {L"boros", L"Uro"}, false);
    testCase_in <_str> (3, L"u", { }, false);
    testCase_in <_str> (4, L"", { }, false);
    testCase_in <_str> (5, L"u", { }, false);
    testCase_in <_str> (6, L"a", { L"ag", L"ha"}, false);
    testCase_in <_str> (7, L"a", { L"ag", L"a"}, true);

    testCase_in <_num> (8, _num(1LL), { _num(2LL), _num(36LL)}, false);
    testCase_in <_num> (9, _num(36LL), { _num(2LL), _num(36LL)}, true);
    testCase_in <_num> (10, _num(12LL), { _num(12LL)}, true);
    testCase_in <_num> (11, _num(12LL), { _num(1LL)}, false);
    testCase_in <_num> (12, _num(12.0L), { _num(11LL), _num(12LL)}, true);
}
