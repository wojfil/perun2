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

#include "keyword.h"
#include "datatype/strings.h"
#include "perun2.h"


namespace perun2
{

KeywordsData::KeywordsData() : KEYWORDS( 
{
   // core commands:
   { STRING_COPY, Keyword::kw_Copy },
   { STRING_CREATE, Keyword::kw_Create },
   { STRING_CREATEFILE, Keyword::kw_CreateFile },
   { STRING_CREATEDIRECTORY, Keyword::kw_CreateDirectory },
   { STRING_CREATEFILES, Keyword::kw_CreateFiles },
   { STRING_CREATEDIRECTORIES, Keyword::kw_CreateDirectories },
   { STRING_DELETE, Keyword::kw_Delete },
   { STRING_DROP, Keyword::kw_Drop },
   { STRING_HIDE, Keyword::kw_Hide },
   { STRING_LOCK, Keyword::kw_Lock },
   { STRING_MOVE, Keyword::kw_Move },
   { STRING_OPEN, Keyword::kw_Open },
   { STRING_REACCESS, Keyword::kw_Reaccess },
   { STRING_RECREATE, Keyword::kw_Recreate },
   { STRING_RECHANGE, Keyword::kw_Rechange },
   { STRING_REMODIFY, Keyword::kw_Remodify },
   { STRING_RENAME, Keyword::kw_Rename },
   { STRING_SELECT, Keyword::kw_Select },
   { STRING_UNHIDE, Keyword::kw_Unhide },
   { STRING_UNLOCK, Keyword::kw_Unlock },
   // core command flags:
   { STRING_FORCE, Keyword::kw_Force },
   { STRING_STACK, Keyword::kw_Stack },
   // logic:
   { STRING_TRUE, Keyword::kw_True },
   { STRING_FALSE, Keyword::kw_False },
   { STRING_AND, Keyword::kw_And },
   { STRING_OR, Keyword::kw_Or },
   { STRING_XOR, Keyword::kw_Xor },
   { STRING_NOT, Keyword::kw_Not },
   // other commands:
   { STRING_PRINT, Keyword::kw_Print },
   { STRING_RUN, Keyword::kw_Run },
   { STRING_SLEEP, Keyword::kw_Sleep },
   // expression elements:
   { STRING_IN, Keyword::kw_In },
   { STRING_LIKE, Keyword::kw_Like },
   // command structs:
   { STRING_ELSE, Keyword::kw_Else },
   { STRING_IF, Keyword::kw_If },
   { STRING_INSIDE, Keyword::kw_Inside },
   { STRING_TIMES, Keyword::kw_Times },
   { STRING_WHILE, Keyword::kw_While },
   // filthers:
   { STRING_EVERY, Keyword::kw_Every },
   { STRING_FINAL, Keyword::kw_Final },
   { STRING_LIMIT, Keyword::kw_Limit },
   { STRING_ORDER, Keyword::kw_Order },
   { STRING_SKIP, Keyword::kw_Skip },
   { STRING_WHERE, Keyword::kw_Where },
   // rest:
   { STRING_AS, Keyword::kw_As },
   { STRING_BY, Keyword::kw_By },
   { STRING_TO, Keyword::kw_To },
   { STRING_EXTENSIONLESS, Keyword::kw_Extensionless },
   { STRING_WITH, Keyword::kw_With },
   // order:
   { STRING_ASC, Keyword::kw_Asc },
   { STRING_DESC, Keyword::kw_Desc },
   // one-word command:
   { STRING_BREAK, Keyword::kw_Break },
   { STRING_CONTINUE, Keyword::kw_Continue },
   { STRING_EXIT, Keyword::kw_Exit },
   { STRING_ERROR, Keyword::kw_Error }
}),

MONTHS({
   { STRING_JANUARY,    TNUM_JANUARY },
   { STRING_FEBRUARY,   TNUM_FEBRUARY },
   { STRING_MARCH,      TNUM_MARCH },
   { STRING_APRIL,      TNUM_APRIL },
   { STRING_MAY,        TNUM_MAY },
   { STRING_JUNE,       TNUM_JUNE },
   { STRING_JULY,       TNUM_JULY },
   { STRING_AUGUST,     TNUM_AUGUST },
   { STRING_SEPTEMBER,  TNUM_SEPTEMBER },
   { STRING_OCTOBER,    TNUM_OCTOBER },
   { STRING_NOVEMBER,   TNUM_NOVEMBER },
   { STRING_DECEMBER,   TNUM_DECEMBER }
}),

WEEKDAYS({
   { STRING_MONDAY,     TNUM_MONDAY },
   { STRING_TUESDAY,    TNUM_TUESDAY },
   { STRING_WEDNESDAY,  TNUM_WEDNESDAY },
   { STRING_THURSDAY,   TNUM_THURSDAY },
   { STRING_FRIDAY,     TNUM_FRIDAY },
   { STRING_SATURDAY,   TNUM_SATURDAY },
   { STRING_SUNDAY,     TNUM_SUNDAY }
})

{ };

}
