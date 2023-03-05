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

#include "hash.h"
#include "datatype/strings.h"
#include <unordered_map>
#include <algorithm>


namespace uro
{

// case-insensitive hash of a string
_hash stringHash(const _str& value)
{
   _str lower;
   lower.resize(value.size());
   std::transform(value.begin(), value.end(), lower.begin(), ::towlower);
   return std::hash<_str>{}(lower);
}

// call this if the string is guaranteed to be lowercase
_hash rawStringHash(const _str& value)
{
   return std::hash<_str>{}(value);
}

// prepare data for future hash comparisons
Hashes::Hashes() :
   HASH_NOTHING(rawStringHash(EMPTY_STRING)),
   HASH_VAR_THIS(rawStringHash(STRING_THIS)),

   HASH_VAR_ACCESS(rawStringHash(STRING_ACCESS)),
   HASH_VAR_ARCHIVE(rawStringHash(STRING_ARCHIVE)),
   HASH_VAR_CHANGE(rawStringHash(STRING_CHANGE)),
   HASH_VAR_COMPRESSED(rawStringHash(STRING_COMPRESSED)),
   HASH_VAR_CREATION(rawStringHash(STRING_CREATION)),
   HASH_VAR_DEPTH(rawStringHash(STRING_DEPTH)),
   HASH_VAR_DRIVE(rawStringHash(STRING_DRIVE)),
   HASH_VAR_EMPTY(rawStringHash(STRING_EMPTY)),
   HASH_VAR_ENCRYPTED(rawStringHash(STRING_ENCRYPTED)),
   HASH_VAR_EXISTS(rawStringHash(STRING_EXISTS)),
   HASH_VAR_EXTENSION(rawStringHash(STRING_EXTENSION)),
   HASH_VAR_FULLNAME(rawStringHash(STRING_FULLNAME)),
   HASH_VAR_HIDDEN(rawStringHash(STRING_HIDDEN)),
   HASH_VAR_INDEX(rawStringHash(STRING_INDEX)),
   HASH_VAR_ISDIRECTORY(rawStringHash(STRING_ISDIRECTORY)),
   HASH_VAR_ISFILE(rawStringHash(STRING_ISFILE)),
   HASH_VAR_LIFETIME(rawStringHash(STRING_LIFETIME)),
   HASH_VAR_LOCATION(rawStringHash(STRING_LOCATION)),
   HASH_VAR_MODIFICATION(rawStringHash(STRING_MODIFICATION)),
   HASH_VAR_NAME(rawStringHash(STRING_NAME)),
   HASH_VAR_PARENT(rawStringHash(STRING_PARENT)),
   HASH_VAR_PATH(rawStringHash(STRING_PATH)),
   HASH_VAR_READONLY(rawStringHash(STRING_READONLY)),
   HASH_VAR_SIZE(rawStringHash(STRING_SIZE)),
   HASH_VAR_SUCCESS(rawStringHash(STRING_SUCCESS)),

   HASH_VAR_NOW(rawStringHash(STRING_NOW)),
   HASH_VAR_TODAY(rawStringHash(STRING_TODAY)),
   HASH_VAR_YESTERDAY(rawStringHash(STRING_YESTERDAY)),
   HASH_VAR_TOMORROW(rawStringHash(STRING_TOMORROW)),

   HASH_VAR_JANUARY(rawStringHash(STRING_JANUARY)),
   HASH_VAR_FEBRUARY(rawStringHash(STRING_FEBRUARY)),
   HASH_VAR_MARCH(rawStringHash(STRING_MARCH)),
   HASH_VAR_APRIL(rawStringHash(STRING_APRIL)),
   HASH_VAR_MAY(rawStringHash(STRING_MAY)),
   HASH_VAR_JUNE(rawStringHash(STRING_JUNE)),
   HASH_VAR_JULY(rawStringHash(STRING_JULY)),
   HASH_VAR_AUGUST(rawStringHash(STRING_AUGUST)),
   HASH_VAR_SEPTEMBER(rawStringHash(STRING_SEPTEMBER)),
   HASH_VAR_OCTOBER(rawStringHash(STRING_OCTOBER)),
   HASH_VAR_NOVEMBER(rawStringHash(STRING_NOVEMBER)),
   HASH_VAR_DECEMBER(rawStringHash(STRING_DECEMBER)),

   HASH_VAR_MONDAY(rawStringHash(STRING_MONDAY)),
   HASH_VAR_TUESDAY(rawStringHash(STRING_TUESDAY)),
   HASH_VAR_WEDNESDAY(rawStringHash(STRING_WEDNESDAY)),
   HASH_VAR_THURSDAY(rawStringHash(STRING_THURSDAY)),
   HASH_VAR_FRIDAY(rawStringHash(STRING_FRIDAY)),
   HASH_VAR_SATURDAY(rawStringHash(STRING_SATURDAY)),
   HASH_VAR_SUNDAY(rawStringHash(STRING_SUNDAY)),

   HASH_VAR_ALPHABET(rawStringHash(STRING_ALPHABET)),
   HASH_VAR_ASCII(rawStringHash(STRING_ASCII)),
   HASH_VAR_ARGUMENTS(rawStringHash(STRING_ARGUMENTS)),

   HASH_VAR_DESKTOP(rawStringHash(STRING_DESKTOP)),
   HASH_VAR_UROBOROS2(rawStringHash(STRING_UROBOROS2)),
   HASH_VAR_ORIGIN(rawStringHash(STRING_ORIGIN)),

   HASH_VAR_DIRECTORIES(rawStringHash(STRING_DIRECTORIES)),
   HASH_VAR_FILES(rawStringHash(STRING_FILES)),
   HASH_VAR_RECURSIVEFILES(rawStringHash(STRING_RECURSIVEFILES)),
   HASH_VAR_RECURSIVEDIRECTORIES(rawStringHash(STRING_RECURSIVEDIRECTORIES)),

   HASH_PER_YEAR(rawStringHash(STRING_YEAR)),
   HASH_PER_MONTH(rawStringHash(STRING_MONTH)),
   HASH_PER_WEEK(rawStringHash(STRING_WEEK)),
   HASH_PER_DAY(rawStringHash(STRING_DAY)),
   HASH_PER_HOUR(rawStringHash(STRING_HOUR)),
   HASH_PER_MINUTE(rawStringHash(STRING_MINUTE)),
   HASH_PER_SECOND(rawStringHash(STRING_SECOND)),

   HASH_PER_YEARS(rawStringHash(STRING_YEARS)),
   HASH_PER_MONTHS(rawStringHash(STRING_MONTHS)),
   HASH_PER_WEEKS(rawStringHash(STRING_WEEKS)),
   HASH_PER_DAYS(rawStringHash(STRING_DAYS)),
   HASH_PER_HOURS(rawStringHash(STRING_HOURS)),
   HASH_PER_MINUTES(rawStringHash(STRING_MINUTES)),
   HASH_PER_SECONDS(rawStringHash(STRING_SECONDS)),

   HASH_PER_DATE(rawStringHash(STRING_DATE)),
   HASH_PER_WEEKDAY(rawStringHash(STRING_WEEKDAY)),

   HASH_FUNC_ISLOWER(rawStringHash(STRING_ISLOWER)),
   HASH_FUNC_ISUPPER(rawStringHash(STRING_ISUPPER)),
   HASH_FUNC_ISNUMBER(rawStringHash(STRING_ISNUMBER)),
   HASH_FUNC_ISLETTER(rawStringHash(STRING_ISLETTER)),
   HASH_FUNC_ISDIGIT(rawStringHash(STRING_ISDIGIT)),
   HASH_FUNC_ISBINARY(rawStringHash(STRING_ISBINARY)),
   HASH_FUNC_ISHEX(rawStringHash(STRING_ISHEX)),

   HASH_FUNC_ANYINSIDE(rawStringHash(STRING_ANYINSIDE)),
   HASH_FUNC_CONTAINS(rawStringHash(STRING_CONTAINS)),
   HASH_FUNC_EXISTSINSIDE(rawStringHash(STRING_EXISTSINSIDE)),
   HASH_FUNC_EXISTINSIDE(rawStringHash(STRING_EXISTINSIDE)),
   HASH_FUNC_STARTSWITH(rawStringHash(STRING_STARTSWITH)),
   HASH_FUNC_ENDSWITH(rawStringHash(STRING_ENDSWITH)),
   HASH_FUNC_FIND(rawStringHash(STRING_FIND)),

   HASH_FUNC_ABSOLUTE(rawStringHash(STRING_ABSOLUTE)),
   HASH_FUNC_CEIL(rawStringHash(STRING_CEIL)),
   HASH_FUNC_FLOOR(rawStringHash(STRING_FLOOR)),
   HASH_FUNC_ROUND(rawStringHash(STRING_ROUND)),
   HASH_FUNC_SIGN(rawStringHash(STRING_SIGN)),
   HASH_FUNC_SQRT(rawStringHash(STRING_SQRT)),
   HASH_FUNC_TRUNCATE(rawStringHash(STRING_TRUNCATE)),

   HASH_FUNC_AVERAGE(rawStringHash(STRING_AVERAGE)),
   HASH_FUNC_SUM(rawStringHash(STRING_SUM)),
   HASH_FUNC_MIN(rawStringHash(STRING_MIN)),
   HASH_FUNC_MAX(rawStringHash(STRING_MAX)),
   HASH_FUNC_MEDIAN(rawStringHash(STRING_MEDIAN)),

   HASH_FUNC_LENGTH(rawStringHash(STRING_LENGTH)),
   HASH_FUNC_FROMBINARY(rawStringHash(STRING_FROMBINARY)),
   HASH_FUNC_FROMHEX(rawStringHash(STRING_FROMHEX)),
   HASH_FUNC_SIZE(rawStringHash(STRING_SIZE)),
   HASH_FUNC_NUMBER(rawStringHash(STRING_NUMBER)),
   HASH_FUNC_COUNTINSIDE(rawStringHash(STRING_COUNTINSIDE)),
   HASH_FUNC_POWER(rawStringHash(STRING_POWER)),

   HASH_FUNC_FIRST(rawStringHash(STRING_FIRST)),
   HASH_FUNC_LAST(rawStringHash(STRING_LAST)),
   HASH_FUNC_RANDOM(rawStringHash(STRING_RANDOM)),
   HASH_FUNC_LIFETIME(rawStringHash(STRING_LIFETIME)),

   HASH_FUNC_AFTER(rawStringHash(STRING_AFTER)),
   HASH_FUNC_BEFORE(rawStringHash(STRING_BEFORE)),
   HASH_FUNC_REVERSED(rawStringHash(STRING_REVERSED)),

   HASH_FUNC_DIGITS(rawStringHash(STRING_DIGITS)),
   HASH_FUNC_LETTERS(rawStringHash(STRING_LETTERS)),
   HASH_FUNC_LOWER(rawStringHash(STRING_LOWER)),
   HASH_FUNC_TRIM(rawStringHash(STRING_TRIM)),
   HASH_FUNC_UPPER(rawStringHash(STRING_UPPER)),
   HASH_FUNC_REVERSE(rawStringHash(STRING_REVERSE)),
   HASH_FUNC_AFTERDIGITS(rawStringHash(STRING_AFTERDIGITS)),
   HASH_FUNC_AFTERLETTERS(rawStringHash(STRING_AFTERLETTERS)),
   HASH_FUNC_BEFOREDIGITS(rawStringHash(STRING_BEFOREDIGITS)),
   HASH_FUNC_BEFORELETTERS(rawStringHash(STRING_BEFORELETTERS)),
   HASH_FUNC_CAPITALIZE(rawStringHash(STRING_CAPITALIZE)),

   HASH_FUNC_REPLACE(rawStringHash(STRING_REPLACE)),
   HASH_FUNC_SUBSTRING(rawStringHash(STRING_SUBSTRING)),
   HASH_FUNC_CONCATENATE(rawStringHash(STRING_CONCATENATE)),
   HASH_FUNC_PATH(rawStringHash(STRING_PATH)),
   HASH_FUNC_STRING(rawStringHash(STRING_STRING)),
   HASH_FUNC_MONTHNAME(rawStringHash(STRING_MONTHNAME)),
   HASH_FUNC_WEEKDAYNAME(rawStringHash(STRING_WEEKDAYNAME)),
   HASH_FUNC_JOIN(rawStringHash(STRING_JOIN)),

   HASH_FUNC_REPEAT(rawStringHash(STRING_REPEAT)),
   HASH_FUNC_LEFT(rawStringHash(STRING_LEFT)),
   HASH_FUNC_RIGHT(rawStringHash(STRING_RIGHT)),
   HASH_FUNC_FILL(rawStringHash(STRING_FILL)),

   HASH_FUNC_ROMAN(rawStringHash(STRING_ROMAN)),
   HASH_FUNC_BINARY(rawStringHash(STRING_BINARY)),
   HASH_FUNC_HEX(rawStringHash(STRING_HEX)),

   HASH_FUNC_CHRISTMAS(rawStringHash(STRING_CHRISTMAS)),
   HASH_FUNC_EASTER(rawStringHash(STRING_EASTER)),
   HASH_FUNC_NEWYEAR(rawStringHash(STRING_NEWYEAR)),

   HASH_FUNC_DATE(rawStringHash(STRING_DATE)),
   HASH_FUNC_TIME(rawStringHash(STRING_TIME)),
   HASH_FUNC_CHARACTERS(rawStringHash(STRING_CHARACTERS)),
   HASH_FUNC_WORDS(rawStringHash(STRING_WORDS)),
   HASH_FUNC_SPLIT(rawStringHash(STRING_SPLIT)),
   HASH_FUNC_NUMBERS(rawStringHash(STRING_NUMBERS)),

   HASH_KW_COPY(rawStringHash(STRING_COPY)),
   HASH_KW_CREATE(rawStringHash(STRING_CREATE)),
   HASH_KW_CREATEFILE(rawStringHash(STRING_CREATEFILE)),
   HASH_KW_CREATEDIRECTORY(rawStringHash(STRING_CREATEDIRECTORY)),
   HASH_KW_CREATEFILES(rawStringHash(STRING_CREATEFILES)),
   HASH_KW_CREATEDIRECTORIES(rawStringHash(STRING_CREATEDIRECTORIES)),
   HASH_KW_DELETE(rawStringHash(STRING_DELETE)),
   HASH_KW_DROP(rawStringHash(STRING_DROP)),
   HASH_KW_HIDE(rawStringHash(STRING_HIDE)),
   HASH_KW_LOCK(rawStringHash(STRING_LOCK)),
   HASH_KW_MOVE(rawStringHash(STRING_MOVE)),
   HASH_KW_OPEN(rawStringHash(STRING_OPEN)),
   HASH_KW_REACCESS(rawStringHash(STRING_REACCESS)),
   HASH_KW_RECREATE(rawStringHash(STRING_RECREATE)),
   HASH_KW_RECHANGE(rawStringHash(STRING_RECHANGE)),
   HASH_KW_REMODIFY(rawStringHash(STRING_REMODIFY)),
   HASH_KW_RENAME(rawStringHash(STRING_RENAME)),
   HASH_KW_SELECT(rawStringHash(STRING_SELECT)),
   HASH_KW_UNHIDE(rawStringHash(STRING_UNHIDE)),
   HASH_KW_UNLOCK(rawStringHash(STRING_UNLOCK)),
   HASH_KW_FORCE(rawStringHash(STRING_FORCE)),
   HASH_KW_STACK(rawStringHash(STRING_STACK)),
   HASH_KW_TRUE(rawStringHash(STRING_TRUE)),
   HASH_KW_FALSE(rawStringHash(STRING_FALSE)),
   HASH_KW_AND(rawStringHash(STRING_AND)),
   HASH_KW_OR(rawStringHash(STRING_OR)),
   HASH_KW_XOR(rawStringHash(STRING_XOR)),
   HASH_KW_NOT(rawStringHash(STRING_NOT)),
   HASH_KW_PRINT(rawStringHash(STRING_PRINT)),
   HASH_KW_RUN(rawStringHash(STRING_RUN)),
   HASH_KW_SLEEP(rawStringHash(STRING_SLEEP)),
   HASH_KW_IN(rawStringHash(STRING_IN)),
   HASH_KW_LIKE(rawStringHash(STRING_LIKE)),
   HASH_KW_ELSE(rawStringHash(STRING_ELSE)),
   HASH_KW_IF(rawStringHash(STRING_IF)),
   HASH_KW_INSIDE(rawStringHash(STRING_INSIDE)),
   HASH_KW_TIMES(rawStringHash(STRING_TIMES)),
   HASH_KW_WHILE(rawStringHash(STRING_WHILE)),
   HASH_KW_EVERY(rawStringHash(STRING_EVERY)),
   HASH_KW_FINAL(rawStringHash(STRING_FINAL)),
   HASH_KW_LIMIT(rawStringHash(STRING_LIMIT)),
   HASH_KW_ORDER(rawStringHash(STRING_ORDER)),
   HASH_KW_SKIP(rawStringHash(STRING_SKIP)),
   HASH_KW_WHERE(rawStringHash(STRING_WHERE)),
   HASH_KW_AS(rawStringHash(STRING_AS)),
   HASH_KW_BY(rawStringHash(STRING_BY)),
   HASH_KW_FROM(rawStringHash(STRING_FROM)),
   HASH_KW_TO(rawStringHash(STRING_TO)),
   HASH_KW_EXTENSIONLESS(rawStringHash(STRING_EXTENSIONLESS)),
   HASH_KW_WITH(rawStringHash(STRING_WITH)),
   HASH_KW_ASC(rawStringHash(STRING_ASC)),
   HASH_KW_DESC(rawStringHash(STRING_DESC)),
   HASH_KW_BREAK(rawStringHash(STRING_BREAK)),
   HASH_KW_CONTINUE(rawStringHash(STRING_CONTINUE)),
   HASH_KW_EXIT(rawStringHash(STRING_EXIT)),
   HASH_KW_ERROR(rawStringHash(STRING_ERROR)),

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
      this->HASH_VAR_UROBOROS2, this->HASH_VAR_ORIGIN, this->HASH_VAR_DIRECTORIES,
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
     this->HASH_FUNC_BEFOREDIGITS, this->HASH_FUNC_BEFORELETTERS, this->HASH_FUNC_CAPITALIZE
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
