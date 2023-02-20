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

#include "keyword.h"
#include "datatype/strings.h"
#include "uroboros.h"


namespace uro
{

KeywordsData::KeywordsData(_uro& uro) : KEYWORDS( {
   // core commands:
   { uro.hashes.HASH_KW_COPY, Keyword::kw_Copy },
   { uro.hashes.HASH_KW_CREATE, Keyword::kw_Create },
   { uro.hashes.HASH_KW_DELETE, Keyword::kw_Delete },
   { uro.hashes.HASH_KW_DOWNLOAD, Keyword::kw_Download },
   { uro.hashes.HASH_KW_DROP, Keyword::kw_Drop },
   { uro.hashes.HASH_KW_HIDE, Keyword::kw_Hide },
   { uro.hashes.HASH_KW_LOCK, Keyword::kw_Lock },
   { uro.hashes.HASH_KW_MOVE, Keyword::kw_Move },
   { uro.hashes.HASH_KW_OPEN, Keyword::kw_Open },
   { uro.hashes.HASH_KW_REACCESS, Keyword::kw_Reaccess },
   { uro.hashes.HASH_KW_RECREATE, Keyword::kw_Recreate },
   { uro.hashes.HASH_KW_RECHANGE, Keyword::kw_Rechange },
   { uro.hashes.HASH_KW_REMODIFY, Keyword::kw_Remodify },
   { uro.hashes.HASH_KW_RENAME, Keyword::kw_Rename },
   { uro.hashes.HASH_KW_SELECT, Keyword::kw_Select },
   { uro.hashes.HASH_KW_UNHIDE, Keyword::kw_Unhide },
   { uro.hashes.HASH_KW_UNLOCK, Keyword::kw_Unlock },
   // core command flags:
   { uro.hashes.HASH_KW_FORCE, Keyword::kw_Force },
   { uro.hashes.HASH_KW_STACK, Keyword::kw_Stack },
   // logic:
   { uro.hashes.HASH_KW_TRUE, Keyword::kw_True },
   { uro.hashes.HASH_KW_FALSE, Keyword::kw_False },
   { uro.hashes.HASH_KW_AND, Keyword::kw_And },
   { uro.hashes.HASH_KW_OR, Keyword::kw_Or },
   { uro.hashes.HASH_KW_XOR, Keyword::kw_Xor },
   { uro.hashes.HASH_KW_NOT, Keyword::kw_Not },
   // other commands:
   { uro.hashes.HASH_KW_PRINT, Keyword::kw_Print },
   { uro.hashes.HASH_KW_RUN, Keyword::kw_Run },
   { uro.hashes.HASH_KW_SLEEP, Keyword::kw_Sleep },
   // expression elements:
   { uro.hashes.HASH_KW_IN, Keyword::kw_In },
   { uro.hashes.HASH_KW_LIKE, Keyword::kw_Like },
   // command structs:
   { uro.hashes.HASH_KW_ELSE, Keyword::kw_Else },
   { uro.hashes.HASH_KW_IF, Keyword::kw_If },
   { uro.hashes.HASH_KW_INSIDE, Keyword::kw_Inside },
   { uro.hashes.HASH_KW_TIMES, Keyword::kw_Times },
   { uro.hashes.HASH_KW_WHILE, Keyword::kw_While },
   // filthers:
   { uro.hashes.HASH_KW_EVERY, Keyword::kw_Every },
   { uro.hashes.HASH_KW_FINAL, Keyword::kw_Final },
   { uro.hashes.HASH_KW_LIMIT, Keyword::kw_Limit },
   { uro.hashes.HASH_KW_ORDER, Keyword::kw_Order },
   { uro.hashes.HASH_KW_SKIP, Keyword::kw_Skip },
   { uro.hashes.HASH_KW_WHERE, Keyword::kw_Where },
   // rest:
   { uro.hashes.HASH_KW_AS, Keyword::kw_As },
   { uro.hashes.HASH_KW_BY, Keyword::kw_By },
   { uro.hashes.HASH_KW_FROM, Keyword::kw_From },
   { uro.hashes.HASH_KW_TO, Keyword::kw_To },
   { uro.hashes.HASH_KW_EXTENSIONLESS, Keyword::kw_Extensionless },
   { uro.hashes.HASH_KW_WITH, Keyword::kw_With },
   // order:
   { uro.hashes.HASH_KW_ASC, Keyword::kw_Asc },
   { uro.hashes.HASH_KW_DESC, Keyword::kw_Desc },
   // one-word command:
   { uro.hashes.HASH_KW_BREAK, Keyword::kw_Break },
   { uro.hashes.HASH_KW_CONTINUE, Keyword::kw_Continue },
   { uro.hashes.HASH_KW_EXIT, Keyword::kw_Exit },
   { uro.hashes.HASH_KW_ERROR, Keyword::kw_Error }
}) { };

}
