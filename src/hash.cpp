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
   NOTHING_HASH(caseSensitiveHash(_str())),
   HASH_THIS(caseSensitiveHash(STRING_THIS)),

   HASH_ACCESS(caseSensitiveHash(STRING_ACCESS)),
   HASH_ARCHIVE(caseSensitiveHash(STRING_ARCHIVE)),
   HASH_CHANGE(caseSensitiveHash(STRING_CHANGE)),
   HASH_COMPRESSED(caseSensitiveHash(STRING_COMPRESSED)),
   HASH_CREATION(caseSensitiveHash(STRING_CREATION)),
   HASH_DEPTH(caseSensitiveHash(STRING_DEPTH)),
   HASH_DRIVE(caseSensitiveHash(STRING_DRIVE)),
   HASH_EMPTY(caseSensitiveHash(STRING_EMPTY)),
   HASH_ENCRYPTED(caseSensitiveHash(STRING_ENCRYPTED)),
   HASH_EXISTS(caseSensitiveHash(STRING_EXISTS)),
   HASH_EXTENSION(caseSensitiveHash(STRING_EXTENSION)),
   HASH_FULLNAME(caseSensitiveHash(STRING_FULLNAME)),
   HASH_HIDDEN(caseSensitiveHash(STRING_HIDDEN)),
   HASH_INDEX(caseSensitiveHash(STRING_INDEX)),
   HASH_ISDIRECTORY(caseSensitiveHash(STRING_ISDIRECTORY)),
   HASH_ISFILE(caseSensitiveHash(STRING_ISFILE)),
   HASH_LIFETIME(caseSensitiveHash(STRING_LIFETIME)),
   HASH_LOCATION(caseSensitiveHash(STRING_LOCATION)),
   HASH_MODIFICATION(caseSensitiveHash(STRING_MODIFICATION)),
   HASH_NAME(caseSensitiveHash(STRING_NAME)),
   HASH_PARENT(caseSensitiveHash(STRING_PARENT)),
   HASH_PATH(caseSensitiveHash(STRING_PATH)),
   HASH_READONLY(caseSensitiveHash(STRING_READONLY)),
   HASH_SIZE(caseSensitiveHash(STRING_SIZE)),
   HASH_SUCCESS(caseSensitiveHash(STRING_SUCCESS)),

   HASH_NOW(caseSensitiveHash(STRING_NOW)),
   HASH_TODAY(caseSensitiveHash(STRING_TODAY)),
   HASH_YESTERDAY(caseSensitiveHash(STRING_YESTERDAY)),
   HASH_TOMORROW(caseSensitiveHash(STRING_TOMORROW)),

   HASH_JANUARY(caseSensitiveHash(STRING_JANUARY)),
   HASH_FEBRUARY(caseSensitiveHash(STRING_FEBRUARY)),
   HASH_MARCH(caseSensitiveHash(STRING_MARCH)),
   HASH_APRIL(caseSensitiveHash(STRING_APRIL)),
   HASH_MAY(caseSensitiveHash(STRING_MAY)),
   HASH_JUNE(caseSensitiveHash(STRING_JUNE)),
   HASH_JULY(caseSensitiveHash(STRING_JULY)),
   HASH_AUGUST(caseSensitiveHash(STRING_AUGUST)),
   HASH_SEPTEMBER(caseSensitiveHash(STRING_SEPTEMBER)),
   HASH_OCTOBER(caseSensitiveHash(STRING_OCTOBER)),
   HASH_NOVEMBER(caseSensitiveHash(STRING_NOVEMBER)),
   HASH_DECEMBER(caseSensitiveHash(STRING_DECEMBER)),

   HASH_MONDAY(caseSensitiveHash(STRING_MONDAY)),
   HASH_TUESDAY(caseSensitiveHash(STRING_TUESDAY)),
   HASH_WEDNESDAY(caseSensitiveHash(STRING_WEDNESDAY)),
   HASH_THURSDAY(caseSensitiveHash(STRING_THURSDAY)),
   HASH_FRIDAY(caseSensitiveHash(STRING_FRIDAY)),
   HASH_SATURDAY(caseSensitiveHash(STRING_SATURDAY)),
   HASH_SUNDAY(caseSensitiveHash(STRING_SUNDAY)),

   HASH_ALPHABET(caseSensitiveHash(STRING_ALPHABET)),
   HASH_ASCII(caseSensitiveHash(STRING_ASCII)),
   HASH_ARGUMENTS(caseSensitiveHash(STRING_ARGUMENTS)),

   HASH_DESKTOP(caseSensitiveHash(STRING_DESKTOP)),
   HASH_PERUN2(caseSensitiveHash(STRING_PERUN2)),
   HASH_ORIGIN(caseSensitiveHash(STRING_ORIGIN)),

   HASH_DIRECTORIES(caseSensitiveHash(STRING_DIRECTORIES)),
   HASH_FILES(caseSensitiveHash(STRING_FILES)),
   HASH_RECURSIVEFILES(caseSensitiveHash(STRING_RECURSIVEFILES)),
   HASH_RECURSIVEDIRECTORIES(caseSensitiveHash(STRING_RECURSIVEDIRECTORIES)),

   HASH_YEAR(caseSensitiveHash(STRING_YEAR)),
   HASH_MONTH(caseSensitiveHash(STRING_MONTH)),
   HASH_WEEK(caseSensitiveHash(STRING_WEEK)),
   HASH_DAY(caseSensitiveHash(STRING_DAY)),
   HASH_HOUR(caseSensitiveHash(STRING_HOUR)),
   HASH_MINUTE(caseSensitiveHash(STRING_MINUTE)),
   HASH_SECOND(caseSensitiveHash(STRING_SECOND)),

   HASH_YEARS(caseSensitiveHash(STRING_YEARS)),
   HASH_MONTHS(caseSensitiveHash(STRING_MONTHS)),
   HASH_WEEKS(caseSensitiveHash(STRING_WEEKS)),
   HASH_DAYS(caseSensitiveHash(STRING_DAYS)),
   HASH_HOURS(caseSensitiveHash(STRING_HOURS)),
   HASH_MINUTES(caseSensitiveHash(STRING_MINUTES)),
   HASH_SECONDS(caseSensitiveHash(STRING_SECONDS)),

   HASH_DATE(caseSensitiveHash(STRING_DATE)),
   HASH_WEEKDAY(caseSensitiveHash(STRING_WEEKDAY)),

   HASH_ISLOWER(caseSensitiveHash(STRING_ISLOWER)),
   HASH_ISUPPER(caseSensitiveHash(STRING_ISUPPER)),
   HASH_ISNUMBER(caseSensitiveHash(STRING_ISNUMBER)),
   HASH_ISLETTER(caseSensitiveHash(STRING_ISLETTER)),
   HASH_ISDIGIT(caseSensitiveHash(STRING_ISDIGIT)),
   HASH_ISBINARY(caseSensitiveHash(STRING_ISBINARY)),
   HASH_ISHEX(caseSensitiveHash(STRING_ISHEX)),

   HASH_ANYINSIDE(caseSensitiveHash(STRING_ANYINSIDE)),
   HASH_CONTAINS(caseSensitiveHash(STRING_CONTAINS)),
   HASH_EXISTSINSIDE(caseSensitiveHash(STRING_EXISTSINSIDE)),
   HASH_EXISTINSIDE(caseSensitiveHash(STRING_EXISTINSIDE)),
   HASH_STARTSWITH(caseSensitiveHash(STRING_STARTSWITH)),
   HASH_ENDSWITH(caseSensitiveHash(STRING_ENDSWITH)),
   HASH_FINDTEXT(caseSensitiveHash(STRING_FINDTEXT)),

   HASH_ABSOLUTE(caseSensitiveHash(STRING_ABSOLUTE)),
   HASH_CEIL(caseSensitiveHash(STRING_CEIL)),
   HASH_FLOOR(caseSensitiveHash(STRING_FLOOR)),
   HASH_ROUND(caseSensitiveHash(STRING_ROUND)),
   HASH_SIGN(caseSensitiveHash(STRING_SIGN)),
   HASH_SQRT(caseSensitiveHash(STRING_SQRT)),
   HASH_TRUNCATE(caseSensitiveHash(STRING_TRUNCATE)),

   HASH_AVERAGE(caseSensitiveHash(STRING_AVERAGE)),
   HASH_SUM(caseSensitiveHash(STRING_SUM)),
   HASH_MIN(caseSensitiveHash(STRING_MIN)),
   HASH_MAX(caseSensitiveHash(STRING_MAX)),
   HASH_MEDIAN(caseSensitiveHash(STRING_MEDIAN)),

   HASH_LENGTH(caseSensitiveHash(STRING_LENGTH)),
   HASH_FROMBINARY(caseSensitiveHash(STRING_FROMBINARY)),
   HASH_FROMHEX(caseSensitiveHash(STRING_FROMHEX)),
   HASH_NUMBER(caseSensitiveHash(STRING_NUMBER)),
   HASH_COUNTINSIDE(caseSensitiveHash(STRING_COUNTINSIDE)),
   HASH_POWER(caseSensitiveHash(STRING_POWER)),
   
   HASH_ANY(caseSensitiveHash(STRING_ANY)),
   HASH_EXIST(caseSensitiveHash(STRING_EXIST)),
   HASH_COUNT(caseSensitiveHash(STRING_COUNT)),

   HASH_FIRST(caseSensitiveHash(STRING_FIRST)),
   HASH_LAST(caseSensitiveHash(STRING_LAST)),
   HASH_RANDOM(caseSensitiveHash(STRING_RANDOM)),

   HASH_AFTER(caseSensitiveHash(STRING_AFTER)),
   HASH_BEFORE(caseSensitiveHash(STRING_BEFORE)),
   HASH_REVERSED(caseSensitiveHash(STRING_REVERSED)),

   HASH_DIGITS(caseSensitiveHash(STRING_DIGITS)),
   HASH_LETTERS(caseSensitiveHash(STRING_LETTERS)),
   HASH_LOWER(caseSensitiveHash(STRING_LOWER)),
   HASH_TRIM(caseSensitiveHash(STRING_TRIM)),
   HASH_UPPER(caseSensitiveHash(STRING_UPPER)),
   HASH_REVERSE(caseSensitiveHash(STRING_REVERSE)),
   HASH_AFTERDIGITS(caseSensitiveHash(STRING_AFTERDIGITS)),
   HASH_AFTERLETTERS(caseSensitiveHash(STRING_AFTERLETTERS)),
   HASH_BEFOREDIGITS(caseSensitiveHash(STRING_BEFOREDIGITS)),
   HASH_BEFORELETTERS(caseSensitiveHash(STRING_BEFORELETTERS)),
   HASH_CAPITALIZE(caseSensitiveHash(STRING_CAPITALIZE)),

   HASH_REPLACE(caseSensitiveHash(STRING_REPLACE)),
   HASH_SUBSTRING(caseSensitiveHash(STRING_SUBSTRING)),
   HASH_CONCATENATE(caseSensitiveHash(STRING_CONCATENATE)),
   HASH_STRING(caseSensitiveHash(STRING_STRING)),
   HASH_MONTHNAME(caseSensitiveHash(STRING_MONTHNAME)),
   HASH_WEEKDAYNAME(caseSensitiveHash(STRING_WEEKDAYNAME)),
   HASH_JOIN(caseSensitiveHash(STRING_JOIN)),

   HASH_REPEAT(caseSensitiveHash(STRING_REPEAT)),
   HASH_LEFT(caseSensitiveHash(STRING_LEFT)),
   HASH_RIGHT(caseSensitiveHash(STRING_RIGHT)),
   HASH_FILL(caseSensitiveHash(STRING_FILL)),

   HASH_ROMAN(caseSensitiveHash(STRING_ROMAN)),
   HASH_BINARY(caseSensitiveHash(STRING_BINARY)),
   HASH_HEX(caseSensitiveHash(STRING_HEX)),

   HASH_CHRISTMAS(caseSensitiveHash(STRING_CHRISTMAS)),
   HASH_EASTER(caseSensitiveHash(STRING_EASTER)),
   HASH_NEWYEAR(caseSensitiveHash(STRING_NEWYEAR)),

   HASH_TIME(caseSensitiveHash(STRING_TIME)),
   HASH_CHARACTERS(caseSensitiveHash(STRING_CHARACTERS)),
   HASH_WORDS(caseSensitiveHash(STRING_WORDS)),
   HASH_SPLIT(caseSensitiveHash(STRING_SPLIT)),
   HASH_NUMBERS(caseSensitiveHash(STRING_NUMBERS)),

   HASH_COPY(caseSensitiveHash(STRING_COPY)),
   HASH_CREATE(caseSensitiveHash(STRING_CREATE)),
   HASH_CREATEFILE(caseSensitiveHash(STRING_CREATEFILE)),
   HASH_CREATEDIRECTORY(caseSensitiveHash(STRING_CREATEDIRECTORY)),
   HASH_CREATEFILES(caseSensitiveHash(STRING_CREATEFILES)),
   HASH_CREATEDIRECTORIES(caseSensitiveHash(STRING_CREATEDIRECTORIES)),
   HASH_DELETE(caseSensitiveHash(STRING_DELETE)),
   HASH_DROP(caseSensitiveHash(STRING_DROP)),
   HASH_HIDE(caseSensitiveHash(STRING_HIDE)),
   HASH_LOCK(caseSensitiveHash(STRING_LOCK)),
   HASH_MOVE(caseSensitiveHash(STRING_MOVE)),
   HASH_OPEN(caseSensitiveHash(STRING_OPEN)),
   HASH_REACCESS(caseSensitiveHash(STRING_REACCESS)),
   HASH_RECREATE(caseSensitiveHash(STRING_RECREATE)),
   HASH_RECHANGE(caseSensitiveHash(STRING_RECHANGE)),
   HASH_REMODIFY(caseSensitiveHash(STRING_REMODIFY)),
   HASH_RENAME(caseSensitiveHash(STRING_RENAME)),
   HASH_SELECT(caseSensitiveHash(STRING_SELECT)),
   HASH_UNHIDE(caseSensitiveHash(STRING_UNHIDE)),
   HASH_UNLOCK(caseSensitiveHash(STRING_UNLOCK)),
   HASH_FORCE(caseSensitiveHash(STRING_FORCE)),
   HASH_STACK(caseSensitiveHash(STRING_STACK)),
   HASH_TRUE(caseSensitiveHash(STRING_TRUE)),
   HASH_FALSE(caseSensitiveHash(STRING_FALSE)),
   HASH_AND(caseSensitiveHash(STRING_AND)),
   HASH_OR(caseSensitiveHash(STRING_OR)),
   HASH_XOR(caseSensitiveHash(STRING_XOR)),
   HASH_NOT(caseSensitiveHash(STRING_NOT)),
   HASH_PRINT(caseSensitiveHash(STRING_PRINT)),
   HASH_RUN(caseSensitiveHash(STRING_RUN)),
   HASH_SLEEP(caseSensitiveHash(STRING_SLEEP)),
   HASH_IN(caseSensitiveHash(STRING_IN)),
   HASH_LIKE(caseSensitiveHash(STRING_LIKE)),
   HASH_ELSE(caseSensitiveHash(STRING_ELSE)),
   HASH_IF(caseSensitiveHash(STRING_IF)),
   HASH_INSIDE(caseSensitiveHash(STRING_INSIDE)),
   HASH_TIMES(caseSensitiveHash(STRING_TIMES)),
   HASH_WHILE(caseSensitiveHash(STRING_WHILE)),
   HASH_EVERY(caseSensitiveHash(STRING_EVERY)),
   HASH_FINAL(caseSensitiveHash(STRING_FINAL)),
   HASH_LIMIT(caseSensitiveHash(STRING_LIMIT)),
   HASH_ORDER(caseSensitiveHash(STRING_ORDER)),
   HASH_SKIP(caseSensitiveHash(STRING_SKIP)),
   HASH_WHERE(caseSensitiveHash(STRING_WHERE)),
   HASH_AS(caseSensitiveHash(STRING_AS)),
   HASH_BY(caseSensitiveHash(STRING_BY)),
   HASH_TO(caseSensitiveHash(STRING_TO)),
   HASH_EXTENSIONLESS(caseSensitiveHash(STRING_EXTENSIONLESS)),
   HASH_WITH(caseSensitiveHash(STRING_WITH)),
   HASH_ASC(caseSensitiveHash(STRING_ASC)),
   HASH_DESC(caseSensitiveHash(STRING_DESC)),
   HASH_BREAK(caseSensitiveHash(STRING_BREAK)),
   HASH_CONTINUE(caseSensitiveHash(STRING_CONTINUE)),
   HASH_EXIT(caseSensitiveHash(STRING_EXIT)),
   HASH_ERROR(caseSensitiveHash(STRING_ERROR)),

   HASH_GROUP_MONTHS({
     this->HASH_JANUARY, this->HASH_FEBRUARY, this->HASH_MARCH,
     this->HASH_APRIL, this->HASH_MAY, this->HASH_JUNE,
     this->HASH_JULY, this->HASH_AUGUST, this->HASH_SEPTEMBER,
     this->HASH_OCTOBER, this->HASH_NOVEMBER, this->HASH_DECEMBER
   }),

   HASH_GROUP_WEEKDAYS({
     this->HASH_MONDAY, this->HASH_TUESDAY, this->HASH_WEDNESDAY,
     this->HASH_THURSDAY, this->HASH_FRIDAY,
     this->HASH_SATURDAY, this->HASH_SUNDAY
   }),

   HASH_GROUP_PERIOD_SINGLE({
     this->HASH_YEAR, this->HASH_MONTH, this->HASH_WEEK, this->HASH_DAY,
     this->HASH_HOUR, this->HASH_MINUTE, this->HASH_SECOND
   }),

   HASH_GROUP_PERIOD_MULTI({
     this->HASH_YEARS, this->HASH_MONTHS, this->HASH_WEEKS, this->HASH_DAYS,
     this->HASH_HOURS, this->HASH_MINUTES, this->HASH_SECONDS
   }),

   HASH_GROUP_ATTR({
     this->HASH_ACCESS, this->HASH_ARCHIVE, this->HASH_CHANGE, this->HASH_COMPRESSED,
     this->HASH_CREATION, this->HASH_DEPTH, this->HASH_DRIVE, this->HASH_EMPTY, this->HASH_EXISTS,
     this->HASH_ENCRYPTED, this->HASH_EXTENSION, this->HASH_FULLNAME,
     this->HASH_HIDDEN, this->HASH_ISDIRECTORY, this->HASH_ISFILE, this->HASH_LIFETIME,
     this->HASH_MODIFICATION, this->HASH_NAME, this->HASH_PARENT, this->HASH_PATH,
     this->HASH_READONLY, this->HASH_SIZE
   }),

   HASH_GROUP_TIME_ATTR({
     this->HASH_ACCESS, this->HASH_CHANGE, this->HASH_CREATION, this->HASH_MODIFICATION
   }),

   HASH_GROUP_TIME_VAR({
     this->HASH_ACCESS, this->HASH_CHANGE, this->HASH_CREATION, this->HASH_MODIFICATION,
     this->HASH_NOW, this->HASH_TODAY
   }),

   // attributes, that can be altered by external circumstances
   // for example, last modification time of an element may be changed by another process
   // but its parent directory stays the same
   HASH_GROUP_ALTERABLE_ATTR({
     this->HASH_ACCESS, this->HASH_ARCHIVE, this->HASH_CHANGE, this->HASH_COMPRESSED,
     this->HASH_CREATION, this->HASH_EMPTY, this->HASH_EXISTS, this->HASH_ENCRYPTED,
     this->HASH_HIDDEN, this->HASH_ISDIRECTORY, this->HASH_ISFILE, this->HASH_LIFETIME,
     this->HASH_MODIFICATION, this->HASH_READONLY, this->HASH_SIZE
   }),

   HASH_GROUP_VARS_IMMUTABLES({
      this->HASH_THIS, this->HASH_ACCESS, this->HASH_ARCHIVE, this->HASH_CHANGE,
      this->HASH_COMPRESSED, this->HASH_CREATION, this->HASH_DEPTH, this->HASH_DRIVE,
      this->HASH_EMPTY, this->HASH_ENCRYPTED, this->HASH_EXISTS, this->HASH_EXTENSION,
      this->HASH_FULLNAME, this->HASH_HIDDEN, this->HASH_INDEX, this->HASH_ISDIRECTORY,
      this->HASH_ISFILE, this->HASH_LIFETIME, this->HASH_LOCATION, this->HASH_MODIFICATION,
      this->HASH_NAME, this->HASH_PARENT, this->HASH_PATH, this->HASH_READONLY, 
      this->HASH_SIZE, this->HASH_SUCCESS, this->HASH_NOW, this->HASH_TODAY,
      this->HASH_YESTERDAY, this->HASH_TOMORROW, this->HASH_JANUARY, this->HASH_FEBRUARY,
      this->HASH_MARCH, this->HASH_APRIL, this->HASH_MAY, this->HASH_JUNE, this->HASH_JULY,
      this->HASH_AUGUST, this->HASH_SEPTEMBER, this->HASH_OCTOBER, this->HASH_NOVEMBER,
      this->HASH_DECEMBER, this->HASH_MONDAY, this->HASH_TUESDAY, this->HASH_WEDNESDAY,
      this->HASH_THURSDAY, this->HASH_FRIDAY, this->HASH_SATURDAY, this->HASH_SUNDAY,
      this->HASH_ALPHABET, this->HASH_ASCII, this->HASH_ARGUMENTS, this->HASH_DESKTOP,
      this->HASH_PERUN2, this->HASH_ORIGIN, this->HASH_DIRECTORIES,
      this->HASH_FILES, this->HASH_RECURSIVEFILES, this->HASH_RECURSIVEDIRECTORIES
   }),

   HASH_GROUP_FUNC_BOO_STR({
     this->HASH_ISLOWER, this->HASH_ISUPPER, this->HASH_ISNUMBER,
     this->HASH_ISLETTER, this->HASH_ISDIGIT, this->HASH_ISBINARY, this->HASH_ISHEX
   }),

   HASH_GROUP_FUNC_NUM_NUM({
     this->HASH_ABSOLUTE, this->HASH_CEIL, this->HASH_FLOOR,
     this->HASH_ROUND, this->HASH_SIGN, this->HASH_SQRT, this->HASH_TRUNCATE
   }),

   HASH_GROUP_AGGRFUNC({
     this->HASH_AVERAGE, this->HASH_SUM, this->HASH_MIN, this->HASH_MAX, this->HASH_MEDIAN
   }),

   HASH_GROUP_FUNC_STR_STR({
     this->HASH_DIGITS,  this->HASH_LETTERS, this->HASH_LOWER, this->HASH_TRIM,
     this->HASH_UPPER, this->HASH_REVERSE, this->HASH_AFTERDIGITS, this->HASH_AFTERLETTERS,
     this->HASH_BEFOREDIGITS, this->HASH_BEFORELETTERS, this->HASH_CAPITALIZE, this->HASH_PARENT
   }),

   HASH_GROUP_FUNC_STR_STR_NUM({
     this->HASH_REPEAT, this->HASH_LEFT, this->HASH_RIGHT, this->HASH_FILL
   }),

   HASH_GROUP_FUNC_STR_NUM({
     this->HASH_ROMAN, this->HASH_BINARY, this->HASH_HEX
   }),

   HASH_GROUP_FUNC_TIM_NUM({
     this->HASH_CHRISTMAS, this->HASH_EASTER, this->HASH_NEWYEAR
   }),

   HASH_MAP_MONTHS({
      { this->HASH_JANUARY,    TNUM_JANUARY },
      { this->HASH_FEBRUARY,   TNUM_FEBRUARY },
      { this->HASH_MARCH,      TNUM_MARCH },
      { this->HASH_APRIL,      TNUM_APRIL },
      { this->HASH_MAY,        TNUM_MAY },
      { this->HASH_JUNE,       TNUM_JUNE },
      { this->HASH_JULY,       TNUM_JULY },
      { this->HASH_AUGUST,     TNUM_AUGUST },
      { this->HASH_SEPTEMBER,  TNUM_SEPTEMBER },
      { this->HASH_OCTOBER,    TNUM_OCTOBER },
      { this->HASH_NOVEMBER,   TNUM_NOVEMBER },
      { this->HASH_DECEMBER,   TNUM_DECEMBER }
   }),

   HASH_MAP_WEEKDAYS({
      { this->HASH_MONDAY,     TNUM_MONDAY },
      { this->HASH_TUESDAY,    TNUM_TUESDAY },
      { this->HASH_WEDNESDAY,  TNUM_WEDNESDAY },
      { this->HASH_THURSDAY,   TNUM_THURSDAY },
      { this->HASH_FRIDAY,     TNUM_FRIDAY },
      { this->HASH_SATURDAY,   TNUM_SATURDAY },
      { this->HASH_SUNDAY,     TNUM_SUNDAY }
   }),

   HASH_MAP_PERIOD_UNITS({
      { this->HASH_YEAR,       Period::u_Years },
      { this->HASH_YEARS,      Period::u_Years },
      { this->HASH_MONTH,      Period::u_Months },
      { this->HASH_MONTHS,     Period::u_Months },
      { this->HASH_WEEK,       Period::u_Weeks },
      { this->HASH_WEEKS,      Period::u_Weeks },
      { this->HASH_DAY,        Period::u_Days },
      { this->HASH_DAYS,       Period::u_Days },
      { this->HASH_HOUR,       Period::u_Hours },
      { this->HASH_HOURS,      Period::u_Hours },
      { this->HASH_MINUTE,     Period::u_Minutes },
      { this->HASH_MINUTES,    Period::u_Minutes },
      { this->HASH_SECOND,     Period::u_Seconds },
      { this->HASH_SECONDS,    Period::u_Seconds }
   }) { };

}
