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

#include "hash.h"
#include "datatype/strings.h"
#include <unordered_map>
#include <algorithm>


namespace perun2
{

_hash caseInsensitiveHash(_str& value)
{
   toLower(value);
   return std::hash<_str>{}(value);
}

_hash caseSensitiveHash(const _str& value)
{
   return std::hash<_str>{}(value);
}

// prepare data for future hash comparisons
Hashes::Hashes() :
   HASH_NOTHING(caseSensitiveHash(EMPTY_STRING)),
   HASH_VAR_THIS(caseSensitiveHash(STRING_THIS)),

   HASH_VAR_ACCESS(caseSensitiveHash(STRING_ACCESS)),
   HASH_VAR_ARCHIVE(caseSensitiveHash(STRING_ARCHIVE)),
   HASH_VAR_CHANGE(caseSensitiveHash(STRING_CHANGE)),
   HASH_VAR_COMPRESSED(caseSensitiveHash(STRING_COMPRESSED)),
   HASH_VAR_CREATION(caseSensitiveHash(STRING_CREATION)),
   HASH_VAR_DEPTH(caseSensitiveHash(STRING_DEPTH)),
   HASH_VAR_DRIVE(caseSensitiveHash(STRING_DRIVE)),
   HASH_VAR_EMPTY(caseSensitiveHash(STRING_EMPTY)),
   HASH_VAR_ENCRYPTED(caseSensitiveHash(STRING_ENCRYPTED)),
   HASH_VAR_EXISTS(caseSensitiveHash(STRING_EXISTS)),
   HASH_VAR_EXTENSION(caseSensitiveHash(STRING_EXTENSION)),
   HASH_VAR_FULLNAME(caseSensitiveHash(STRING_FULLNAME)),
   HASH_VAR_HIDDEN(caseSensitiveHash(STRING_HIDDEN)),
   HASH_VAR_INDEX(caseSensitiveHash(STRING_INDEX)),
   HASH_VAR_ISDIRECTORY(caseSensitiveHash(STRING_ISDIRECTORY)),
   HASH_VAR_ISFILE(caseSensitiveHash(STRING_ISFILE)),
   HASH_VAR_LIFETIME(caseSensitiveHash(STRING_LIFETIME)),
   HASH_VAR_LOCATION(caseSensitiveHash(STRING_LOCATION)),
   HASH_VAR_MODIFICATION(caseSensitiveHash(STRING_MODIFICATION)),
   HASH_VAR_NAME(caseSensitiveHash(STRING_NAME)),
   HASH_VAR_PARENT(caseSensitiveHash(STRING_PARENT)),
   HASH_VAR_PATH(caseSensitiveHash(STRING_PATH)),
   HASH_VAR_READONLY(caseSensitiveHash(STRING_READONLY)),
   HASH_VAR_SIZE(caseSensitiveHash(STRING_SIZE)),
   HASH_VAR_SUCCESS(caseSensitiveHash(STRING_SUCCESS)),

   HASH_VAR_NOW(caseSensitiveHash(STRING_NOW)),
   HASH_VAR_TODAY(caseSensitiveHash(STRING_TODAY)),
   HASH_VAR_YESTERDAY(caseSensitiveHash(STRING_YESTERDAY)),
   HASH_VAR_TOMORROW(caseSensitiveHash(STRING_TOMORROW)),

   HASH_VAR_JANUARY(caseSensitiveHash(STRING_JANUARY)),
   HASH_VAR_FEBRUARY(caseSensitiveHash(STRING_FEBRUARY)),
   HASH_VAR_MARCH(caseSensitiveHash(STRING_MARCH)),
   HASH_VAR_APRIL(caseSensitiveHash(STRING_APRIL)),
   HASH_VAR_MAY(caseSensitiveHash(STRING_MAY)),
   HASH_VAR_JUNE(caseSensitiveHash(STRING_JUNE)),
   HASH_VAR_JULY(caseSensitiveHash(STRING_JULY)),
   HASH_VAR_AUGUST(caseSensitiveHash(STRING_AUGUST)),
   HASH_VAR_SEPTEMBER(caseSensitiveHash(STRING_SEPTEMBER)),
   HASH_VAR_OCTOBER(caseSensitiveHash(STRING_OCTOBER)),
   HASH_VAR_NOVEMBER(caseSensitiveHash(STRING_NOVEMBER)),
   HASH_VAR_DECEMBER(caseSensitiveHash(STRING_DECEMBER)),

   HASH_VAR_MONDAY(caseSensitiveHash(STRING_MONDAY)),
   HASH_VAR_TUESDAY(caseSensitiveHash(STRING_TUESDAY)),
   HASH_VAR_WEDNESDAY(caseSensitiveHash(STRING_WEDNESDAY)),
   HASH_VAR_THURSDAY(caseSensitiveHash(STRING_THURSDAY)),
   HASH_VAR_FRIDAY(caseSensitiveHash(STRING_FRIDAY)),
   HASH_VAR_SATURDAY(caseSensitiveHash(STRING_SATURDAY)),
   HASH_VAR_SUNDAY(caseSensitiveHash(STRING_SUNDAY)),

   HASH_VAR_ALPHABET(caseSensitiveHash(STRING_ALPHABET)),
   HASH_VAR_ASCII(caseSensitiveHash(STRING_ASCII)),
   HASH_VAR_ARGUMENTS(caseSensitiveHash(STRING_ARGUMENTS)),

   HASH_VAR_DESKTOP(caseSensitiveHash(STRING_DESKTOP)),
   HASH_VAR_PERUN2(caseSensitiveHash(STRING_PERUN2)),
   HASH_VAR_ORIGIN(caseSensitiveHash(STRING_ORIGIN)),

   HASH_VAR_DIRECTORIES(caseSensitiveHash(STRING_DIRECTORIES)),
   HASH_VAR_FILES(caseSensitiveHash(STRING_FILES)),
   HASH_VAR_RECURSIVEFILES(caseSensitiveHash(STRING_RECURSIVEFILES)),
   HASH_VAR_RECURSIVEDIRECTORIES(caseSensitiveHash(STRING_RECURSIVEDIRECTORIES)),

   HASH_PER_YEAR(caseSensitiveHash(STRING_YEAR)),
   HASH_PER_MONTH(caseSensitiveHash(STRING_MONTH)),
   HASH_PER_WEEK(caseSensitiveHash(STRING_WEEK)),
   HASH_PER_DAY(caseSensitiveHash(STRING_DAY)),
   HASH_PER_HOUR(caseSensitiveHash(STRING_HOUR)),
   HASH_PER_MINUTE(caseSensitiveHash(STRING_MINUTE)),
   HASH_PER_SECOND(caseSensitiveHash(STRING_SECOND)),

   HASH_PER_YEARS(caseSensitiveHash(STRING_YEARS)),
   HASH_PER_MONTHS(caseSensitiveHash(STRING_MONTHS)),
   HASH_PER_WEEKS(caseSensitiveHash(STRING_WEEKS)),
   HASH_PER_DAYS(caseSensitiveHash(STRING_DAYS)),
   HASH_PER_HOURS(caseSensitiveHash(STRING_HOURS)),
   HASH_PER_MINUTES(caseSensitiveHash(STRING_MINUTES)),
   HASH_PER_SECONDS(caseSensitiveHash(STRING_SECONDS)),

   HASH_PER_DATE(caseSensitiveHash(STRING_DATE)),
   HASH_PER_WEEKDAY(caseSensitiveHash(STRING_WEEKDAY)),

   HASH_FUNC_ISLOWER(caseSensitiveHash(STRING_ISLOWER)),
   HASH_FUNC_ISUPPER(caseSensitiveHash(STRING_ISUPPER)),
   HASH_FUNC_ISNUMBER(caseSensitiveHash(STRING_ISNUMBER)),
   HASH_FUNC_ISLETTER(caseSensitiveHash(STRING_ISLETTER)),
   HASH_FUNC_ISDIGIT(caseSensitiveHash(STRING_ISDIGIT)),
   HASH_FUNC_ISBINARY(caseSensitiveHash(STRING_ISBINARY)),
   HASH_FUNC_ISHEX(caseSensitiveHash(STRING_ISHEX)),

   HASH_FUNC_ANYINSIDE(caseSensitiveHash(STRING_ANYINSIDE)),
   HASH_FUNC_CONTAINS(caseSensitiveHash(STRING_CONTAINS)),
   HASH_FUNC_EXISTSINSIDE(caseSensitiveHash(STRING_EXISTSINSIDE)),
   HASH_FUNC_EXISTINSIDE(caseSensitiveHash(STRING_EXISTINSIDE)),
   HASH_FUNC_STARTSWITH(caseSensitiveHash(STRING_STARTSWITH)),
   HASH_FUNC_ENDSWITH(caseSensitiveHash(STRING_ENDSWITH)),
   HASH_FUNC_FINDTEXT(caseSensitiveHash(STRING_FINDTEXT)),

   HASH_FUNC_ABSOLUTE(caseSensitiveHash(STRING_ABSOLUTE)),
   HASH_FUNC_CEIL(caseSensitiveHash(STRING_CEIL)),
   HASH_FUNC_FLOOR(caseSensitiveHash(STRING_FLOOR)),
   HASH_FUNC_ROUND(caseSensitiveHash(STRING_ROUND)),
   HASH_FUNC_SIGN(caseSensitiveHash(STRING_SIGN)),
   HASH_FUNC_SQRT(caseSensitiveHash(STRING_SQRT)),
   HASH_FUNC_TRUNCATE(caseSensitiveHash(STRING_TRUNCATE)),

   HASH_FUNC_AVERAGE(caseSensitiveHash(STRING_AVERAGE)),
   HASH_FUNC_SUM(caseSensitiveHash(STRING_SUM)),
   HASH_FUNC_MIN(caseSensitiveHash(STRING_MIN)),
   HASH_FUNC_MAX(caseSensitiveHash(STRING_MAX)),
   HASH_FUNC_MEDIAN(caseSensitiveHash(STRING_MEDIAN)),

   HASH_FUNC_LENGTH(caseSensitiveHash(STRING_LENGTH)),
   HASH_FUNC_FROMBINARY(caseSensitiveHash(STRING_FROMBINARY)),
   HASH_FUNC_FROMHEX(caseSensitiveHash(STRING_FROMHEX)),
   HASH_FUNC_SIZE(caseSensitiveHash(STRING_SIZE)),
   HASH_FUNC_NUMBER(caseSensitiveHash(STRING_NUMBER)),
   HASH_FUNC_COUNTINSIDE(caseSensitiveHash(STRING_COUNTINSIDE)),
   HASH_FUNC_POWER(caseSensitiveHash(STRING_POWER)),
   
   HASH_FUNC_ANY(caseSensitiveHash(STRING_ANY)),
   HASH_FUNC_EXIST(caseSensitiveHash(STRING_EXIST)),
   HASH_FUNC_EXISTS(caseSensitiveHash(STRING_EXISTS)),
   HASH_FUNC_COUNT(caseSensitiveHash(STRING_COUNT)),

   HASH_FUNC_FIRST(caseSensitiveHash(STRING_FIRST)),
   HASH_FUNC_LAST(caseSensitiveHash(STRING_LAST)),
   HASH_FUNC_RANDOM(caseSensitiveHash(STRING_RANDOM)),
   HASH_FUNC_LIFETIME(caseSensitiveHash(STRING_LIFETIME)),

   HASH_FUNC_AFTER(caseSensitiveHash(STRING_AFTER)),
   HASH_FUNC_BEFORE(caseSensitiveHash(STRING_BEFORE)),
   HASH_FUNC_REVERSED(caseSensitiveHash(STRING_REVERSED)),

   HASH_FUNC_DIGITS(caseSensitiveHash(STRING_DIGITS)),
   HASH_FUNC_LETTERS(caseSensitiveHash(STRING_LETTERS)),
   HASH_FUNC_LOWER(caseSensitiveHash(STRING_LOWER)),
   HASH_FUNC_TRIM(caseSensitiveHash(STRING_TRIM)),
   HASH_FUNC_UPPER(caseSensitiveHash(STRING_UPPER)),
   HASH_FUNC_REVERSE(caseSensitiveHash(STRING_REVERSE)),
   HASH_FUNC_AFTERDIGITS(caseSensitiveHash(STRING_AFTERDIGITS)),
   HASH_FUNC_AFTERLETTERS(caseSensitiveHash(STRING_AFTERLETTERS)),
   HASH_FUNC_BEFOREDIGITS(caseSensitiveHash(STRING_BEFOREDIGITS)),
   HASH_FUNC_BEFORELETTERS(caseSensitiveHash(STRING_BEFORELETTERS)),
   HASH_FUNC_CAPITALIZE(caseSensitiveHash(STRING_CAPITALIZE)),

   HASH_FUNC_REPLACE(caseSensitiveHash(STRING_REPLACE)),
   HASH_FUNC_SUBSTRING(caseSensitiveHash(STRING_SUBSTRING)),
   HASH_FUNC_CONCATENATE(caseSensitiveHash(STRING_CONCATENATE)),
   HASH_FUNC_PATH(caseSensitiveHash(STRING_PATH)),
   HASH_FUNC_STRING(caseSensitiveHash(STRING_STRING)),
   HASH_FUNC_MONTHNAME(caseSensitiveHash(STRING_MONTHNAME)),
   HASH_FUNC_WEEKDAYNAME(caseSensitiveHash(STRING_WEEKDAYNAME)),
   HASH_FUNC_JOIN(caseSensitiveHash(STRING_JOIN)),

   HASH_FUNC_REPEAT(caseSensitiveHash(STRING_REPEAT)),
   HASH_FUNC_LEFT(caseSensitiveHash(STRING_LEFT)),
   HASH_FUNC_RIGHT(caseSensitiveHash(STRING_RIGHT)),
   HASH_FUNC_FILL(caseSensitiveHash(STRING_FILL)),

   HASH_FUNC_ROMAN(caseSensitiveHash(STRING_ROMAN)),
   HASH_FUNC_BINARY(caseSensitiveHash(STRING_BINARY)),
   HASH_FUNC_HEX(caseSensitiveHash(STRING_HEX)),

   HASH_FUNC_CHRISTMAS(caseSensitiveHash(STRING_CHRISTMAS)),
   HASH_FUNC_EASTER(caseSensitiveHash(STRING_EASTER)),
   HASH_FUNC_NEWYEAR(caseSensitiveHash(STRING_NEWYEAR)),

   HASH_FUNC_DATE(caseSensitiveHash(STRING_DATE)),
   HASH_FUNC_TIME(caseSensitiveHash(STRING_TIME)),
   HASH_FUNC_CHARACTERS(caseSensitiveHash(STRING_CHARACTERS)),
   HASH_FUNC_WORDS(caseSensitiveHash(STRING_WORDS)),
   HASH_FUNC_SPLIT(caseSensitiveHash(STRING_SPLIT)),
   HASH_FUNC_NUMBERS(caseSensitiveHash(STRING_NUMBERS)),

   HASH_KW_COPY(caseSensitiveHash(STRING_COPY)),
   HASH_KW_CREATE(caseSensitiveHash(STRING_CREATE)),
   HASH_KW_CREATEFILE(caseSensitiveHash(STRING_CREATEFILE)),
   HASH_KW_CREATEDIRECTORY(caseSensitiveHash(STRING_CREATEDIRECTORY)),
   HASH_KW_CREATEFILES(caseSensitiveHash(STRING_CREATEFILES)),
   HASH_KW_CREATEDIRECTORIES(caseSensitiveHash(STRING_CREATEDIRECTORIES)),
   HASH_KW_DELETE(caseSensitiveHash(STRING_DELETE)),
   HASH_KW_DROP(caseSensitiveHash(STRING_DROP)),
   HASH_KW_HIDE(caseSensitiveHash(STRING_HIDE)),
   HASH_KW_LOCK(caseSensitiveHash(STRING_LOCK)),
   HASH_KW_MOVE(caseSensitiveHash(STRING_MOVE)),
   HASH_KW_OPEN(caseSensitiveHash(STRING_OPEN)),
   HASH_KW_REACCESS(caseSensitiveHash(STRING_REACCESS)),
   HASH_KW_RECREATE(caseSensitiveHash(STRING_RECREATE)),
   HASH_KW_RECHANGE(caseSensitiveHash(STRING_RECHANGE)),
   HASH_KW_REMODIFY(caseSensitiveHash(STRING_REMODIFY)),
   HASH_KW_RENAME(caseSensitiveHash(STRING_RENAME)),
   HASH_KW_SELECT(caseSensitiveHash(STRING_SELECT)),
   HASH_KW_UNHIDE(caseSensitiveHash(STRING_UNHIDE)),
   HASH_KW_UNLOCK(caseSensitiveHash(STRING_UNLOCK)),
   HASH_KW_FORCE(caseSensitiveHash(STRING_FORCE)),
   HASH_KW_STACK(caseSensitiveHash(STRING_STACK)),
   HASH_KW_TRUE(caseSensitiveHash(STRING_TRUE)),
   HASH_KW_FALSE(caseSensitiveHash(STRING_FALSE)),
   HASH_KW_AND(caseSensitiveHash(STRING_AND)),
   HASH_KW_OR(caseSensitiveHash(STRING_OR)),
   HASH_KW_XOR(caseSensitiveHash(STRING_XOR)),
   HASH_KW_NOT(caseSensitiveHash(STRING_NOT)),
   HASH_KW_PRINT(caseSensitiveHash(STRING_PRINT)),
   HASH_KW_RUN(caseSensitiveHash(STRING_RUN)),
   HASH_KW_SLEEP(caseSensitiveHash(STRING_SLEEP)),
   HASH_KW_IN(caseSensitiveHash(STRING_IN)),
   HASH_KW_LIKE(caseSensitiveHash(STRING_LIKE)),
   HASH_KW_ELSE(caseSensitiveHash(STRING_ELSE)),
   HASH_KW_IF(caseSensitiveHash(STRING_IF)),
   HASH_KW_INSIDE(caseSensitiveHash(STRING_INSIDE)),
   HASH_KW_TIMES(caseSensitiveHash(STRING_TIMES)),
   HASH_KW_WHILE(caseSensitiveHash(STRING_WHILE)),
   HASH_KW_EVERY(caseSensitiveHash(STRING_EVERY)),
   HASH_KW_FINAL(caseSensitiveHash(STRING_FINAL)),
   HASH_KW_LIMIT(caseSensitiveHash(STRING_LIMIT)),
   HASH_KW_ORDER(caseSensitiveHash(STRING_ORDER)),
   HASH_KW_SKIP(caseSensitiveHash(STRING_SKIP)),
   HASH_KW_WHERE(caseSensitiveHash(STRING_WHERE)),
   HASH_KW_AS(caseSensitiveHash(STRING_AS)),
   HASH_KW_BY(caseSensitiveHash(STRING_BY)),
   HASH_KW_TO(caseSensitiveHash(STRING_TO)),
   HASH_KW_EXTENSIONLESS(caseSensitiveHash(STRING_EXTENSIONLESS)),
   HASH_KW_WITH(caseSensitiveHash(STRING_WITH)),
   HASH_KW_ASC(caseSensitiveHash(STRING_ASC)),
   HASH_KW_DESC(caseSensitiveHash(STRING_DESC)),
   HASH_KW_BREAK(caseSensitiveHash(STRING_BREAK)),
   HASH_KW_CONTINUE(caseSensitiveHash(STRING_CONTINUE)),
   HASH_KW_EXIT(caseSensitiveHash(STRING_EXIT)),
   HASH_KW_ERROR(caseSensitiveHash(STRING_ERROR)),

   HASH_GROUP_MONTHS({
     this->HASH_VAR_JANUARY, this->HASH_VAR_FEBRUARY, this->HASH_VAR_MARCH,
     this->HASH_VAR_APRIL, this->HASH_VAR_MAY, this->HASH_VAR_JUNE,
     this->HASH_VAR_JULY, this->HASH_VAR_AUGUST, this->HASH_VAR_SEPTEMBER,
     this->HASH_VAR_OCTOBER, this->HASH_VAR_NOVEMBER, this->HASH_VAR_DECEMBER
   }),

   HASH_GROUP_WEEKDAYS({
     this->HASH_VAR_MONDAY, this->HASH_VAR_TUESDAY, this->HASH_VAR_WEDNESDAY,
     this->HASH_VAR_THURSDAY, this->HASH_VAR_FRIDAY,
     this->HASH_VAR_SATURDAY, this->HASH_VAR_SUNDAY
   }),

   HASH_GROUP_PERIOD_SINGLE({
     this->HASH_PER_YEAR, this->HASH_PER_MONTH, this->HASH_PER_WEEK, this->HASH_PER_DAY,
     this->HASH_PER_HOUR, this->HASH_PER_MINUTE, this->HASH_PER_SECOND
   }),

   HASH_GROUP_PERIOD_MULTI({
     this->HASH_PER_YEARS, this->HASH_PER_MONTHS, this->HASH_PER_WEEKS, this->HASH_PER_DAYS,
     this->HASH_PER_HOURS, this->HASH_PER_MINUTES, this->HASH_PER_SECONDS
   }),

   HASH_GROUP_ATTR({
     this->HASH_VAR_ACCESS, this->HASH_VAR_ARCHIVE, this->HASH_VAR_CHANGE, this->HASH_VAR_COMPRESSED,
     this->HASH_VAR_CREATION, this->HASH_VAR_DEPTH, this->HASH_VAR_DRIVE, this->HASH_VAR_EMPTY, this->HASH_VAR_EXISTS,
     this->HASH_VAR_ENCRYPTED, this->HASH_VAR_EXTENSION, this->HASH_VAR_FULLNAME,
     this->HASH_VAR_HIDDEN, this->HASH_VAR_ISDIRECTORY, this->HASH_VAR_ISFILE, this->HASH_VAR_LIFETIME,
     this->HASH_VAR_MODIFICATION, this->HASH_VAR_NAME, this->HASH_VAR_PARENT, this->HASH_VAR_PATH,
     this->HASH_VAR_READONLY, this->HASH_VAR_SIZE
   }),

   HASH_GROUP_TIME_ATTR({
     this->HASH_VAR_ACCESS, this->HASH_VAR_CHANGE, this->HASH_VAR_CREATION, this->HASH_VAR_MODIFICATION
   }),

   HASH_GROUP_TIME_VAR({
     this->HASH_VAR_ACCESS, this->HASH_VAR_CHANGE, this->HASH_VAR_CREATION, this->HASH_VAR_MODIFICATION,
     this->HASH_VAR_NOW, this->HASH_VAR_TODAY
   }),

   // attributes, that can be altered by external circumstances
   // for example, last modification time of an element may be changed by another process
   // but its parent directory stays the same
   HASH_GROUP_ALTERABLE_ATTR({
     this->HASH_VAR_ACCESS, this->HASH_VAR_ARCHIVE, this->HASH_VAR_CHANGE, this->HASH_VAR_COMPRESSED,
     this->HASH_VAR_CREATION, this->HASH_VAR_EMPTY, this->HASH_VAR_EXISTS, this->HASH_VAR_ENCRYPTED,
     this->HASH_VAR_HIDDEN, this->HASH_VAR_ISDIRECTORY, this->HASH_VAR_ISFILE, this->HASH_VAR_LIFETIME,
     this->HASH_VAR_MODIFICATION, this->HASH_VAR_READONLY, this->HASH_VAR_SIZE
   }),

   HASH_GROUP_VARS_IMMUTABLES({
      this->HASH_VAR_THIS, this->HASH_VAR_ACCESS, this->HASH_VAR_ARCHIVE, this->HASH_VAR_CHANGE,
      this->HASH_VAR_COMPRESSED, this->HASH_VAR_CREATION, this->HASH_VAR_DEPTH, this->HASH_VAR_DRIVE,
      this->HASH_VAR_EMPTY, this->HASH_VAR_ENCRYPTED, this->HASH_VAR_EXISTS, this->HASH_VAR_EXTENSION,
      this->HASH_VAR_FULLNAME, this->HASH_VAR_HIDDEN, this->HASH_VAR_INDEX, this->HASH_VAR_ISDIRECTORY,
      this->HASH_VAR_ISFILE, this->HASH_VAR_LIFETIME, this->HASH_VAR_LOCATION, this->HASH_VAR_MODIFICATION,
      this->HASH_VAR_NAME, this->HASH_VAR_PARENT, this->HASH_VAR_PATH, this->HASH_VAR_READONLY, 
      this->HASH_VAR_SIZE, this->HASH_VAR_SUCCESS, this->HASH_VAR_NOW, this->HASH_VAR_TODAY,
      this->HASH_VAR_YESTERDAY, this->HASH_VAR_TOMORROW, this->HASH_VAR_JANUARY, this->HASH_VAR_FEBRUARY,
      this->HASH_VAR_MARCH, this->HASH_VAR_APRIL, this->HASH_VAR_MAY, this->HASH_VAR_JUNE, this->HASH_VAR_JULY,
      this->HASH_VAR_AUGUST, this->HASH_VAR_SEPTEMBER, this->HASH_VAR_OCTOBER, this->HASH_VAR_NOVEMBER,
      this->HASH_VAR_DECEMBER, this->HASH_VAR_MONDAY, this->HASH_VAR_TUESDAY, this->HASH_VAR_WEDNESDAY,
      this->HASH_VAR_THURSDAY, this->HASH_VAR_FRIDAY, this->HASH_VAR_SATURDAY, this->HASH_VAR_SUNDAY,
      this->HASH_VAR_ALPHABET, this->HASH_VAR_ASCII, this->HASH_VAR_ARGUMENTS, this->HASH_VAR_DESKTOP,
      this->HASH_VAR_PERUN2, this->HASH_VAR_ORIGIN, this->HASH_VAR_DIRECTORIES,
      this->HASH_VAR_FILES, this->HASH_VAR_RECURSIVEFILES, this->HASH_VAR_RECURSIVEDIRECTORIES
   }),

   HASH_GROUP_FUNC_BOO_STR({
     this->HASH_FUNC_ISLOWER, this->HASH_FUNC_ISUPPER, this->HASH_FUNC_ISNUMBER,
     this->HASH_FUNC_ISLETTER, this->HASH_FUNC_ISDIGIT, this->HASH_FUNC_ISBINARY, this->HASH_FUNC_ISHEX
   }),

   HASH_GROUP_FUNC_NUM_NUM({
     this->HASH_FUNC_ABSOLUTE, this->HASH_FUNC_CEIL, this->HASH_FUNC_FLOOR,
     this->HASH_FUNC_ROUND, this->HASH_FUNC_SIGN, this->HASH_FUNC_SQRT, this->HASH_FUNC_TRUNCATE
   }),

   HASH_GROUP_AGGRFUNC({
     this->HASH_FUNC_AVERAGE, this->HASH_FUNC_SUM, this->HASH_FUNC_MIN, this->HASH_FUNC_MAX, this->HASH_FUNC_MEDIAN
   }),

   HASH_GROUP_FUNC_STR_STR({
     this->HASH_FUNC_DIGITS,  this->HASH_FUNC_LETTERS, this->HASH_FUNC_LOWER, this->HASH_FUNC_TRIM,
     this->HASH_FUNC_UPPER, this->HASH_FUNC_REVERSE, this->HASH_FUNC_AFTERDIGITS, this->HASH_FUNC_AFTERLETTERS,
     this->HASH_FUNC_BEFOREDIGITS, this->HASH_FUNC_BEFORELETTERS, this->HASH_FUNC_CAPITALIZE, this->HASH_VAR_PARENT
   }),

   HASH_GROUP_FUNC_STR_STR_NUM({
     this->HASH_FUNC_REPEAT, this->HASH_FUNC_LEFT, this->HASH_FUNC_RIGHT, this->HASH_FUNC_FILL
   }),

   HASH_GROUP_FUNC_STR_NUM({
     this->HASH_FUNC_ROMAN, this->HASH_FUNC_BINARY, this->HASH_FUNC_HEX
   }),

   HASH_GROUP_FUNC_TIM_NUM({
     this->HASH_FUNC_CHRISTMAS, this->HASH_FUNC_EASTER, this->HASH_FUNC_NEWYEAR
   }),

   HASH_MAP_MONTHS({
      { this->HASH_VAR_JANUARY,    TNUM_JANUARY },
      { this->HASH_VAR_FEBRUARY,   TNUM_FEBRUARY },
      { this->HASH_VAR_MARCH,      TNUM_MARCH },
      { this->HASH_VAR_APRIL,      TNUM_APRIL },
      { this->HASH_VAR_MAY,        TNUM_MAY },
      { this->HASH_VAR_JUNE,       TNUM_JUNE },
      { this->HASH_VAR_JULY,       TNUM_JULY },
      { this->HASH_VAR_AUGUST,     TNUM_AUGUST },
      { this->HASH_VAR_SEPTEMBER,  TNUM_SEPTEMBER },
      { this->HASH_VAR_OCTOBER,    TNUM_OCTOBER },
      { this->HASH_VAR_NOVEMBER,   TNUM_NOVEMBER },
      { this->HASH_VAR_DECEMBER,   TNUM_DECEMBER }
   }),

   HASH_MAP_WEEKDAYS({
      { this->HASH_VAR_MONDAY,     TNUM_MONDAY },
      { this->HASH_VAR_TUESDAY,    TNUM_TUESDAY },
      { this->HASH_VAR_WEDNESDAY,  TNUM_WEDNESDAY },
      { this->HASH_VAR_THURSDAY,   TNUM_THURSDAY },
      { this->HASH_VAR_FRIDAY,     TNUM_FRIDAY },
      { this->HASH_VAR_SATURDAY,   TNUM_SATURDAY },
      { this->HASH_VAR_SUNDAY,     TNUM_SUNDAY }
   }),

   HASH_MAP_PERIOD_UNITS({
      { this->HASH_PER_YEAR,       Period::u_Years },
      { this->HASH_PER_YEARS,      Period::u_Years },
      { this->HASH_PER_MONTH,      Period::u_Months },
      { this->HASH_PER_MONTHS,     Period::u_Months },
      { this->HASH_PER_WEEK,       Period::u_Weeks },
      { this->HASH_PER_WEEKS,      Period::u_Weeks },
      { this->HASH_PER_DAY,        Period::u_Days },
      { this->HASH_PER_DAYS,       Period::u_Days },
      { this->HASH_PER_HOUR,       Period::u_Hours },
      { this->HASH_PER_HOURS,      Period::u_Hours },
      { this->HASH_PER_MINUTE,     Period::u_Minutes },
      { this->HASH_PER_MINUTES,    Period::u_Minutes },
      { this->HASH_PER_SECOND,     Period::u_Seconds },
      { this->HASH_PER_SECONDS,    Period::u_Seconds }
   }) { };

}
