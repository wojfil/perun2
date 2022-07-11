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

#include "test.h"
#include "test-like.h"
#include "../../datatype/generator/gen-like.h"
#include <iostream>


void test_like()
{
	testCase_like(1, L"u%", L"uro", true);
	testCase_like(2, L"b%", L"uro", false);
    testCase_like(3, L"%", L"", true);
    testCase_like(4, L"%", L" L", true);
    testCase_like(5, L"%", L"asdfa asdf asdf", true);
    testCase_like(6, L"%", L"%", true);
    testCase_like(7, L"_", L"", false);
    testCase_like(8, L"_", L" ", true);
    testCase_like(9, L"_", L"4", true);
    testCase_like(10, L"_", L"C", true);
    testCase_like(11, L"_", L"CX", false);
    testCase_like(12, L"[ABCD]", L"", false);
    testCase_like(13, L"[ABCD]", L"A", true);
    testCase_like(14, L"[ABCD]", L"b", false);
    testCase_like(15, L"[ABCD]", L"X", false);
    testCase_like(16, L"[ABCD]", L"AB", false);
    testCase_like(17, L"[B-D]", L"C", true);
    testCase_like(18, L"[B-D]", L"D", true);
    testCase_like(19, L"[B-D]", L"A", false);
    testCase_like(20, L"[^B-D]", L"C", false);
    testCase_like(21, L"[^B-D]", L"D", false);
    testCase_like(22, L"[^B-D]", L"A", true);
    testCase_like(23, L"%TEST[ABCD]XXX", L"lolTESTBXXX", true);
    testCase_like(24, L"%TEST[ABCD]XXX", L"lolTESTZXXX", false);
    testCase_like(25, L"%TEST[^ABCD]XXX", L"lolTESTBXXX", false);
    testCase_like(26, L"%TEST[^ABCD]XXX", L"lolTESTZXXX", true);
    testCase_like(27, L"%TEST[B-D]XXX", L"lolTESTBXXX", true);
    testCase_like(28, L"%TEST[^B-D]XXX", L"lolTESTZXXX", true);
    testCase_like(29, L"%Uroboros.txt", L"Uroboros.txt", true);
    testCase_like(30, L"%Uroboros.txt", L"TestUroboros.txt", true);
    testCase_like(31, L"%Uroboros.txt", L"TestUroboros.txt.img", false);
    testCase_like(32, L"%Uroboros.txt", L"Uroboros.txt.img", false);
    testCase_like(33, L"%Uroboros.txt", L"TestUroboros001.txt.img", false);
    testCase_like(34, L"Uroboros.txt%", L"Uroboros.txt", true);
    testCase_like(35, L"Uroboros.txt%", L"TestUroboros.txt", false);
    testCase_like(36, L"Uroboros.txt%", L"TestUroboros.txt.img", false);
    testCase_like(37, L"Uroboros.txt%", L"Uroboros.txt.img", true);
    testCase_like(38, L"Uroboros.txt%", L"TestUroboros001.txt.img", false);
    testCase_like(39, L"%Uroboros.txt%", L"Uroboros.txt", true);
    testCase_like(40, L"%Uroboros.txt%", L"TestUroboros.txt", true);
    testCase_like(41, L"%Uroboros.txt%", L"TestUroboros.txt.img", true);
    testCase_like(42, L"%Uroboros.txt%", L"Uroboros.txt.img", true);
    testCase_like(43, L"%Uroboros.txt%", L"TestUroboros001.txt.img", false);
    testCase_like(44, L"%Uroboros%.txt", L"Uroboros.txt", true);
    testCase_like(45, L"%Uroboros%.txt", L"TestUroboros.txt", true);
    testCase_like(46, L"%Uroboros%.txt", L"TestUroboros.txt.img", false);
    testCase_like(47, L"%Uroboros%.txt", L"Uroboros.txt.img", false);
    testCase_like(48, L"%Uroboros%.txt", L"TestUroboros001.txt.img", false);
    testCase_like(49, L"%Uroboros%.txt", L"TestUroboros001.txt", true);
    testCase_like(50, L"Uroboros%.txt%", L"Uroboros.txt", true);
    testCase_like(51, L"Uroboros%.txt%", L"TestUroboros.txt", false);
    testCase_like(52, L"Uroboros%.txt%", L"TestUroboros.txt.img", false);
    testCase_like(53, L"Uroboros%.txt%", L"Uroboros.txt.img", true);
    testCase_like(54, L"Uroboros%.txt%", L"TestUroboros001.txt.img", false);
    testCase_like(55, L"Uroboros%.txt%", L"TestUroboros001.txt", false);
    testCase_like(56, L"%Uroboros%.txt%", L"Uroboros.txt", true);
    testCase_like(57, L"%Uroboros%.txt%", L"TestUroboros.txt", true);
    testCase_like(58, L"%Uroboros%.txt%", L"TestUroboros.txt.img", true);
    testCase_like(59, L"%Uroboros%.txt%", L"Uroboros.txt.img", true);
    testCase_like(60, L"%Uroboros%.txt%", L"TestUroboros001.txt.img", true);
    testCase_like(61, L"%Uroboros%.txt%", L"TestUroboros001.txt", true);
    testCase_like(62, L"_Uroboros_.txt_", L"1Uroboros3.txt4", true);
    testCase_like(63, L"_Uroboros_.txt_", L"1Uroboros.txt4", false);
    testCase_like(64, L"_Uroboros_.txt_", L"1Uroboros3.txt", false);
    testCase_like(65, L"_Uroboros_.txt_", L"Uroboros3.txt4", false);
    testCase_like(66, L"____", L"abdc", true);
    testCase_like(67, L"____", L"abc", false);
    testCase_like(68, L"___#", L"abcd", false);
    testCase_like(69, L"___#", L"abc5", true);
    testCase_like(70, L"%#%", L"Uroboros", false);
    testCase_like(71, L"%#%", L"1Uroboros", true);
    testCase_like(72, L"%#%", L"Uroboros3", true);
    testCase_like(73, L"%#%", L"Urob7oros", true);
    testCase_like(74, L"#_#", L"3aa", false);
    testCase_like(75, L"#_#", L"320", true);
    testCase_like(76, L"#%", L"320", true);
    testCase_like(77, L"##", L"320", false);
    testCase_like(78, L"[0-9]#", L"32", true);
    testCase_like(79, L"[0-9]#", L"a2", false);
    testCase_like(80, L"ur#b#r#s", L"ur0b0r0s", true);
    testCase_like(81, L"ur#b#r#s", L"ur0b0ros", false);
    testCase_like(82, L"[0-9]g", L"a2", false);
    testCase_like(83, L"%opo%", L"Popol Vuh", true);
}


void testCase_like(const _int& caseId, const _str& pattern, const _str& value, const _boo& expectedResult)
{
	LikeComparer* comparer = parseLikeComparer(pattern);
	const _boo result = comparer->compareToPattern(value);
    VERIFY(result == expectedResult, caseId);
	delete comparer;
}
