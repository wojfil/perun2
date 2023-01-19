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

#ifndef STRINGS_H_INCLUDED
#define STRINGS_H_INCLUDED

#include "chars.h"


namespace uro
{

static const _str EMPTY_STRING =              _str();
static const _str STRING_SPACE =              toStr(CHAR_SPACE);
static const _str STRING_0 =                  toStr(DIGIT_0);
static const _str STRING_1 =                  toStr(DIGIT_1);
static const _str STRING_MINUS =              toStr(CHAR_MINUS);
static const _str STRING_ASTERISK =           toStr(CHAR_ASTERISK);
static const _str STRING_COMMA =              toStr(CHAR_COMMA);
static const _str STRING_DOUBLE_ASTERISK =    str(STRING_ASTERISK, STRING_ASTERISK);
static const _str STRING_COMMA_SPACE =        str(STRING_COMMA, STRING_SPACE);
static const _str STRING_QUOTATION_MARK =     toStr(CHAR_QUOTATION_MARK);
static const _str STRING_COLON =              toStr(CHAR_COLON);
static const _str STRING_DOT =                toStr(CHAR_DOT);
static const _str STRING_OPENING_ROUND_BRACKET =     toStr(STRING_OPENING_ROUND_BRACKET);
static const _str STRING_CLOSING_ROUND_BRACKET =     toStr(STRING_CLOSING_ROUND_BRACKET);


static const _str ROMAN_VINCULUM_THOUSAND = L"I" L"̅";
static const _str ROMAN_STRING_LITERALS[] = { L"I", L"IV", L"V", L"IX", L"X", L"XL", L"L", L"XC", L"C", L"CD", L"D", L"CM", L"M",
      (L"I" L"̅" L"V" L"̅"), (L"V" L"̅"), (L"I" L"̅" L"X" L"̅"), (L"X" L"̅"),
      (L"X" L"̅" L"L" L"̅"), (L"L" L"̅"), (L"X" L"̅" L"C" L"̅"),
      (L"C" L"̅"), (L"C" L"̅" L"D" L"̅"), (L"D" L"̅"),
      (L"C" L"̅" L"M" L"̅"), (L"M" L"̅")
};

static const _str STRING_NO_TIME =              L"no time";
static const _str STRING_NO_PERIOD =            L"no period";

static const _str STRING_WEEKDAY_MONDAY =       L"Monday";
static const _str STRING_WEEKDAY_TUESDAY =      L"Tuesday";
static const _str STRING_WEEKDAY_WEDNESDAY =    L"Wednesday";
static const _str STRING_WEEKDAY_THURSDAY =     L"Thursday";
static const _str STRING_WEEKDAY_FRIDAY =       L"Friday";
static const _str STRING_WEEKDAY_SATURDAY =     L"Saturday";
static const _str STRING_WEEKDAY_SUNDAY =       L"Sunday";

static const _str STRING_MONTH_JANUARY =        L"January";
static const _str STRING_MONTH_FEBRUARY =       L"February";
static const _str STRING_MONTH_MARCH =          L"March";
static const _str STRING_MONTH_APRIL =          L"April";
static const _str STRING_MONTH_MAY =            L"May";
static const _str STRING_MONTH_JUNE =           L"June";
static const _str STRING_MONTH_JULY =           L"July";
static const _str STRING_MONTH_AUGUST =         L"August";
static const _str STRING_MONTH_SEPTEMBER =      L"September";
static const _str STRING_MONTH_OCTOBER =        L"October";
static const _str STRING_MONTH_NOVEMBER =       L"November";
static const _str STRING_MONTH_DECEMBER =       L"December";

}


#endif // STRINGS_H_INCLUDED
