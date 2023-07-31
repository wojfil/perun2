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
#include <vector>


namespace perun2
{

struct Token;

p_str toStr(const p_char ch);

template<typename T>
p_str toStr(const T& n)
{
   p_ostream s;
   s << n;
   return s.str();
}

p_ndouble stringToDouble(const p_str& value);

void toLower(p_str& value);
void toUpper(p_str& value);

p_constexpr p_int LETTERS_IN_ENGLISH_ALPHABET = 26;

p_constexpr p_char ROMAN_VINCULUM_THOUSAND[] =     L"I" L"̅";
extern const p_list ROMAN_STRING_LITERALS;
extern const p_list STRINGS_ASCII;

p_constexpr p_char STRING_FILE_OPEN_MODE[] =       L"rtS, ccs=UTF-8";
p_constexpr p_char STRING_WINDOWS_PATH_PREFIX[] =  L"\\\\?\\";

p_constexpr p_char EMPTY_STRING[] =                { };
p_constexpr p_char STRING_NO_TIME[] =              L"no time";
p_constexpr p_char STRING_NO_PERIOD[] =            L"no period";
p_constexpr p_char STRING_NOTHING[] =              L"nothing";
p_constexpr p_char STRING_NEVER[] =                L"never";

p_constexpr p_char STRING_ARG_VERSION[] =          L"--version";
p_constexpr p_char STRING_ARG_DOCS[] =             L"--docs";
p_constexpr p_char STRING_ARG_WEBSITE[] =          L"--website";
p_constexpr p_char STRING_ARG_HELP[] =             L"--help";

p_constexpr p_char STRING_DOT_GIT[] =              L".git";
p_constexpr p_char STRING_DOT_SVN[] =              L".svn";
p_constexpr p_char STRING_MSPAINT_EXE[] =          L"mspaint.exe";
p_constexpr p_char STRING_NOTEPAD_EXE[] =          L"notepad.exe";

p_constexpr p_char STRING_MSPAINT[] =              L"mspaint";
p_constexpr p_char STRING_PAINT[] =                L"paint";
p_constexpr p_char STRING_NOTEPAD[] =              L"notepad";
p_constexpr p_char STRING_PENDRIVE[] =             L"pendrive";
p_constexpr p_char STRING_PENDRIVES[] =            L"pendrives";

p_constexpr p_char STRING_WEEKDAY_MONDAY[] =       L"Monday";
p_constexpr p_char STRING_WEEKDAY_TUESDAY[] =      L"Tuesday";
p_constexpr p_char STRING_WEEKDAY_WEDNESDAY[] =    L"Wednesday";
p_constexpr p_char STRING_WEEKDAY_THURSDAY[] =     L"Thursday";
p_constexpr p_char STRING_WEEKDAY_FRIDAY[] =       L"Friday";
p_constexpr p_char STRING_WEEKDAY_SATURDAY[] =     L"Saturday";
p_constexpr p_char STRING_WEEKDAY_SUNDAY[] =       L"Sunday";

p_constexpr p_char STRING_MONTH_JANUARY[] =        L"January";
p_constexpr p_char STRING_MONTH_FEBRUARY[] =       L"February";
p_constexpr p_char STRING_MONTH_MARCH[] =          L"March";
p_constexpr p_char STRING_MONTH_APRIL[] =          L"April";
p_constexpr p_char STRING_MONTH_MAY[] =            L"May";
p_constexpr p_char STRING_MONTH_JUNE[] =           L"June";
p_constexpr p_char STRING_MONTH_JULY[] =           L"July";
p_constexpr p_char STRING_MONTH_AUGUST[] =         L"August";
p_constexpr p_char STRING_MONTH_SEPTEMBER[] =      L"September";
p_constexpr p_char STRING_MONTH_OCTOBER[] =        L"October";
p_constexpr p_char STRING_MONTH_NOVEMBER[] =       L"November";
p_constexpr p_char STRING_MONTH_DECEMBER[] =       L"December";

p_constexpr p_char STRING_BOOL[] =                 L"bool";
p_constexpr p_char STRING_PERIOD[] =               L"period";
p_constexpr p_char STRING_DEFINITION[] =           L"definition";
p_constexpr p_char STRING_NUMERIC_LIST[] =         L"numeric list";
p_constexpr p_char STRING_TIME_LIST[] =            L"time list";
p_constexpr p_char STRING_LIST[] =                 L"list";

p_constexpr p_char STRING_THIS[] =                 L"this";
p_constexpr p_char STRING_ACCESS[] =               L"access";
p_constexpr p_char STRING_ARCHIVE[] =              L"archive";
p_constexpr p_char STRING_CHANGE[] =               L"change";
p_constexpr p_char STRING_COMPRESSED[] =           L"compressed";
p_constexpr p_char STRING_CREATION[] =             L"creation";
p_constexpr p_char STRING_DEPTH[] =                L"depth";
p_constexpr p_char STRING_DRIVE[] =                L"drive";
p_constexpr p_char STRING_EMPTY[] =                L"empty";
p_constexpr p_char STRING_ENCRYPTED[] =            L"encrypted";
p_constexpr p_char STRING_EXISTS[] =               L"exists";
p_constexpr p_char STRING_EXTENSION[] =            L"extension";
p_constexpr p_char STRING_FULLNAME[] =             L"fullname";
p_constexpr p_char STRING_HIDDEN[] =               L"hidden";
p_constexpr p_char STRING_INDEX[] =                L"index";
p_constexpr p_char STRING_ISDIRECTORY[] =          L"isdirectory";
p_constexpr p_char STRING_ISFILE[] =               L"isfile";
p_constexpr p_char STRING_LIFETIME[] =             L"lifetime";
p_constexpr p_char STRING_LOCATION[] =             L"location";
p_constexpr p_char STRING_MODIFICATION[] =         L"modification";
p_constexpr p_char STRING_NAME[] =                 L"name";
p_constexpr p_char STRING_PARENT[] =               L"parent";
p_constexpr p_char STRING_PATH[] =                 L"path";
p_constexpr p_char STRING_READONLY[] =             L"readonly";
p_constexpr p_char STRING_SIZE[] =                 L"size";
p_constexpr p_char STRING_SUCCESS[] =              L"success";
p_constexpr p_char STRING_NOW[] =                  L"now";
p_constexpr p_char STRING_TODAY[] =                L"today";
p_constexpr p_char STRING_YESTERDAY[] =            L"yesterday";
p_constexpr p_char STRING_TOMORROW[] =             L"tomorrow";
p_constexpr p_char STRING_JANUARY[] =              L"january";
p_constexpr p_char STRING_FEBRUARY[] =             L"february";
p_constexpr p_char STRING_MARCH[] =                L"march";
p_constexpr p_char STRING_APRIL[] =                L"april";
p_constexpr p_char STRING_MAY[] =                  L"may";
p_constexpr p_char STRING_JUNE[] =                 L"june";
p_constexpr p_char STRING_JULY[] =                 L"july";
p_constexpr p_char STRING_AUGUST[] =               L"august";
p_constexpr p_char STRING_SEPTEMBER[] =            L"september";
p_constexpr p_char STRING_OCTOBER[] =              L"october";
p_constexpr p_char STRING_NOVEMBER[] =             L"november";
p_constexpr p_char STRING_DECEMBER[] =             L"december";
p_constexpr p_char STRING_MONDAY[] =               L"monday";
p_constexpr p_char STRING_TUESDAY[] =              L"tuesday";
p_constexpr p_char STRING_WEDNESDAY[] =            L"wednesday";
p_constexpr p_char STRING_THURSDAY[] =             L"thursday";
p_constexpr p_char STRING_FRIDAY[] =               L"friday";
p_constexpr p_char STRING_SATURDAY[] =             L"saturday";
p_constexpr p_char STRING_SUNDAY[] =               L"sunday";
p_constexpr p_char STRING_ALPHABET[] =             L"alphabet";
p_constexpr p_char STRING_ASCII[] =                L"ascii";
p_constexpr p_char STRING_ARGUMENTS[] =            L"arguments";
p_constexpr p_char STRING_DESKTOP[] =              L"desktop";
p_constexpr p_char STRING_PERUN2[] =               L"perun2";
p_constexpr p_char STRING_ORIGIN[] =               L"origin";
p_constexpr p_char STRING_DIRECTORIES[] =          L"directories";
p_constexpr p_char STRING_FILES[] =                L"files";
p_constexpr p_char STRING_RECURSIVEFILES[] =       L"recursivefiles";
p_constexpr p_char STRING_RECURSIVEDIRECTORIES[] = L"recursivedirectories";
p_constexpr p_char STRING_YEAR[] =                 L"year";
p_constexpr p_char STRING_MONTH[] =                L"month";
p_constexpr p_char STRING_WEEK[] =                 L"week";
p_constexpr p_char STRING_DAY[] =                  L"day";
p_constexpr p_char STRING_HOUR[] =                 L"hour";
p_constexpr p_char STRING_MINUTE[] =               L"minute";
p_constexpr p_char STRING_SECOND[] =               L"second";
p_constexpr p_char STRING_YEARS[] =                L"years";
p_constexpr p_char STRING_MONTHS[] =               L"months";
p_constexpr p_char STRING_WEEKS[] =                L"weeks";
p_constexpr p_char STRING_DAYS[] =                 L"days";
p_constexpr p_char STRING_HOURS[] =                L"hours";
p_constexpr p_char STRING_MINUTES[] =              L"minutes";
p_constexpr p_char STRING_SECONDS[] =              L"seconds";
p_constexpr p_char STRING_DATE[] =                 L"date";
p_constexpr p_char STRING_WEEKDAY[] =              L"weekday";
p_constexpr p_char STRING_WEEKDAY_CAMELCASE[] =    L"weekDay";
p_constexpr p_char STRING_ISLOWER[] =              L"islower";
p_constexpr p_char STRING_ISUPPER[] =              L"isupper";
p_constexpr p_char STRING_ISNUMBER[] =             L"isnumber";
p_constexpr p_char STRING_ISLETTER[] =             L"isletter";
p_constexpr p_char STRING_ISDIGIT[] =              L"isdigit";
p_constexpr p_char STRING_ISBINARY[] =             L"isbinary";
p_constexpr p_char STRING_ISHEX[] =                L"ishex";
p_constexpr p_char STRING_ANYINSIDE[] =            L"anyinside";
p_constexpr p_char STRING_ANY[] =                  L"any";
p_constexpr p_char STRING_EXIST[] =                L"exist";
p_constexpr p_char STRING_COUNT[] =                L"count";
p_constexpr p_char STRING_CONTAINS[] =             L"contains";
p_constexpr p_char STRING_EXISTSINSIDE[] =         L"existsinside";
p_constexpr p_char STRING_EXISTINSIDE[] =          L"existinside";
p_constexpr p_char STRING_STARTSWITH[] =           L"startswith";
p_constexpr p_char STRING_ENDSWITH[] =             L"endswith";
p_constexpr p_char STRING_FINDTEXT[] =             L"findtext";
p_constexpr p_char STRING_ABSOLUTE[] =             L"absolute";
p_constexpr p_char STRING_CEIL[] =                 L"ceil";
p_constexpr p_char STRING_FLOOR[] =                L"floor";
p_constexpr p_char STRING_ROUND[] =                L"round";
p_constexpr p_char STRING_SIGN[] =                 L"sign";
p_constexpr p_char STRING_SQRT[] =                 L"sqrt";
p_constexpr p_char STRING_TRUNCATE[] =             L"truncate";
p_constexpr p_char STRING_AVERAGE[] =              L"average";
p_constexpr p_char STRING_SUM[] =                  L"sum";
p_constexpr p_char STRING_MIN[] =                  L"min";
p_constexpr p_char STRING_MAX[] =                  L"max";
p_constexpr p_char STRING_MEDIAN[] =               L"median";
p_constexpr p_char STRING_LENGTH[] =               L"length";
p_constexpr p_char STRING_FROMBINARY[] =           L"frombinary";
p_constexpr p_char STRING_FROMHEX[] =              L"fromhex";
p_constexpr p_char STRING_NUMBER[] =               L"number";
p_constexpr p_char STRING_COUNTINSIDE[] =          L"countinside";
p_constexpr p_char STRING_POWER[] =                L"power";
p_constexpr p_char STRING_FIRST[] =                L"first";
p_constexpr p_char STRING_LAST[] =                 L"last";
p_constexpr p_char STRING_RANDOM[] =               L"random";
p_constexpr p_char STRING_AFTER[] =                L"after";
p_constexpr p_char STRING_BEFORE[] =               L"before";
p_constexpr p_char STRING_REVERSED[] =             L"reversed";
p_constexpr p_char STRING_DIGITS[] =               L"digits";
p_constexpr p_char STRING_LETTERS[] =              L"letters";
p_constexpr p_char STRING_LOWER[] =                L"lower";
p_constexpr p_char STRING_TRIM[] =                 L"trim";
p_constexpr p_char STRING_UPPER[] =                L"upper";
p_constexpr p_char STRING_REVERSE[] =              L"reverse";
p_constexpr p_char STRING_AFTERDIGITS[] =          L"afterdigits";
p_constexpr p_char STRING_AFTERLETTERS[] =         L"afterletters";
p_constexpr p_char STRING_BEFOREDIGITS[] =         L"beforedigits";
p_constexpr p_char STRING_BEFORELETTERS[] =        L"beforeletters";
p_constexpr p_char STRING_CAPITALIZE[] =           L"capitalize";
p_constexpr p_char STRING_REPLACE[] =              L"replace";
p_constexpr p_char STRING_SUBSTRING[] =            L"substring";
p_constexpr p_char STRING_CONCATENATE[] =          L"concatenate";
p_constexpr p_char STRING_STRING[] =               L"string";
p_constexpr p_char STRING_MONTHNAME[] =            L"monthname";
p_constexpr p_char STRING_WEEKDAYNAME[] =          L"weekdayname";
p_constexpr p_char STRING_JOIN[] =                 L"join";
p_constexpr p_char STRING_REPEAT[] =               L"repeat";
p_constexpr p_char STRING_LEFT[] =                 L"left";
p_constexpr p_char STRING_RIGHT[] =                L"right";
p_constexpr p_char STRING_FILL[] =                 L"fill";
p_constexpr p_char STRING_ROMAN[] =                L"roman";
p_constexpr p_char STRING_BINARY[] =               L"binary";
p_constexpr p_char STRING_HEX[] =                  L"hex";
p_constexpr p_char STRING_CHRISTMAS[] =            L"christmas";
p_constexpr p_char STRING_EASTER[] =               L"easter";
p_constexpr p_char STRING_NEWYEAR[] =              L"newyear";
p_constexpr p_char STRING_TIME[] =                 L"time";
p_constexpr p_char STRING_CHARACTERS[] =           L"characters";
p_constexpr p_char STRING_WORDS[] =                L"words";
p_constexpr p_char STRING_SPLIT[] =                L"split";
p_constexpr p_char STRING_NUMBERS[] =              L"numbers";
p_constexpr p_char STRING_SHIFTMONTH[] =           L"shiftmonth";
p_constexpr p_char STRING_SHIFTWEEKDAY[] =         L"shiftweekday";

p_constexpr p_char STRING_COPY[] =                 L"copy";
p_constexpr p_char STRING_CREATE[] =               L"create";
p_constexpr p_char STRING_CREATEFILE[] =           L"createfile";
p_constexpr p_char STRING_CREATEDIRECTORY[] =      L"createdirectory";
p_constexpr p_char STRING_CREATEFILES[] =          L"createfiles";
p_constexpr p_char STRING_CREATEDIRECTORIES[] =    L"createdirectories";
p_constexpr p_char STRING_DELETE[] =               L"delete";
p_constexpr p_char STRING_DROP[] =                 L"drop";
p_constexpr p_char STRING_HIDE[] =                 L"hide";
p_constexpr p_char STRING_LOCK[] =                 L"lock";
p_constexpr p_char STRING_MOVE[] =                 L"move";
p_constexpr p_char STRING_OPEN[] =                 L"open";
p_constexpr p_char STRING_REACCESS[] =             L"reaccess";
p_constexpr p_char STRING_RECREATE[] =             L"recreate";
p_constexpr p_char STRING_RECHANGE[] =             L"rechange";
p_constexpr p_char STRING_REMODIFY[] =             L"remodify";
p_constexpr p_char STRING_RENAME[] =               L"rename";
p_constexpr p_char STRING_SELECT[] =               L"select";
p_constexpr p_char STRING_UNHIDE[] =               L"unhide";
p_constexpr p_char STRING_UNLOCK[] =               L"unlock";
p_constexpr p_char STRING_FORCE[] =                L"force";
p_constexpr p_char STRING_STACK[] =                L"stack";
p_constexpr p_char STRING_TRUE[] =                 L"true";
p_constexpr p_char STRING_FALSE[] =                L"false";
p_constexpr p_char STRING_AND[] =                  L"and";
p_constexpr p_char STRING_OR[] =                   L"or";
p_constexpr p_char STRING_XOR[] =                  L"xor";
p_constexpr p_char STRING_NOT[] =                  L"not";
p_constexpr p_char STRING_PRINT[] =                L"print";
p_constexpr p_char STRING_RUN[] =                  L"run";
p_constexpr p_char STRING_SLEEP[] =                L"sleep";
p_constexpr p_char STRING_IN[] =                   L"in";
p_constexpr p_char STRING_LIKE[] =                 L"like";
p_constexpr p_char STRING_CATCH[] =                L"catch";
p_constexpr p_char STRING_ELSE[] =                 L"else";
p_constexpr p_char STRING_IF[] =                   L"if";
p_constexpr p_char STRING_INSIDE[] =               L"inside";
p_constexpr p_char STRING_TIMES[] =                L"times";
p_constexpr p_char STRING_WHILE[] =                L"while";
p_constexpr p_char STRING_EVERY[] =                L"every";
p_constexpr p_char STRING_FINAL[] =                L"final";
p_constexpr p_char STRING_LIMIT[] =                L"limit";
p_constexpr p_char STRING_ORDER[] =                L"order";
p_constexpr p_char STRING_SKIP[] =                 L"skip";
p_constexpr p_char STRING_WHERE[] =                L"where";
p_constexpr p_char STRING_AS[] =                   L"as";
p_constexpr p_char STRING_BY[] =                   L"by";
p_constexpr p_char STRING_TO[] =                   L"to";
p_constexpr p_char STRING_EXTENSIONLESS[] =        L"extensionless";
p_constexpr p_char STRING_WITH[] =                 L"with";
p_constexpr p_char STRING_ASC[] =                  L"asc";
p_constexpr p_char STRING_DESC[] =                 L"desc";
p_constexpr p_char STRING_BREAK[] =                L"break";
p_constexpr p_char STRING_CONTINUE[] =             L"continue";
p_constexpr p_char STRING_EXIT[] =                 L"exit";
p_constexpr p_char STRING_ERROR[] =                L"error";

extern const p_list STRINGS_ALIASES;
extern const p_list STRINGS_MONTHS;
extern const p_list STRINGS_WEEKDAYS;
extern const p_list STRINGS_PERIOD_SINGLE;
extern const p_list STRINGS_PERIOD_MULTI;
extern const p_list STRINGS_ATTR;
extern const p_list STRINGS_TIME_ATTR;
extern const p_list STRINGS_TIME_VAR;
extern const p_list STRINGS_ALTERABLE_ATTR;
extern const p_list STRINGS_VARS_IMMUTABLES;
extern const p_list STRINGS_FUNC_BOO_STR;
extern const p_list STRINGS_FUNC_NUM_NUM;
extern const p_list STRINGS_AGGRFUNC;
extern const p_list STRINGS_FUNC_STR_STR;
extern const p_list STRINGS_FUNC_STR_STR_NUM;
extern const p_list STRINGS_FUNC_STR_NUM;
extern const p_list STRINGS_FUNC_TIM_NUM;

}

#endif // STRINGS_H_INCLUDED
