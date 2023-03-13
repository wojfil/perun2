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

KeywordsData::KeywordsData(_p2& p2) : KEYWORDS( {
   // core commands:
   { p2.hashes.HASH_KW_COPY, Keyword::kw_Copy },
   { p2.hashes.HASH_KW_CREATE, Keyword::kw_Create },
   { p2.hashes.HASH_KW_CREATEFILE, Keyword::kw_CreateFile },
   { p2.hashes.HASH_KW_CREATEDIRECTORY, Keyword::kw_CreateDirectory },
   { p2.hashes.HASH_KW_CREATEFILES, Keyword::kw_CreateFiles },
   { p2.hashes.HASH_KW_CREATEDIRECTORIES, Keyword::kw_CreateDirectories },
   { p2.hashes.HASH_KW_DELETE, Keyword::kw_Delete },
   { p2.hashes.HASH_KW_DROP, Keyword::kw_Drop },
   { p2.hashes.HASH_KW_HIDE, Keyword::kw_Hide },
   { p2.hashes.HASH_KW_LOCK, Keyword::kw_Lock },
   { p2.hashes.HASH_KW_MOVE, Keyword::kw_Move },
   { p2.hashes.HASH_KW_OPEN, Keyword::kw_Open },
   { p2.hashes.HASH_KW_REACCESS, Keyword::kw_Reaccess },
   { p2.hashes.HASH_KW_RECREATE, Keyword::kw_Recreate },
   { p2.hashes.HASH_KW_RECHANGE, Keyword::kw_Rechange },
   { p2.hashes.HASH_KW_REMODIFY, Keyword::kw_Remodify },
   { p2.hashes.HASH_KW_RENAME, Keyword::kw_Rename },
   { p2.hashes.HASH_KW_SELECT, Keyword::kw_Select },
   { p2.hashes.HASH_KW_UNHIDE, Keyword::kw_Unhide },
   { p2.hashes.HASH_KW_UNLOCK, Keyword::kw_Unlock },
   // core command flags:
   { p2.hashes.HASH_KW_FORCE, Keyword::kw_Force },
   { p2.hashes.HASH_KW_STACK, Keyword::kw_Stack },
   // logic:
   { p2.hashes.HASH_KW_TRUE, Keyword::kw_True },
   { p2.hashes.HASH_KW_FALSE, Keyword::kw_False },
   { p2.hashes.HASH_KW_AND, Keyword::kw_And },
   { p2.hashes.HASH_KW_OR, Keyword::kw_Or },
   { p2.hashes.HASH_KW_XOR, Keyword::kw_Xor },
   { p2.hashes.HASH_KW_NOT, Keyword::kw_Not },
   // other commands:
   { p2.hashes.HASH_KW_PRINT, Keyword::kw_Print },
   { p2.hashes.HASH_KW_RUN, Keyword::kw_Run },
   { p2.hashes.HASH_KW_SLEEP, Keyword::kw_Sleep },
   // expression elements:
   { p2.hashes.HASH_KW_IN, Keyword::kw_In },
   { p2.hashes.HASH_KW_LIKE, Keyword::kw_Like },
   // command structs:
   { p2.hashes.HASH_KW_ELSE, Keyword::kw_Else },
   { p2.hashes.HASH_KW_IF, Keyword::kw_If },
   { p2.hashes.HASH_KW_INSIDE, Keyword::kw_Inside },
   { p2.hashes.HASH_KW_TIMES, Keyword::kw_Times },
   { p2.hashes.HASH_KW_WHILE, Keyword::kw_While },
   // filthers:
   { p2.hashes.HASH_KW_EVERY, Keyword::kw_Every },
   { p2.hashes.HASH_KW_FINAL, Keyword::kw_Final },
   { p2.hashes.HASH_KW_LIMIT, Keyword::kw_Limit },
   { p2.hashes.HASH_KW_ORDER, Keyword::kw_Order },
   { p2.hashes.HASH_KW_SKIP, Keyword::kw_Skip },
   { p2.hashes.HASH_KW_WHERE, Keyword::kw_Where },
   // rest:
   { p2.hashes.HASH_KW_AS, Keyword::kw_As },
   { p2.hashes.HASH_KW_BY, Keyword::kw_By },
   { p2.hashes.HASH_KW_TO, Keyword::kw_To },
   { p2.hashes.HASH_KW_EXTENSIONLESS, Keyword::kw_Extensionless },
   { p2.hashes.HASH_KW_WITH, Keyword::kw_With },
   // order:
   { p2.hashes.HASH_KW_ASC, Keyword::kw_Asc },
   { p2.hashes.HASH_KW_DESC, Keyword::kw_Desc },
   // one-word command:
   { p2.hashes.HASH_KW_BREAK, Keyword::kw_Break },
   { p2.hashes.HASH_KW_CONTINUE, Keyword::kw_Continue },
   { p2.hashes.HASH_KW_EXIT, Keyword::kw_Exit },
   { p2.hashes.HASH_KW_ERROR, Keyword::kw_Error }
}) { };

}
