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

#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include "datatype/primitives.h"
#include "datatype/period.h"
#include <unordered_set>
#include <unordered_map>


namespace perun2
{

typedef std::size_t   _hash;

_hash caseInsensitiveHash(_str& value);
_hash caseSensitiveHash(const _str& value);

// prepare hashes for language keywords
// so, instead of comparing strings, we will compare hashes
struct Hashes
{
public:

   Hashes();

   const _hash NOTHING_HASH;
   const _hash HASH_THIS;

   const _hash HASH_ACCESS;
   const _hash HASH_ARCHIVE;
   const _hash HASH_CHANGE;
   const _hash HASH_COMPRESSED;
   const _hash HASH_CREATION;
   const _hash HASH_DEPTH;
   const _hash HASH_DRIVE;
   const _hash HASH_EMPTY;
   const _hash HASH_ENCRYPTED;
   const _hash HASH_EXISTS;
   const _hash HASH_EXTENSION;
   const _hash HASH_FULLNAME;
   const _hash HASH_HIDDEN;
   const _hash HASH_INDEX;
   const _hash HASH_ISDIRECTORY;
   const _hash HASH_ISFILE;
   const _hash HASH_LIFETIME;
   const _hash HASH_LOCATION;
   const _hash HASH_MODIFICATION;
   const _hash HASH_NAME;
   const _hash HASH_PARENT;
   const _hash HASH_PATH;
   const _hash HASH_READONLY;
   const _hash HASH_SIZE;
   const _hash HASH_SUCCESS;

   const _hash HASH_NOW;
   const _hash HASH_TODAY;
   const _hash HASH_YESTERDAY;
   const _hash HASH_TOMORROW;

   const _hash HASH_JANUARY;
   const _hash HASH_FEBRUARY;
   const _hash HASH_MARCH;
   const _hash HASH_APRIL;
   const _hash HASH_MAY;
   const _hash HASH_JUNE;
   const _hash HASH_JULY;
   const _hash HASH_AUGUST;
   const _hash HASH_SEPTEMBER;
   const _hash HASH_OCTOBER;
   const _hash HASH_NOVEMBER;
   const _hash HASH_DECEMBER;

   const _hash HASH_MONDAY;
   const _hash HASH_TUESDAY;
   const _hash HASH_WEDNESDAY;
   const _hash HASH_THURSDAY;
   const _hash HASH_FRIDAY;
   const _hash HASH_SATURDAY;
   const _hash HASH_SUNDAY;

   const _hash HASH_ALPHABET;
   const _hash HASH_ASCII;
   const _hash HASH_ARGUMENTS;

   const _hash HASH_DESKTOP;
   const _hash HASH_PERUN2;
   const _hash HASH_ORIGIN;

   const _hash HASH_PAINT;
   const _hash HASH_MSPAINT;
   const _hash HASH_NOTEPAD;

   const _hash HASH_DIRECTORIES;
   const _hash HASH_FILES;
   const _hash HASH_RECURSIVEFILES;
   const _hash HASH_RECURSIVEDIRECTORIES;

   const _hash HASH_YEAR;
   const _hash HASH_MONTH;
   const _hash HASH_WEEK;
   const _hash HASH_DAY;
   const _hash HASH_HOUR;
   const _hash HASH_MINUTE;
   const _hash HASH_SECOND;

   const _hash HASH_YEARS;
   const _hash HASH_MONTHS;
   const _hash HASH_WEEKS;
   const _hash HASH_DAYS;
   const _hash HASH_HOURS;
   const _hash HASH_MINUTES;
   const _hash HASH_SECONDS;

   const _hash HASH_WEEKDAY;

   const _hash HASH_ISLOWER;
   const _hash HASH_ISUPPER;
   const _hash HASH_ISNUMBER;
   const _hash HASH_ISLETTER;
   const _hash HASH_ISDIGIT;
   const _hash HASH_ISBINARY;
   const _hash HASH_ISHEX;

   const _hash HASH_ANYINSIDE;
   const _hash HASH_CONTAINS;
   const _hash HASH_EXISTSINSIDE;
   const _hash HASH_EXISTINSIDE;
   const _hash HASH_STARTSWITH;
   const _hash HASH_ENDSWITH;
   const _hash HASH_FINDTEXT;

   const _hash HASH_ABSOLUTE;
   const _hash HASH_CEIL;
   const _hash HASH_FLOOR;
   const _hash HASH_ROUND;
   const _hash HASH_SIGN;
   const _hash HASH_SQRT;
   const _hash HASH_TRUNCATE;

   const _hash HASH_AVERAGE;
   const _hash HASH_SUM;
   const _hash HASH_MIN;
   const _hash HASH_MAX;
   const _hash HASH_MEDIAN;

   const _hash HASH_LENGTH;
   const _hash HASH_FROMBINARY;
   const _hash HASH_FROMHEX;
   const _hash HASH_NUMBER;
   const _hash HASH_COUNTINSIDE;
   const _hash HASH_POWER;

   const _hash HASH_ANY;
   const _hash HASH_EXIST;
   const _hash HASH_COUNT;

   const _hash HASH_FIRST;
   const _hash HASH_LAST;
   const _hash HASH_RANDOM;

   const _hash HASH_AFTER;
   const _hash HASH_BEFORE;
   const _hash HASH_REVERSED;

   const _hash HASH_DIGITS;
   const _hash HASH_LETTERS;
   const _hash HASH_LOWER;
   const _hash HASH_TRIM;
   const _hash HASH_UPPER;
   const _hash HASH_REVERSE;
   const _hash HASH_AFTERDIGITS;
   const _hash HASH_AFTERLETTERS;
   const _hash HASH_BEFOREDIGITS;
   const _hash HASH_BEFORELETTERS;
   const _hash HASH_CAPITALIZE;

   const _hash HASH_REPLACE;
   const _hash HASH_SUBSTRING;
   const _hash HASH_CONCATENATE;
   const _hash HASH_STRING;
   const _hash HASH_MONTHNAME;
   const _hash HASH_WEEKDAYNAME;
   const _hash HASH_JOIN;

   const _hash HASH_REPEAT;
   const _hash HASH_LEFT;
   const _hash HASH_RIGHT;
   const _hash HASH_FILL;

   const _hash HASH_ROMAN;
   const _hash HASH_BINARY;
   const _hash HASH_HEX;

   const _hash HASH_CHRISTMAS;
   const _hash HASH_EASTER;
   const _hash HASH_NEWYEAR;

   const _hash HASH_DATE;
   const _hash HASH_TIME;
   const _hash HASH_CHARACTERS;
   const _hash HASH_WORDS;
   const _hash HASH_SPLIT;
   const _hash HASH_NUMBERS;
   const _hash HASH_SHIFTMONTH;
   const _hash HASH_SHIFTWEEKDAY;

   const _hash HASH_COPY;
   const _hash HASH_CREATE;
   const _hash HASH_CREATEFILE;
   const _hash HASH_CREATEDIRECTORY;
   const _hash HASH_CREATEFILES;
   const _hash HASH_CREATEDIRECTORIES;
   const _hash HASH_DELETE;
   const _hash HASH_DROP;
   const _hash HASH_HIDE;
   const _hash HASH_LOCK;
   const _hash HASH_MOVE;
   const _hash HASH_OPEN;
   const _hash HASH_REACCESS;
   const _hash HASH_RECREATE;
   const _hash HASH_RECHANGE;
   const _hash HASH_REMODIFY;
   const _hash HASH_RENAME;
   const _hash HASH_SELECT;
   const _hash HASH_UNHIDE;
   const _hash HASH_UNLOCK;
   const _hash HASH_FORCE;
   const _hash HASH_STACK;
   const _hash HASH_TRUE;
   const _hash HASH_FALSE;
   const _hash HASH_AND;
   const _hash HASH_OR;
   const _hash HASH_XOR;
   const _hash HASH_NOT;
   const _hash HASH_PRINT;
   const _hash HASH_RUN;
   const _hash HASH_SLEEP;
   const _hash HASH_IN;
   const _hash HASH_LIKE;
   const _hash HASH_ELSE;
   const _hash HASH_IF;
   const _hash HASH_INSIDE;
   const _hash HASH_TIMES;
   const _hash HASH_WHILE;
   const _hash HASH_EVERY;
   const _hash HASH_FINAL;
   const _hash HASH_LIMIT;
   const _hash HASH_ORDER;
   const _hash HASH_SKIP;
   const _hash HASH_WHERE;
   const _hash HASH_AS;
   const _hash HASH_BY;
   const _hash HASH_TO;
   const _hash HASH_EXTENSIONLESS;
   const _hash HASH_WITH;
   const _hash HASH_ASC;
   const _hash HASH_DESC;
   const _hash HASH_BREAK;
   const _hash HASH_CONTINUE;
   const _hash HASH_EXIT;
   const _hash HASH_ERROR;

   const std::unordered_set<_hash> HASH_GROUP_ALIASES;
   const std::unordered_set<_hash> HASH_GROUP_MONTHS;
   const std::unordered_set<_hash> HASH_GROUP_WEEKDAYS;
   const std::unordered_set<_hash> HASH_GROUP_PERIOD_SINGLE;
   const std::unordered_set<_hash> HASH_GROUP_PERIOD_MULTI;
   const std::unordered_set<_hash> HASH_GROUP_ATTR;
   const std::unordered_set<_hash> HASH_GROUP_TIME_ATTR;
   const std::unordered_set<_hash> HASH_GROUP_TIME_VAR;
   const std::unordered_set<_hash> HASH_GROUP_ALTERABLE_ATTR;
   const std::unordered_set<_hash> HASH_GROUP_VARS_IMMUTABLES;
   const std::unordered_set<_hash> HASH_GROUP_FUNC_BOO_STR;
   const std::unordered_set<_hash> HASH_GROUP_FUNC_NUM_NUM;
   const std::unordered_set<_hash> HASH_GROUP_AGGRFUNC;
   const std::unordered_set<_hash> HASH_GROUP_FUNC_STR_STR;
   const std::unordered_set<_hash> HASH_GROUP_FUNC_STR_STR_NUM;
   const std::unordered_set<_hash> HASH_GROUP_FUNC_STR_NUM;
   const std::unordered_set<_hash> HASH_GROUP_FUNC_TIM_NUM;
   const std::unordered_map<_hash, _tnum> HASH_MAP_MONTHS;
   const std::unordered_map<_hash, _tnum> HASH_MAP_WEEKDAYS;
   const std::unordered_map<_hash, Period::PeriodUnit> HASH_MAP_PERIOD_UNITS;

};

}

#endif // HASH_H_INCLUDED
