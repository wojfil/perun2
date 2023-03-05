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

#ifndef STRINGS_H_INCLUDED
#define STRINGS_H_INCLUDED

#include "chars.h"


namespace uro
{

inline constexpr _size LETTERS_IN_ENGLISH_ALPHABET = 26;

static const _str EMPTY_STRING =                   _str();
static const _str STRING_CHAR_SPACE =              toStr(CHAR_SPACE);
static const _str STRING_CHAR_0 =                  toStr(CHAR_0);
static const _str STRING_CHAR_1 =                  toStr(CHAR_1);
static const _str STRING_CHAR_MINUS =              toStr(CHAR_MINUS);
static const _str STRING_CHAR_ASTERISK =           toStr(CHAR_ASTERISK);
static const _str STRING_CHAR_APOSTROPHE =         toStr(CHAR_APOSTROPHE);
static const _str STRING_CHAR_COMMA =              toStr(CHAR_COMMA);
static const _str STRING_CHAR_QUOTATION_MARK =     toStr(CHAR_QUOTATION_MARK);
static const _str STRING_CHAR_COLON =              toStr(CHAR_COLON);
static const _str STRING_CHAR_DOT =                toStr(CHAR_DOT);
static const _str STRING_CHAR_OPENING_ROUND_BRACKET =     toStr(CHAR_OPENING_ROUND_BRACKET);
static const _str STRING_CHAR_CLOSING_ROUND_BRACKET =     toStr(CHAR_CLOSING_ROUND_BRACKET);

static const _str STRING_DOUBLE_ASTERISK =         str(STRING_CHAR_ASTERISK, STRING_CHAR_ASTERISK);
static const _str STRING_COMMA_SPACE =             str(STRING_CHAR_COMMA, STRING_CHAR_SPACE);

static const _str ROMAN_VINCULUM_THOUSAND = L"I" L"̅";
static const _str ROMAN_STRING_LITERALS[] = { L"I", L"IV", L"V", L"IX", L"X", L"XL", L"L", L"XC", L"C", L"CD", L"D", L"CM", L"M",
      (L"I" L"̅" L"V" L"̅"), (L"V" L"̅"), (L"I" L"̅" L"X" L"̅"), (L"X" L"̅"),
      (L"X" L"̅" L"L" L"̅"), (L"L" L"̅"), (L"X" L"̅" L"C" L"̅"),
      (L"C" L"̅"), (L"C" L"̅" L"D" L"̅"), (L"D" L"̅"),
      (L"C" L"̅" L"M" L"̅"), (L"M" L"̅")
};

static const _list STRINGS_ASCII = {
      L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ",
      L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ", L" ",
      L" ", L"!", L"\"", L"#", L"$", L"%", L"&", L"'", L"(", L")", L"*", L"+", L",", L"-", L".", L"/",
      L"0", L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L":", L";", L"<", L"=", L">", L"?",
      L"@", L"A", L"B", L"C", L"D", L"E", L"F", L"G", L"H", L"I", L"J", L"K", L"L", L"M", L"N", L"O",
      L"P", L"Q", L"R", L"S", L"T", L"U", L"V", L"W", L"X", L"Y", L"Z", L"[", L"\\", L"]", L"^", L"_",
      L"`", L"a", L"b", L"c", L"d", L"e", L"f", L"g", L"h", L"i", L"j", L"k", L"l", L"m", L"n", L"o",
      L"p", L"q", L"r", L"s", L"t", L"u", L"v", L"w", L"x", L"y", L"z", L"{", L"|", L"}", L"~", L" "
};

static const _str STRING_NO_TIME =              L"no time";
static const _str STRING_NO_PERIOD =            L"no period";
static const _str STRING_NOTHING =              L"nothing";

static const _str STRING_VERSION =              L"version";
static const _str STRING_DOCS =                 L"docs";
static const _str STRING_WEBSITE =              L"website";
static const _str STRING_HELP =                 L"help";
static const _str STRING_URO =                  L"uro";
static const _str STRING_GIT =                  L"git";
static const _str STRING_SVN =                  L"svn";

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

static const _str STRING_BOOL =                 L"bool";
static const _str STRING_PERIOD =               L"period";
static const _str STRING_DEFINITION =           L"definition";
static const _str STRING_NUMERIC_LIST =         L"numeric list";
static const _str STRING_TIME_LIST =            L"time list";
static const _str STRING_LIST =                 L"list";

static const _str STRING_THIS =                 L"this";
static const _str STRING_ACCESS =               L"access";
static const _str STRING_ARCHIVE =              L"archive";
static const _str STRING_CHANGE =               L"change";
static const _str STRING_COMPRESSED =           L"compressed";
static const _str STRING_CREATION =             L"creation";
static const _str STRING_DEPTH =                L"depth";
static const _str STRING_DRIVE =                L"drive";
static const _str STRING_EMPTY =                L"empty";
static const _str STRING_ENCRYPTED =            L"encrypted";
static const _str STRING_EXISTS =               L"exists";
static const _str STRING_EXTENSION =            L"extension";
static const _str STRING_FULLNAME =             L"fullname";
static const _str STRING_HIDDEN =               L"hidden";
static const _str STRING_INDEX =                L"index";
static const _str STRING_ISDIRECTORY =          L"isdirectory";
static const _str STRING_ISFILE =               L"isfile";
static const _str STRING_LIFETIME =             L"lifetime";
static const _str STRING_LOCATION =             L"location";
static const _str STRING_MODIFICATION =         L"modification";
static const _str STRING_NAME =                 L"name";
static const _str STRING_PARENT =               L"parent";
static const _str STRING_PATH =                 L"path";
static const _str STRING_READONLY =             L"readonly";
static const _str STRING_SIZE =                 L"size";
static const _str STRING_SUCCESS =              L"success";
static const _str STRING_NOW =                  L"now";
static const _str STRING_TODAY =                L"today";
static const _str STRING_YESTERDAY =            L"yesterday";
static const _str STRING_TOMORROW =             L"tomorrow";
static const _str STRING_JANUARY =              L"january";
static const _str STRING_FEBRUARY =             L"february";
static const _str STRING_MARCH =                L"march";
static const _str STRING_APRIL =                L"april";
static const _str STRING_MAY =                  L"may";
static const _str STRING_JUNE =                 L"june";
static const _str STRING_JULY =                 L"july";
static const _str STRING_AUGUST =               L"august";
static const _str STRING_SEPTEMBER =            L"september";
static const _str STRING_OCTOBER =              L"october";
static const _str STRING_NOVEMBER =             L"november";
static const _str STRING_DECEMBER =             L"december";
static const _str STRING_MONDAY =               L"monday";
static const _str STRING_TUESDAY =              L"tuesday";
static const _str STRING_WEDNESDAY =            L"wednesday";
static const _str STRING_THURSDAY =             L"thursday";
static const _str STRING_FRIDAY =               L"friday";
static const _str STRING_SATURDAY =             L"saturday";
static const _str STRING_SUNDAY =               L"sunday";
static const _str STRING_ALPHABET =             L"alphabet";
static const _str STRING_ASCII =                L"ascii";
static const _str STRING_ARGUMENTS =            L"arguments";
static const _str STRING_DESKTOP =              L"desktop";
static const _str STRING_UROBOROS2 =            L"uroboros2";
static const _str STRING_ORIGIN =               L"origin";
static const _str STRING_DIRECTORIES =          L"directories";
static const _str STRING_FILES =                L"files";
static const _str STRING_RECURSIVEFILES =       L"recursivefiles";
static const _str STRING_RECURSIVEDIRECTORIES = L"recursivedirectories";
static const _str STRING_YEAR =                 L"year";
static const _str STRING_MONTH =                L"month";
static const _str STRING_WEEK =                 L"week";
static const _str STRING_DAY =                  L"day";
static const _str STRING_HOUR =                 L"hour";
static const _str STRING_MINUTE =               L"minute";
static const _str STRING_SECOND =               L"second";
static const _str STRING_YEARS =                L"years";
static const _str STRING_MONTHS =               L"months";
static const _str STRING_WEEKS =                L"weeks";
static const _str STRING_DAYS =                 L"days";
static const _str STRING_HOURS =                L"hours";
static const _str STRING_MINUTES =              L"minutes";
static const _str STRING_SECONDS =              L"seconds";
static const _str STRING_DATE =                 L"date";
static const _str STRING_WEEKDAY =              L"weekday";
static const _str STRING_ISLOWER =              L"islower";
static const _str STRING_ISUPPER =              L"isupper";
static const _str STRING_ISNUMBER =             L"isnumber";
static const _str STRING_ISLETTER =             L"isletter";
static const _str STRING_ISDIGIT =              L"isdigit";
static const _str STRING_ISBINARY =             L"isbinary";
static const _str STRING_ISHEX =                L"ishex";
static const _str STRING_ANYINSIDE =            L"anyinside";
static const _str STRING_CONTAINS =             L"contains";
static const _str STRING_EXISTSINSIDE =         L"existsinside";
static const _str STRING_EXISTINSIDE =          L"existinside";
static const _str STRING_STARTSWITH =           L"startswith";
static const _str STRING_ENDSWITH =             L"endswith";
static const _str STRING_FINDTEXT =             L"findtext";
static const _str STRING_ABSOLUTE =             L"absolute";
static const _str STRING_CEIL =                 L"ceil";
static const _str STRING_FLOOR =                L"floor";
static const _str STRING_ROUND =                L"round";
static const _str STRING_SIGN =                 L"sign";
static const _str STRING_SQRT =                 L"sqrt";
static const _str STRING_TRUNCATE =             L"truncate";
static const _str STRING_AVERAGE =              L"average";
static const _str STRING_SUM =                  L"sum";
static const _str STRING_MIN =                  L"min";
static const _str STRING_MAX =                  L"max";
static const _str STRING_MEDIAN =               L"median";
static const _str STRING_LENGTH =               L"length";
static const _str STRING_FROMBINARY =           L"frombinary";
static const _str STRING_FROMHEX =              L"fromhex";
static const _str STRING_NUMBER =               L"number";
static const _str STRING_COUNTINSIDE =          L"countinside";
static const _str STRING_POWER =                L"power";
static const _str STRING_FIRST =                L"first";
static const _str STRING_LAST =                 L"last";
static const _str STRING_RANDOM =               L"random";
static const _str STRING_AFTER =                L"after";
static const _str STRING_BEFORE =               L"before";
static const _str STRING_REVERSED =             L"reversed";
static const _str STRING_DIGITS =               L"digits";
static const _str STRING_LETTERS =              L"letters";
static const _str STRING_LOWER =                L"lower";
static const _str STRING_TRIM =                 L"trim";
static const _str STRING_UPPER =                L"upper";
static const _str STRING_REVERSE =              L"reverse";
static const _str STRING_AFTERDIGITS =          L"afterdigits";
static const _str STRING_AFTERLETTERS =         L"afterletters";
static const _str STRING_BEFOREDIGITS =         L"beforedigits";
static const _str STRING_BEFORELETTERS =        L"beforeletters";
static const _str STRING_CAPITALIZE =           L"capitalize";
static const _str STRING_REPLACE =              L"replace";
static const _str STRING_SUBSTRING =            L"substring";
static const _str STRING_CONCATENATE =          L"concatenate";
static const _str STRING_STRING =               L"string";
static const _str STRING_MONTHNAME =            L"monthname";
static const _str STRING_WEEKDAYNAME =          L"weekdayname";
static const _str STRING_JOIN =                 L"join";
static const _str STRING_REPEAT =               L"repeat";
static const _str STRING_LEFT =                 L"left";
static const _str STRING_RIGHT =                L"right";
static const _str STRING_FILL =                 L"fill";
static const _str STRING_ROMAN =                L"roman";
static const _str STRING_BINARY =               L"binary";
static const _str STRING_HEX =                  L"hex";
static const _str STRING_CHRISTMAS =            L"christmas";
static const _str STRING_EASTER =               L"easter";
static const _str STRING_NEWYEAR =              L"newyear";
static const _str STRING_TIME =                 L"time";
static const _str STRING_CHARACTERS =           L"characters";
static const _str STRING_WORDS =                L"words";
static const _str STRING_SPLIT =                L"split";
static const _str STRING_NUMBERS =              L"numbers";

static const _str STRING_COPY =                 L"copy";
static const _str STRING_CREATE =               L"create";
static const _str STRING_CREATEFILE =           L"createfile";
static const _str STRING_CREATEDIRECTORY =      L"createdirectory";
static const _str STRING_CREATEFILES =          L"createfiles";
static const _str STRING_CREATEDIRECTORIES =    L"createdirectories";
static const _str STRING_DELETE =               L"delete";
static const _str STRING_DROP =                 L"drop";
static const _str STRING_HIDE =                 L"hide";
static const _str STRING_LOCK =                 L"lock";
static const _str STRING_MOVE =                 L"move";
static const _str STRING_OPEN =                 L"open";
static const _str STRING_REACCESS =             L"reaccess";
static const _str STRING_RECREATE =             L"recreate";
static const _str STRING_RECHANGE =             L"rechange";
static const _str STRING_REMODIFY =             L"remodify";
static const _str STRING_RENAME =               L"rename";
static const _str STRING_SELECT =               L"select";
static const _str STRING_UNHIDE =               L"unhide";
static const _str STRING_UNLOCK =               L"unlock";
static const _str STRING_FORCE =                L"force";
static const _str STRING_STACK =                L"stack";
static const _str STRING_TRUE =                 L"true";
static const _str STRING_FALSE =                L"false";
static const _str STRING_AND =                  L"and";
static const _str STRING_OR =                   L"or";
static const _str STRING_XOR =                  L"xor";
static const _str STRING_NOT =                  L"not";
static const _str STRING_PRINT =                L"print";
static const _str STRING_RUN =                  L"run";
static const _str STRING_SLEEP =                L"sleep";
static const _str STRING_IN =                   L"in";
static const _str STRING_LIKE =                 L"like";
static const _str STRING_CATCH =                L"catch";
static const _str STRING_ELSE =                 L"else";
static const _str STRING_IF =                   L"if";
static const _str STRING_INSIDE =               L"inside";
static const _str STRING_TIMES =                L"times";
static const _str STRING_WHILE =                L"while";
static const _str STRING_EVERY =                L"every";
static const _str STRING_FINAL =                L"final";
static const _str STRING_LIMIT =                L"limit";
static const _str STRING_ORDER =                L"order";
static const _str STRING_SKIP =                 L"skip";
static const _str STRING_WHERE =                L"where";
static const _str STRING_AS =                   L"as";
static const _str STRING_BY =                   L"by";
static const _str STRING_FROM =                 L"from";
static const _str STRING_TO =                   L"to";
static const _str STRING_EXTENSIONLESS =        L"extensionless";
static const _str STRING_WITH =                 L"with";
static const _str STRING_ASC =                  L"asc";
static const _str STRING_DESC =                 L"desc";
static const _str STRING_BREAK =                L"break";
static const _str STRING_CONTINUE =             L"continue";
static const _str STRING_EXIT =                 L"exit";
static const _str STRING_ERROR =                L"error";

}

#endif // STRINGS_H_INCLUDED
