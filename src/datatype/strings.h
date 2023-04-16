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


void insertStr(_str& result);

template<typename T, typename... Args>
void insertStr(_str& result, const T& firstValue, Args const&... args)
{
   result += firstValue;
   insertStr(result, args...);
}

template<typename... Args>
_str str(Args const&... args)
{
   _str result;
   insertStr(result, args...);
   return result;
}

_ndouble stringToDouble(const _str& value);

void toLower(_str& value);
void toUpper(_str& value);

_constexpr _int LETTERS_IN_ENGLISH_ALPHABET = 26;

_constexpr _char STRING_DOUBLE_ASTERISK[] =         { CHAR_ASTERISK, CHAR_ASTERISK };
_constexpr _char STRING_COMMA_SPACE[] =             { CHAR_COMMA, CHAR_SPACE };

_constexpr _char ROMAN_VINCULUM_THOUSAND[] =     L"I" L"̅";
extern const _list ROMAN_STRING_LITERALS;
extern const _list STRINGS_ASCII;

_constexpr _char STRING_FILE_OPEN_MODE[] =       L"rtS, ccs=UTF-8";

_constexpr _char STRING_NO_TIME[] =              L"no time";
_constexpr _char STRING_NO_PERIOD[] =            L"no period";
_constexpr _char STRING_NOTHING[] =              L"nothing";

_constexpr _char STRING_ARG_VERSION[] =          L"--version";
_constexpr _char STRING_ARG_DOCS[] =             L"--docs";
_constexpr _char STRING_ARG_WEBSITE[] =          L"--website";
_constexpr _char STRING_ARG_HELP[] =             L"--help";

_constexpr _char STRING_DOT_GIT[] =              L".git";
_constexpr _char STRING_DOT_SVN[] =              L".svn";

_constexpr _char STRING_WEEKDAY_MONDAY[] =       L"Monday";
_constexpr _char STRING_WEEKDAY_TUESDAY[] =      L"Tuesday";
_constexpr _char STRING_WEEKDAY_WEDNESDAY[] =    L"Wednesday";
_constexpr _char STRING_WEEKDAY_THURSDAY[] =     L"Thursday";
_constexpr _char STRING_WEEKDAY_FRIDAY[] =       L"Friday";
_constexpr _char STRING_WEEKDAY_SATURDAY[] =     L"Saturday";
_constexpr _char STRING_WEEKDAY_SUNDAY[] =       L"Sunday";

_constexpr _char STRING_MONTH_JANUARY[] =        L"January";
_constexpr _char STRING_MONTH_FEBRUARY[] =       L"February";
_constexpr _char STRING_MONTH_MARCH[] =          L"March";
_constexpr _char STRING_MONTH_APRIL[] =          L"April";
_constexpr _char STRING_MONTH_MAY[] =            L"May";
_constexpr _char STRING_MONTH_JUNE[] =           L"June";
_constexpr _char STRING_MONTH_JULY[] =           L"July";
_constexpr _char STRING_MONTH_AUGUST[] =         L"August";
_constexpr _char STRING_MONTH_SEPTEMBER[] =      L"September";
_constexpr _char STRING_MONTH_OCTOBER[] =        L"October";
_constexpr _char STRING_MONTH_NOVEMBER[] =       L"November";
_constexpr _char STRING_MONTH_DECEMBER[] =       L"December";

_constexpr _char STRING_BOOL[] =                 L"bool";
_constexpr _char STRING_PERIOD[] =               L"period";
_constexpr _char STRING_DEFINITION[] =           L"definition";
_constexpr _char STRING_NUMERIC_LIST[] =         L"numeric list";
_constexpr _char STRING_TIME_LIST[] =            L"time list";
_constexpr _char STRING_LIST[] =                 L"list";

_constexpr _char STRING_THIS[] =                 L"this";
_constexpr _char STRING_ACCESS[] =               L"access";
_constexpr _char STRING_ARCHIVE[] =              L"archive";
_constexpr _char STRING_CHANGE[] =               L"change";
_constexpr _char STRING_COMPRESSED[] =           L"compressed";
_constexpr _char STRING_CREATION[] =             L"creation";
_constexpr _char STRING_DEPTH[] =                L"depth";
_constexpr _char STRING_DRIVE[] =                L"drive";
_constexpr _char STRING_EMPTY[] =                L"empty";
_constexpr _char STRING_ENCRYPTED[] =            L"encrypted";
_constexpr _char STRING_EXISTS[] =               L"exists";
_constexpr _char STRING_EXTENSION[] =            L"extension";
_constexpr _char STRING_FULLNAME[] =             L"fullname";
_constexpr _char STRING_HIDDEN[] =               L"hidden";
_constexpr _char STRING_INDEX[] =                L"index";
_constexpr _char STRING_ISDIRECTORY[] =          L"isdirectory";
_constexpr _char STRING_ISFILE[] =               L"isfile";
_constexpr _char STRING_LIFETIME[] =             L"lifetime";
_constexpr _char STRING_LOCATION[] =             L"location";
_constexpr _char STRING_MODIFICATION[] =         L"modification";
_constexpr _char STRING_NAME[] =                 L"name";
_constexpr _char STRING_PARENT[] =               L"parent";
_constexpr _char STRING_PATH[] =                 L"path";
_constexpr _char STRING_READONLY[] =             L"readonly";
_constexpr _char STRING_SIZE[] =                 L"size";
_constexpr _char STRING_SUCCESS[] =              L"success";
_constexpr _char STRING_NOW[] =                  L"now";
_constexpr _char STRING_TODAY[] =                L"today";
_constexpr _char STRING_YESTERDAY[] =            L"yesterday";
_constexpr _char STRING_TOMORROW[] =             L"tomorrow";
_constexpr _char STRING_JANUARY[] =              L"january";
_constexpr _char STRING_FEBRUARY[] =             L"february";
_constexpr _char STRING_MARCH[] =                L"march";
_constexpr _char STRING_APRIL[] =                L"april";
_constexpr _char STRING_MAY[] =                  L"may";
_constexpr _char STRING_JUNE[] =                 L"june";
_constexpr _char STRING_JULY[] =                 L"july";
_constexpr _char STRING_AUGUST[] =               L"august";
_constexpr _char STRING_SEPTEMBER[] =            L"september";
_constexpr _char STRING_OCTOBER[] =              L"october";
_constexpr _char STRING_NOVEMBER[] =             L"november";
_constexpr _char STRING_DECEMBER[] =             L"december";
_constexpr _char STRING_MONDAY[] =               L"monday";
_constexpr _char STRING_TUESDAY[] =              L"tuesday";
_constexpr _char STRING_WEDNESDAY[] =            L"wednesday";
_constexpr _char STRING_THURSDAY[] =             L"thursday";
_constexpr _char STRING_FRIDAY[] =               L"friday";
_constexpr _char STRING_SATURDAY[] =             L"saturday";
_constexpr _char STRING_SUNDAY[] =               L"sunday";
_constexpr _char STRING_ALPHABET[] =             L"alphabet";
_constexpr _char STRING_ASCII[] =                L"ascii";
_constexpr _char STRING_ARGUMENTS[] =            L"arguments";
_constexpr _char STRING_DESKTOP[] =              L"desktop";
_constexpr _char STRING_PERUN2[] =               L"perun2";
_constexpr _char STRING_ORIGIN[] =               L"origin";
_constexpr _char STRING_DIRECTORIES[] =          L"directories";
_constexpr _char STRING_FILES[] =                L"files";
_constexpr _char STRING_RECURSIVEFILES[] =       L"recursivefiles";
_constexpr _char STRING_RECURSIVEDIRECTORIES[] = L"recursivedirectories";
_constexpr _char STRING_YEAR[] =                 L"year";
_constexpr _char STRING_MONTH[] =                L"month";
_constexpr _char STRING_WEEK[] =                 L"week";
_constexpr _char STRING_DAY[] =                  L"day";
_constexpr _char STRING_HOUR[] =                 L"hour";
_constexpr _char STRING_MINUTE[] =               L"minute";
_constexpr _char STRING_SECOND[] =               L"second";
_constexpr _char STRING_YEARS[] =                L"years";
_constexpr _char STRING_MONTHS[] =               L"months";
_constexpr _char STRING_WEEKS[] =                L"weeks";
_constexpr _char STRING_DAYS[] =                 L"days";
_constexpr _char STRING_HOURS[] =                L"hours";
_constexpr _char STRING_MINUTES[] =              L"minutes";
_constexpr _char STRING_SECONDS[] =              L"seconds";
_constexpr _char STRING_DATE[] =                 L"date";
_constexpr _char STRING_WEEKDAY[] =              L"weekday";
_constexpr _char STRING_WEEKDAY_CAMELCASE[] =    L"weekDay";
_constexpr _char STRING_ISLOWER[] =              L"islower";
_constexpr _char STRING_ISUPPER[] =              L"isupper";
_constexpr _char STRING_ISNUMBER[] =             L"isnumber";
_constexpr _char STRING_ISLETTER[] =             L"isletter";
_constexpr _char STRING_ISDIGIT[] =              L"isdigit";
_constexpr _char STRING_ISBINARY[] =             L"isbinary";
_constexpr _char STRING_ISHEX[] =                L"ishex";
_constexpr _char STRING_ANYINSIDE[] =            L"anyinside";
_constexpr _char STRING_ANY[] =                  L"any";
_constexpr _char STRING_EXIST[] =                L"exist";
_constexpr _char STRING_COUNT[] =                L"count";
_constexpr _char STRING_CONTAINS[] =             L"contains";
_constexpr _char STRING_EXISTSINSIDE[] =         L"existsinside";
_constexpr _char STRING_EXISTINSIDE[] =          L"existinside";
_constexpr _char STRING_STARTSWITH[] =           L"startswith";
_constexpr _char STRING_ENDSWITH[] =             L"endswith";
_constexpr _char STRING_FINDTEXT[] =             L"findtext";
_constexpr _char STRING_ABSOLUTE[] =             L"absolute";
_constexpr _char STRING_CEIL[] =                 L"ceil";
_constexpr _char STRING_FLOOR[] =                L"floor";
_constexpr _char STRING_ROUND[] =                L"round";
_constexpr _char STRING_SIGN[] =                 L"sign";
_constexpr _char STRING_SQRT[] =                 L"sqrt";
_constexpr _char STRING_TRUNCATE[] =             L"truncate";
_constexpr _char STRING_AVERAGE[] =              L"average";
_constexpr _char STRING_SUM[] =                  L"sum";
_constexpr _char STRING_MIN[] =                  L"min";
_constexpr _char STRING_MAX[] =                  L"max";
_constexpr _char STRING_MEDIAN[] =               L"median";
_constexpr _char STRING_LENGTH[] =               L"length";
_constexpr _char STRING_FROMBINARY[] =           L"frombinary";
_constexpr _char STRING_FROMHEX[] =              L"fromhex";
_constexpr _char STRING_NUMBER[] =               L"number";
_constexpr _char STRING_COUNTINSIDE[] =          L"countinside";
_constexpr _char STRING_POWER[] =                L"power";
_constexpr _char STRING_FIRST[] =                L"first";
_constexpr _char STRING_LAST[] =                 L"last";
_constexpr _char STRING_RANDOM[] =               L"random";
_constexpr _char STRING_AFTER[] =                L"after";
_constexpr _char STRING_BEFORE[] =               L"before";
_constexpr _char STRING_REVERSED[] =             L"reversed";
_constexpr _char STRING_DIGITS[] =               L"digits";
_constexpr _char STRING_LETTERS[] =              L"letters";
_constexpr _char STRING_LOWER[] =                L"lower";
_constexpr _char STRING_TRIM[] =                 L"trim";
_constexpr _char STRING_UPPER[] =                L"upper";
_constexpr _char STRING_REVERSE[] =              L"reverse";
_constexpr _char STRING_AFTERDIGITS[] =          L"afterdigits";
_constexpr _char STRING_AFTERLETTERS[] =         L"afterletters";
_constexpr _char STRING_BEFOREDIGITS[] =         L"beforedigits";
_constexpr _char STRING_BEFORELETTERS[] =        L"beforeletters";
_constexpr _char STRING_CAPITALIZE[] =           L"capitalize";
_constexpr _char STRING_REPLACE[] =              L"replace";
_constexpr _char STRING_SUBSTRING[] =            L"substring";
_constexpr _char STRING_CONCATENATE[] =          L"concatenate";
_constexpr _char STRING_STRING[] =               L"string";
_constexpr _char STRING_MONTHNAME[] =            L"monthname";
_constexpr _char STRING_WEEKDAYNAME[] =          L"weekdayname";
_constexpr _char STRING_JOIN[] =                 L"join";
_constexpr _char STRING_REPEAT[] =               L"repeat";
_constexpr _char STRING_LEFT[] =                 L"left";
_constexpr _char STRING_RIGHT[] =                L"right";
_constexpr _char STRING_FILL[] =                 L"fill";
_constexpr _char STRING_ROMAN[] =                L"roman";
_constexpr _char STRING_BINARY[] =               L"binary";
_constexpr _char STRING_HEX[] =                  L"hex";
_constexpr _char STRING_CHRISTMAS[] =            L"christmas";
_constexpr _char STRING_EASTER[] =               L"easter";
_constexpr _char STRING_NEWYEAR[] =              L"newyear";
_constexpr _char STRING_TIME[] =                 L"time";
_constexpr _char STRING_CHARACTERS[] =           L"characters";
_constexpr _char STRING_WORDS[] =                L"words";
_constexpr _char STRING_SPLIT[] =                L"split";
_constexpr _char STRING_NUMBERS[] =              L"numbers";

_constexpr _char STRING_COPY[] =                 L"copy";
_constexpr _char STRING_CREATE[] =               L"create";
_constexpr _char STRING_CREATEFILE[] =           L"createfile";
_constexpr _char STRING_CREATEDIRECTORY[] =      L"createdirectory";
_constexpr _char STRING_CREATEFILES[] =          L"createfiles";
_constexpr _char STRING_CREATEDIRECTORIES[] =    L"createdirectories";
_constexpr _char STRING_DELETE[] =               L"delete";
_constexpr _char STRING_DROP[] =                 L"drop";
_constexpr _char STRING_HIDE[] =                 L"hide";
_constexpr _char STRING_LOCK[] =                 L"lock";
_constexpr _char STRING_MOVE[] =                 L"move";
_constexpr _char STRING_OPEN[] =                 L"open";
_constexpr _char STRING_REACCESS[] =             L"reaccess";
_constexpr _char STRING_RECREATE[] =             L"recreate";
_constexpr _char STRING_RECHANGE[] =             L"rechange";
_constexpr _char STRING_REMODIFY[] =             L"remodify";
_constexpr _char STRING_RENAME[] =               L"rename";
_constexpr _char STRING_SELECT[] =               L"select";
_constexpr _char STRING_UNHIDE[] =               L"unhide";
_constexpr _char STRING_UNLOCK[] =               L"unlock";
_constexpr _char STRING_FORCE[] =                L"force";
_constexpr _char STRING_STACK[] =                L"stack";
_constexpr _char STRING_TRUE[] =                 L"true";
_constexpr _char STRING_FALSE[] =                L"false";
_constexpr _char STRING_AND[] =                  L"and";
_constexpr _char STRING_OR[] =                   L"or";
_constexpr _char STRING_XOR[] =                  L"xor";
_constexpr _char STRING_NOT[] =                  L"not";
_constexpr _char STRING_PRINT[] =                L"print";
_constexpr _char STRING_RUN[] =                  L"run";
_constexpr _char STRING_SLEEP[] =                L"sleep";
_constexpr _char STRING_IN[] =                   L"in";
_constexpr _char STRING_LIKE[] =                 L"like";
_constexpr _char STRING_CATCH[] =                L"catch";
_constexpr _char STRING_ELSE[] =                 L"else";
_constexpr _char STRING_IF[] =                   L"if";
_constexpr _char STRING_INSIDE[] =               L"inside";
_constexpr _char STRING_TIMES[] =                L"times";
_constexpr _char STRING_WHILE[] =                L"while";
_constexpr _char STRING_EVERY[] =                L"every";
_constexpr _char STRING_FINAL[] =                L"final";
_constexpr _char STRING_LIMIT[] =                L"limit";
_constexpr _char STRING_ORDER[] =                L"order";
_constexpr _char STRING_SKIP[] =                 L"skip";
_constexpr _char STRING_WHERE[] =                L"where";
_constexpr _char STRING_AS[] =                   L"as";
_constexpr _char STRING_BY[] =                   L"by";
_constexpr _char STRING_TO[] =                   L"to";
_constexpr _char STRING_EXTENSIONLESS[] =        L"extensionless";
_constexpr _char STRING_WITH[] =                 L"with";
_constexpr _char STRING_ASC[] =                  L"asc";
_constexpr _char STRING_DESC[] =                 L"desc";
_constexpr _char STRING_BREAK[] =                L"break";
_constexpr _char STRING_CONTINUE[] =             L"continue";
_constexpr _char STRING_EXIT[] =                 L"exit";
_constexpr _char STRING_ERROR[] =                L"error";

}

#endif // STRINGS_H_INCLUDED
