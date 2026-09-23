/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "chars.h"
#include <vector>


namespace perun2
{

struct Token;

p_str intToString(const p_nint value);
p_str doubleToString(const p_ndouble value);
p_str charToString(const p_char value);

p_ndouble stringToDouble(const p_str& value);

void str_toLower(p_str& value);
void str_toUpper(p_str& value);
void str_trim(p_str& value);
void str_trimEndNewLines(p_str& value);
p_bool str_startsWith(const p_str& value, const p_str& phrase);

p_constexpr p_int LETTERS_IN_ENGLISH_ALPHABET = 26;

extern const p_str ROMAN_VINCULUM_THOUSAND;
extern const p_list ROMAN_STRING_LITERALS;
extern const p_list STRINGS_ASCII;

extern const p_str STRING_WINDOWS_PATH_PREFIX;
extern const p_str STRING_POPUP_TITLE;
extern const p_str STRING_GOOD;

extern const p_str EMPTY_STRING;
extern const p_str STRING_NO_PERIOD;
extern const p_str STRING_NOTHING;
extern const p_str STRING_NEVER;
extern const p_str STRING_DOWNLOADS;

extern const p_str STRING_ARG_VERSION;
extern const p_str STRING_ARG_DOCS;
extern const p_str STRING_ARG_WEBSITE;
extern const p_str STRING_ARG_HELP;

extern const p_str STRING_ICON_SUFFIX;

extern const p_str STRING_PENDRIVE;
extern const p_str STRING_PENDRIVES;
extern const p_str STRING_PROGRAMS;

extern const p_str STRING_WEEKDAY_MONDAY;
extern const p_str STRING_WEEKDAY_TUESDAY;
extern const p_str STRING_WEEKDAY_WEDNESDAY;
extern const p_str STRING_WEEKDAY_THURSDAY;
extern const p_str STRING_WEEKDAY_FRIDAY;
extern const p_str STRING_WEEKDAY_SATURDAY;
extern const p_str STRING_WEEKDAY_SUNDAY;

extern const p_str STRING_MONTH_JANUARY;
extern const p_str STRING_MONTH_FEBRUARY;
extern const p_str STRING_MONTH_MARCH;
extern const p_str STRING_MONTH_APRIL;
extern const p_str STRING_MONTH_MAY;
extern const p_str STRING_MONTH_JUNE;
extern const p_str STRING_MONTH_JULY;
extern const p_str STRING_MONTH_AUGUST;
extern const p_str STRING_MONTH_SEPTEMBER;
extern const p_str STRING_MONTH_OCTOBER;
extern const p_str STRING_MONTH_NOVEMBER;
extern const p_str STRING_MONTH_DECEMBER;

extern const p_str STRING_BOOL;
extern const p_str STRING_PERIOD;
extern const p_str STRING_DEFINITION;
extern const p_str STRING_NUMERIC_LIST;
extern const p_str STRING_TIME_LIST;
extern const p_str STRING_LIST;

extern const p_str STRING_NAN;
extern const p_str STRING_PRINTABLE_NAN;

extern const p_str STRING_THIS;
extern const p_str STRING_ACCESS;
extern const p_str STRING_ARCHIVE;
extern const p_str STRING_CHANGE;
extern const p_str STRING_COMPRESSED;
extern const p_str STRING_CREATION;
extern const p_str STRING_DEPTH;
extern const p_str STRING_DRIVE;
extern const p_str STRING_DURATION;
extern const p_str STRING_EMPTY;
extern const p_str STRING_ENCRYPTED;
extern const p_str STRING_EXISTS;
extern const p_str STRING_EXTENSION;
extern const p_str STRING_FULLNAME;
extern const p_str STRING_HEIGHT;
extern const p_str STRING_HIDDEN;
extern const p_str STRING_INDEX;
extern const p_str STRING_ISDIRECTORY;
extern const p_str STRING_ISFILE;
extern const p_str STRING_ISIMAGE;
extern const p_str STRING_ISVIDEO;
extern const p_str STRING_LIFETIME;
extern const p_str STRING_LOCATION;
extern const p_str STRING_MODIFICATION;
extern const p_str STRING_NAME;
extern const p_str STRING_PARENT;
extern const p_str STRING_PATH;
extern const p_str STRING_READONLY;
extern const p_str STRING_SIZE;
extern const p_str STRING_WIDTH;

extern const p_str STRING_SUCCESS;
extern const p_str STRING_NOW;
extern const p_str STRING_TODAY;
extern const p_str STRING_YESTERDAY;
extern const p_str STRING_TOMORROW;
extern const p_str STRING_JANUARY;
extern const p_str STRING_FEBRUARY;
extern const p_str STRING_MARCH;
extern const p_str STRING_APRIL;
extern const p_str STRING_MAY;
extern const p_str STRING_JUNE;
extern const p_str STRING_JULY;
extern const p_str STRING_AUGUST;
extern const p_str STRING_SEPTEMBER;
extern const p_str STRING_OCTOBER;
extern const p_str STRING_NOVEMBER;
extern const p_str STRING_DECEMBER;
extern const p_str STRING_MONDAY;
extern const p_str STRING_TUESDAY;
extern const p_str STRING_WEDNESDAY;
extern const p_str STRING_THURSDAY;
extern const p_str STRING_FRIDAY;
extern const p_str STRING_SATURDAY;
extern const p_str STRING_SUNDAY;
extern const p_str STRING_ALPHABET;
extern const p_str STRING_ASCII;
extern const p_str STRING_ARGUMENTS;
extern const p_str STRING_DESKTOP;
extern const p_str STRING_PERUN2;
extern const p_str STRING_ORIGIN;
extern const p_str STRING_DIRECTORIES;
extern const p_str STRING_FILES;
extern const p_str STRING_IMAGES;
extern const p_str STRING_VIDEOS;
extern const p_str STRING_RECURSIVEDIRECTORIES;
extern const p_str STRING_RECURSIVEFILES;
extern const p_str STRING_RECURSIVEIMAGES;
extern const p_str STRING_RECURSIVEVIDEOS;
extern const p_str STRING_YEAR;
extern const p_str STRING_MONTH;
extern const p_str STRING_WEEK;
extern const p_str STRING_DAY;
extern const p_str STRING_HOUR;
extern const p_str STRING_MINUTE;
extern const p_str STRING_SECOND;
extern const p_str STRING_YEARS;
extern const p_str STRING_MONTHS;
extern const p_str STRING_WEEKS;
extern const p_str STRING_DAYS;
extern const p_str STRING_HOURS;
extern const p_str STRING_MINUTES;
extern const p_str STRING_SECONDS;
extern const p_str STRING_DATE;
extern const p_str STRING_WEEKDAY;
extern const p_str STRING_WEEKDAY_CAMELCASE;
extern const p_str STRING_ISLOWER;
extern const p_str STRING_ISUPPER;
extern const p_str STRING_ISNUMBER;
extern const p_str STRING_ISLETTER;
extern const p_str STRING_ISDIGIT;
extern const p_str STRING_ISBINARY;
extern const p_str STRING_ISHEX;
extern const p_str STRING_ANYINSIDE;
extern const p_str STRING_ANY;
extern const p_str STRING_EXIST;
extern const p_str STRING_COUNT;
extern const p_str STRING_CONTAINS;
extern const p_str STRING_EXISTSINSIDE;
extern const p_str STRING_EXISTINSIDE;
extern const p_str STRING_STARTSWITH;
extern const p_str STRING_ENDSWITH;
extern const p_str STRING_FINDTEXT;
extern const p_str STRING_ABSOLUTE;
extern const p_str STRING_CEIL;
extern const p_str STRING_FLOOR;
extern const p_str STRING_ROUND;
extern const p_str STRING_SIGN;
extern const p_str STRING_SQRT;
extern const p_str STRING_TRUNCATE;
extern const p_str STRING_AVERAGE;
extern const p_str STRING_SUM;
extern const p_str STRING_MIN;
extern const p_str STRING_MAX;
extern const p_str STRING_MEDIAN;
extern const p_str STRING_LENGTH;
extern const p_str STRING_FROMBINARY;
extern const p_str STRING_FROMHEX;
extern const p_str STRING_NUMBER;
extern const p_str STRING_COUNTINSIDE;
extern const p_str STRING_POWER;
extern const p_str STRING_FIRST;
extern const p_str STRING_LAST;
extern const p_str STRING_RANDOM;
extern const p_str STRING_AFTER;
extern const p_str STRING_BEFORE;
extern const p_str STRING_REVERSED;
extern const p_str STRING_DIGITS;
extern const p_str STRING_LETTERS;
extern const p_str STRING_LOWER;
extern const p_str STRING_TRIM;
extern const p_str STRING_UPPER;
extern const p_str STRING_REVERSE;
extern const p_str STRING_AFTERDIGITS;
extern const p_str STRING_AFTERLETTERS;
extern const p_str STRING_BEFOREDIGITS;
extern const p_str STRING_BEFORELETTERS;
extern const p_str STRING_CAPITALIZE;
extern const p_str STRING_REPLACE;
extern const p_str STRING_SUBSTRING;
extern const p_str STRING_CONCATENATE;
extern const p_str STRING_STRING;
extern const p_str STRING_MONTHNAME;
extern const p_str STRING_WEEKDAYNAME;
extern const p_str STRING_JOIN;
extern const p_str STRING_REPEAT;
extern const p_str STRING_LEFT;
extern const p_str STRING_RIGHT;
extern const p_str STRING_FILL;
extern const p_str STRING_ROMAN;
extern const p_str STRING_BINARY;
extern const p_str STRING_HEX;
extern const p_str STRING_CHRISTMAS;
extern const p_str STRING_EASTER;
extern const p_str STRING_NEWYEAR;
extern const p_str STRING_TIME;
extern const p_str STRING_CHARACTERS;
extern const p_str STRING_WORDS;
extern const p_str STRING_SPLIT;
extern const p_str STRING_NUMBERS;
extern const p_str STRING_SHIFTMONTH;
extern const p_str STRING_SHIFTWEEKDAY;
extern const p_str STRING_ISNAN;
extern const p_str STRING_ISNEVER;
extern const p_str STRING_CLOCK;
extern const p_str STRING_RAW;
extern const p_str STRING_RESEMBLANCE;
extern const p_str STRING_ASKPYTHON;
extern const p_str STRING_ASKPYTHON3;
extern const p_str STRING_EXECUTE;

extern const p_str STRING_COPY;
extern const p_str STRING_CREATE;
extern const p_str STRING_CREATEFILE;
extern const p_str STRING_CREATEDIRECTORY;
extern const p_str STRING_CREATEFILES;
extern const p_str STRING_CREATEDIRECTORIES;
extern const p_str STRING_DELETE;
extern const p_str STRING_DROP;
extern const p_str STRING_HIDE;
extern const p_str STRING_LOCK;
extern const p_str STRING_MOVE;
extern const p_str STRING_OPEN;
extern const p_str STRING_REACCESS;
extern const p_str STRING_RECREATE;
extern const p_str STRING_RECHANGE;
extern const p_str STRING_REMODIFY;
extern const p_str STRING_RENAME;
extern const p_str STRING_SELECT;
extern const p_str STRING_UNHIDE;
extern const p_str STRING_UNLOCK;
extern const p_str STRING_POPUP;
extern const p_str STRING_FORCE;
extern const p_str STRING_STACK;
extern const p_str STRING_TRUE;
extern const p_str STRING_FALSE;
extern const p_str STRING_AND;
extern const p_str STRING_OR;
extern const p_str STRING_XOR;
extern const p_str STRING_NOT;
extern const p_str STRING_PRINT;
extern const p_str STRING_RUN;
extern const p_str STRING_SLEEP;
extern const p_str STRING_IN;
extern const p_str STRING_LIKE;
extern const p_str STRING_RESEMBLES;
extern const p_str STRING_REGEXP;
extern const p_str STRING_BETWEEN;
extern const p_str STRING_ELSE;
extern const p_str STRING_IF;
extern const p_str STRING_FOREACH;
extern const p_str STRING_INSIDE;
extern const p_str STRING_TIMES;
extern const p_str STRING_WHILE;
extern const p_str STRING_EVERY;
extern const p_str STRING_FINAL;
extern const p_str STRING_LIMIT;
extern const p_str STRING_ORDER;
extern const p_str STRING_SKIP;
extern const p_str STRING_WHERE;
extern const p_str STRING_FROM;
extern const p_str STRING_AS;
extern const p_str STRING_BY;
extern const p_str STRING_TO;
extern const p_str STRING_EXTENSIONLESS;
extern const p_str STRING_WITH;
extern const p_str STRING_ASC;
extern const p_str STRING_DESC;
extern const p_str STRING_BREAK;
extern const p_str STRING_CONTINUE;
extern const p_str STRING_EXIT;
extern const p_str STRING_ERROR;
extern const p_str STRING_PYTHON;
extern const p_str STRING_PYTHON3;

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
