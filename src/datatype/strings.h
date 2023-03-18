/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STRINGS_H_INCLUDED
#define STRINGS_H_INCLUDED

#include "chars.h"


namespace perun2
{

_str toStr(const _char ch);

template<typename T>
_str toStr(const T& n)
{
   _ostream s;
   s << n;
   return s.str();
}

// the following function
// is the default method of string concatenation in Perun2
// this is probably the most efficient way to do that
template<typename... Args>
_str str(Args const&... args)
{
   _size len = 0;
   using value_type = std::common_type_t<Args const&...>;
   for (auto const& arg : { static_cast<value_type>(args)...} ) {
      len += arg.size();
   }

   _str result;
   result.reserve(len);
   for (auto const& arg : { static_cast<value_type>(args)...} ) {
      result += arg;
   }

   return result;
}

_ndouble stringToDouble(const _str& value);

void toLower(_str& value);
void toUpper(_str& value);

_constexpr _size LETTERS_IN_ENGLISH_ALPHABET = 26;

_const_str EMPTY_STRING =                   _str();
_const_str STRING_CHAR_SPACE =              toStr(CHAR_SPACE);
_const_str STRING_CHAR_0 =                  toStr(CHAR_0);
_const_str STRING_CHAR_1 =                  toStr(CHAR_1);
_const_str STRING_CHAR_MINUS =              toStr(CHAR_MINUS);
_const_str STRING_CHAR_ASTERISK =           toStr(CHAR_ASTERISK);
_const_str STRING_CHAR_APOSTROPHE =         toStr(CHAR_APOSTROPHE);
_const_str STRING_CHAR_COMMA =              toStr(CHAR_COMMA);
_const_str STRING_CHAR_QUOTATION_MARK =     toStr(CHAR_QUOTATION_MARK);
_const_str STRING_CHAR_COLON =              toStr(CHAR_COLON);
_const_str STRING_CHAR_DOT =                toStr(CHAR_DOT);
_const_str STRING_CHAR_OPENING_ROUND_BRACKET =     toStr(CHAR_OPENING_ROUND_BRACKET);
_const_str STRING_CHAR_CLOSING_ROUND_BRACKET =     toStr(CHAR_CLOSING_ROUND_BRACKET);

_const_str STRING_DOUBLE_ASTERISK =         str(STRING_CHAR_ASTERISK, STRING_CHAR_ASTERISK);
_const_str STRING_COMMA_SPACE =             str(STRING_CHAR_COMMA, STRING_CHAR_SPACE);

_const_str ROMAN_VINCULUM_THOUSAND = L"I" L"̅";
_const_str ROMAN_STRING_LITERALS[] = { L"I", L"IV", L"V", L"IX", L"X", L"XL", L"L", L"XC", L"C", L"CD", L"D", L"CM", L"M",
      (L"I" L"̅" L"V" L"̅"), (L"V" L"̅"), (L"I" L"̅" L"X" L"̅"), (L"X" L"̅"),
      (L"X" L"̅" L"L" L"̅"), (L"L" L"̅"), (L"X" L"̅" L"C" L"̅"),
      (L"C" L"̅"), (L"C" L"̅" L"D" L"̅"), (L"D" L"̅"),
      (L"C" L"̅" L"M" L"̅"), (L"M" L"̅")
};

_const_list STRINGS_ASCII = {
      L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ",
      L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ",
      L" ", L"!", L"\"", L"#", L"$", L"%", L"&", L"'", L"(", L")", L"*", L"+", L",", L"-", L".", L"/",
      L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L":", L";", L"<", L"=", L">", L"?",
      L"@", L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O",
      L"P", L"Q", L"R", L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z", L"[", L"\\", L"]", L"^", L"_",
      L"`", L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h", L"i", L"j", L"k", L"l", L"m", L"n", L"o",
      L"p", L"q", L"r", L"s", L"t", L"u", L"v", L"w", L"x", L"y", L"z", L"{", L"|", L"}", L"~", L" "
};

_const_str STRING_NO_TIME =              L"no time";
_const_str STRING_NO_PERIOD =            L"no period";
_const_str STRING_NOTHING =              L"nothing";

_const_str STRING_VERSION =              L"version";
_const_str STRING_DOCS =                 L"docs";
_const_str STRING_WEBSITE =              L"website";
_const_str STRING_HELP =                 L"help";
_const_str STRING_GIT =                  L"git";
_const_str STRING_SVN =                  L"svn";

_const_str STRING_WEEKDAY_MONDAY =       L"Monday";
_const_str STRING_WEEKDAY_TUESDAY =      L"Tuesday";
_const_str STRING_WEEKDAY_WEDNESDAY =    L"Wednesday";
_const_str STRING_WEEKDAY_THURSDAY =     L"Thursday";
_const_str STRING_WEEKDAY_FRIDAY =       L"Friday";
_const_str STRING_WEEKDAY_SATURDAY =     L"Saturday";
_const_str STRING_WEEKDAY_SUNDAY =       L"Sunday";

_const_str STRING_MONTH_JANUARY =        L"January";
_const_str STRING_MONTH_FEBRUARY =       L"February";
_const_str STRING_MONTH_MARCH =          L"March";
_const_str STRING_MONTH_APRIL =          L"April";
_const_str STRING_MONTH_MAY =            L"May";
_const_str STRING_MONTH_JUNE =           L"June";
_const_str STRING_MONTH_JULY =           L"July";
_const_str STRING_MONTH_AUGUST =         L"August";
_const_str STRING_MONTH_SEPTEMBER =      L"September";
_const_str STRING_MONTH_OCTOBER =        L"October";
_const_str STRING_MONTH_NOVEMBER =       L"November";
_const_str STRING_MONTH_DECEMBER =       L"December";

_const_str STRING_BOOL =                 L"bool";
_const_str STRING_PERIOD =               L"period";
_const_str STRING_DEFINITION =           L"definition";
_const_str STRING_NUMERIC_LIST =         L"numeric list";
_const_str STRING_TIME_LIST =            L"time list";
_const_str STRING_LIST =                 L"list";

_const_str STRING_THIS =                 L"this";
_const_str STRING_ACCESS =               L"access";
_const_str STRING_ARCHIVE =              L"archive";
_const_str STRING_CHANGE =               L"change";
_const_str STRING_COMPRESSED =           L"compressed";
_const_str STRING_CREATION =             L"creation";
_const_str STRING_DEPTH =                L"depth";
_const_str STRING_DRIVE =                L"drive";
_const_str STRING_EMPTY =                L"empty";
_const_str STRING_ENCRYPTED =            L"encrypted";
_const_str STRING_EXISTS =               L"exists";
_const_str STRING_EXTENSION =            L"extension";
_const_str STRING_FULLNAME =             L"fullname";
_const_str STRING_HIDDEN =               L"hidden";
_const_str STRING_INDEX =                L"index";
_const_str STRING_ISDIRECTORY =          L"isdirectory";
_const_str STRING_ISFILE =               L"isfile";
_const_str STRING_LIFETIME =             L"lifetime";
_const_str STRING_LOCATION =             L"location";
_const_str STRING_MODIFICATION =         L"modification";
_const_str STRING_NAME =                 L"name";
_const_str STRING_PARENT =               L"parent";
_const_str STRING_PATH =                 L"path";
_const_str STRING_READONLY =             L"readonly";
_const_str STRING_SIZE =                 L"size";
_const_str STRING_SUCCESS =              L"success";
_const_str STRING_NOW =                  L"now";
_const_str STRING_TODAY =                L"today";
_const_str STRING_YESTERDAY =            L"yesterday";
_const_str STRING_TOMORROW =             L"tomorrow";
_const_str STRING_JANUARY =              L"january";
_const_str STRING_FEBRUARY =             L"february";
_const_str STRING_MARCH =                L"march";
_const_str STRING_APRIL =                L"april";
_const_str STRING_MAY =                  L"may";
_const_str STRING_JUNE =                 L"june";
_const_str STRING_JULY =                 L"july";
_const_str STRING_AUGUST =               L"august";
_const_str STRING_SEPTEMBER =            L"september";
_const_str STRING_OCTOBER =              L"october";
_const_str STRING_NOVEMBER =             L"november";
_const_str STRING_DECEMBER =             L"december";
_const_str STRING_MONDAY =               L"monday";
_const_str STRING_TUESDAY =              L"tuesday";
_const_str STRING_WEDNESDAY =            L"wednesday";
_const_str STRING_THURSDAY =             L"thursday";
_const_str STRING_FRIDAY =               L"friday";
_const_str STRING_SATURDAY =             L"saturday";
_const_str STRING_SUNDAY =               L"sunday";
_const_str STRING_ALPHABET =             L"alphabet";
_const_str STRING_ASCII =                L"ascii";
_const_str STRING_ARGUMENTS =            L"arguments";
_const_str STRING_DESKTOP =              L"desktop";
_const_str STRING_PERUN2 =               L"perun2";
_const_str STRING_ORIGIN =               L"origin";
_const_str STRING_DIRECTORIES =          L"directories";
_const_str STRING_FILES =                L"files";
_const_str STRING_RECURSIVEFILES =       L"recursivefiles";
_const_str STRING_RECURSIVEDIRECTORIES = L"recursivedirectories";
_const_str STRING_YEAR =                 L"year";
_const_str STRING_MONTH =                L"month";
_const_str STRING_WEEK =                 L"week";
_const_str STRING_DAY =                  L"day";
_const_str STRING_HOUR =                 L"hour";
_const_str STRING_MINUTE =               L"minute";
_const_str STRING_SECOND =               L"second";
_const_str STRING_YEARS =                L"years";
_const_str STRING_MONTHS =               L"months";
_const_str STRING_WEEKS =                L"weeks";
_const_str STRING_DAYS =                 L"days";
_const_str STRING_HOURS =                L"hours";
_const_str STRING_MINUTES =              L"minutes";
_const_str STRING_SECONDS =              L"seconds";
_const_str STRING_DATE =                 L"date";
_const_str STRING_WEEKDAY =              L"weekday";
_const_str STRING_WEEKDAY_CAMELCASE =    L"weekDay";
_const_str STRING_ISLOWER =              L"islower";
_const_str STRING_ISUPPER =              L"isupper";
_const_str STRING_ISNUMBER =             L"isnumber";
_const_str STRING_ISLETTER =             L"isletter";
_const_str STRING_ISDIGIT =              L"isdigit";
_const_str STRING_ISBINARY =             L"isbinary";
_const_str STRING_ISHEX =                L"ishex";
_const_str STRING_ANYINSIDE =            L"anyinside";
_const_str STRING_ANY =                  L"any";
_const_str STRING_EXIST =                L"exist";
_const_str STRING_COUNT =                L"count";
_const_str STRING_CONTAINS =             L"contains";
_const_str STRING_EXISTSINSIDE =         L"existsinside";
_const_str STRING_EXISTINSIDE =          L"existinside";
_const_str STRING_STARTSWITH =           L"startswith";
_const_str STRING_ENDSWITH =             L"endswith";
_const_str STRING_FINDTEXT =             L"findtext";
_const_str STRING_ABSOLUTE =             L"absolute";
_const_str STRING_CEIL =                 L"ceil";
_const_str STRING_FLOOR =                L"floor";
_const_str STRING_ROUND =                L"round";
_const_str STRING_SIGN =                 L"sign";
_const_str STRING_SQRT =                 L"sqrt";
_const_str STRING_TRUNCATE =             L"truncate";
_const_str STRING_AVERAGE =              L"average";
_const_str STRING_SUM =                  L"sum";
_const_str STRING_MIN =                  L"min";
_const_str STRING_MAX =                  L"max";
_const_str STRING_MEDIAN =               L"median";
_const_str STRING_LENGTH =               L"length";
_const_str STRING_FROMBINARY =           L"frombinary";
_const_str STRING_FROMHEX =              L"fromhex";
_const_str STRING_NUMBER =               L"number";
_const_str STRING_COUNTINSIDE =          L"countinside";
_const_str STRING_POWER =                L"power";
_const_str STRING_FIRST =                L"first";
_const_str STRING_LAST =                 L"last";
_const_str STRING_RANDOM =               L"random";
_const_str STRING_AFTER =                L"after";
_const_str STRING_BEFORE =               L"before";
_const_str STRING_REVERSED =             L"reversed";
_const_str STRING_DIGITS =               L"digits";
_const_str STRING_LETTERS =              L"letters";
_const_str STRING_LOWER =                L"lower";
_const_str STRING_TRIM =                 L"trim";
_const_str STRING_UPPER =                L"upper";
_const_str STRING_REVERSE =              L"reverse";
_const_str STRING_AFTERDIGITS =          L"afterdigits";
_const_str STRING_AFTERLETTERS =         L"afterletters";
_const_str STRING_BEFOREDIGITS =         L"beforedigits";
_const_str STRING_BEFORELETTERS =        L"beforeletters";
_const_str STRING_CAPITALIZE =           L"capitalize";
_const_str STRING_REPLACE =              L"replace";
_const_str STRING_SUBSTRING =            L"substring";
_const_str STRING_CONCATENATE =          L"concatenate";
_const_str STRING_STRING =               L"string";
_const_str STRING_MONTHNAME =            L"monthname";
_const_str STRING_WEEKDAYNAME =          L"weekdayname";
_const_str STRING_JOIN =                 L"join";
_const_str STRING_REPEAT =               L"repeat";
_const_str STRING_LEFT =                 L"left";
_const_str STRING_RIGHT =                L"right";
_const_str STRING_FILL =                 L"fill";
_const_str STRING_ROMAN =                L"roman";
_const_str STRING_BINARY =               L"binary";
_const_str STRING_HEX =                  L"hex";
_const_str STRING_CHRISTMAS =            L"christmas";
_const_str STRING_EASTER =               L"easter";
_const_str STRING_NEWYEAR =              L"newyear";
_const_str STRING_TIME =                 L"time";
_const_str STRING_CHARACTERS =           L"characters";
_const_str STRING_WORDS =                L"words";
_const_str STRING_SPLIT =                L"split";
_const_str STRING_NUMBERS =              L"numbers";

_const_str STRING_COPY =                 L"copy";
_const_str STRING_CREATE =               L"create";
_const_str STRING_CREATEFILE =           L"createfile";
_const_str STRING_CREATEDIRECTORY =      L"createdirectory";
_const_str STRING_CREATEFILES =          L"createfiles";
_const_str STRING_CREATEDIRECTORIES =    L"createdirectories";
_const_str STRING_DELETE =               L"delete";
_const_str STRING_DROP =                 L"drop";
_const_str STRING_HIDE =                 L"hide";
_const_str STRING_LOCK =                 L"lock";
_const_str STRING_MOVE =                 L"move";
_const_str STRING_OPEN =                 L"open";
_const_str STRING_REACCESS =             L"reaccess";
_const_str STRING_RECREATE =             L"recreate";
_const_str STRING_RECHANGE =             L"rechange";
_const_str STRING_REMODIFY =             L"remodify";
_const_str STRING_RENAME =               L"rename";
_const_str STRING_SELECT =               L"select";
_const_str STRING_UNHIDE =               L"unhide";
_const_str STRING_UNLOCK =               L"unlock";
_const_str STRING_FORCE =                L"force";
_const_str STRING_STACK =                L"stack";
_const_str STRING_TRUE =                 L"true";
_const_str STRING_FALSE =                L"false";
_const_str STRING_AND =                  L"and";
_const_str STRING_OR =                   L"or";
_const_str STRING_XOR =                  L"xor";
_const_str STRING_NOT =                  L"not";
_const_str STRING_PRINT =                L"print";
_const_str STRING_RUN =                  L"run";
_const_str STRING_SLEEP =                L"sleep";
_const_str STRING_IN =                   L"in";
_const_str STRING_LIKE =                 L"like";
_const_str STRING_CATCH =                L"catch";
_const_str STRING_ELSE =                 L"else";
_const_str STRING_IF =                   L"if";
_const_str STRING_INSIDE =               L"inside";
_const_str STRING_TIMES =                L"times";
_const_str STRING_WHILE =                L"while";
_const_str STRING_EVERY =                L"every";
_const_str STRING_FINAL =                L"final";
_const_str STRING_LIMIT =                L"limit";
_const_str STRING_ORDER =                L"order";
_const_str STRING_SKIP =                 L"skip";
_const_str STRING_WHERE =                L"where";
_const_str STRING_AS =                   L"as";
_const_str STRING_BY =                   L"by";
_const_str STRING_TO =                   L"to";
_const_str STRING_EXTENSIONLESS =        L"extensionless";
_const_str STRING_WITH =                 L"with";
_const_str STRING_ASC =                  L"asc";
_const_str STRING_DESC =                 L"desc";
_const_str STRING_BREAK =                L"break";
_const_str STRING_CONTINUE =             L"continue";
_const_str STRING_EXIT =                 L"exit";
_const_str STRING_ERROR =                L"error";

}

#endif // STRINGS_H_INCLUDED
