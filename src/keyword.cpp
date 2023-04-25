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
   { p2.hashes.HASH_COPY, Keyword::kw_Copy },
   { p2.hashes.HASH_CREATE, Keyword::kw_Create },
   { p2.hashes.HASH_CREATEFILE, Keyword::kw_CreateFile },
   { p2.hashes.HASH_CREATEDIRECTORY, Keyword::kw_CreateDirectory },
   { p2.hashes.HASH_CREATEFILES, Keyword::kw_CreateFiles },
   { p2.hashes.HASH_CREATEDIRECTORIES, Keyword::kw_CreateDirectories },
   { p2.hashes.HASH_DELETE, Keyword::kw_Delete },
   { p2.hashes.HASH_DROP, Keyword::kw_Drop },
   { p2.hashes.HASH_HIDE, Keyword::kw_Hide },
   { p2.hashes.HASH_LOCK, Keyword::kw_Lock },
   { p2.hashes.HASH_MOVE, Keyword::kw_Move },
   { p2.hashes.HASH_OPEN, Keyword::kw_Open },
   { p2.hashes.HASH_REACCESS, Keyword::kw_Reaccess },
   { p2.hashes.HASH_RECREATE, Keyword::kw_Recreate },
   { p2.hashes.HASH_RECHANGE, Keyword::kw_Rechange },
   { p2.hashes.HASH_REMODIFY, Keyword::kw_Remodify },
   { p2.hashes.HASH_RENAME, Keyword::kw_Rename },
   { p2.hashes.HASH_SELECT, Keyword::kw_Select },
   { p2.hashes.HASH_UNHIDE, Keyword::kw_Unhide },
   { p2.hashes.HASH_UNLOCK, Keyword::kw_Unlock },
   // core command flags:
   { p2.hashes.HASH_FORCE, Keyword::kw_Force },
   { p2.hashes.HASH_STACK, Keyword::kw_Stack },
   // logic:
   { p2.hashes.HASH_TRUE, Keyword::kw_True },
   { p2.hashes.HASH_FALSE, Keyword::kw_False },
   { p2.hashes.HASH_AND, Keyword::kw_And },
   { p2.hashes.HASH_OR, Keyword::kw_Or },
   { p2.hashes.HASH_XOR, Keyword::kw_Xor },
   { p2.hashes.HASH_NOT, Keyword::kw_Not },
   // other commands:
   { p2.hashes.HASH_PRINT, Keyword::kw_Print },
   { p2.hashes.HASH_RUN, Keyword::kw_Run },
   { p2.hashes.HASH_SLEEP, Keyword::kw_Sleep },
   // expression elements:
   { p2.hashes.HASH_IN, Keyword::kw_In },
   { p2.hashes.HASH_LIKE, Keyword::kw_Like },
   // command structs:
   { p2.hashes.HASH_ELSE, Keyword::kw_Else },
   { p2.hashes.HASH_IF, Keyword::kw_If },
   { p2.hashes.HASH_INSIDE, Keyword::kw_Inside },
   { p2.hashes.HASH_TIMES, Keyword::kw_Times },
   { p2.hashes.HASH_WHILE, Keyword::kw_While },
   // filthers:
   { p2.hashes.HASH_EVERY, Keyword::kw_Every },
   { p2.hashes.HASH_FINAL, Keyword::kw_Final },
   { p2.hashes.HASH_LIMIT, Keyword::kw_Limit },
   { p2.hashes.HASH_ORDER, Keyword::kw_Order },
   { p2.hashes.HASH_SKIP, Keyword::kw_Skip },
   { p2.hashes.HASH_WHERE, Keyword::kw_Where },
   // rest:
   { p2.hashes.HASH_AS, Keyword::kw_As },
   { p2.hashes.HASH_BY, Keyword::kw_By },
   { p2.hashes.HASH_TO, Keyword::kw_To },
   { p2.hashes.HASH_EXTENSIONLESS, Keyword::kw_Extensionless },
   { p2.hashes.HASH_WITH, Keyword::kw_With },
   // order:
   { p2.hashes.HASH_ASC, Keyword::kw_Asc },
   { p2.hashes.HASH_DESC, Keyword::kw_Desc },
   // one-word command:
   { p2.hashes.HASH_BREAK, Keyword::kw_Break },
   { p2.hashes.HASH_CONTINUE, Keyword::kw_Continue },
   { p2.hashes.HASH_EXIT, Keyword::kw_Exit },
   { p2.hashes.HASH_ERROR, Keyword::kw_Error }
}) { };

}
