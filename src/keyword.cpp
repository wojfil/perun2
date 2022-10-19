/*
    This file is part of Uroboros.
    Uroboros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros. If not, see <http://www.gnu.org/licenses/>.
*/

#include "keyword.h"


namespace uro
{

KeywordsData::KeywordsData() : KEYWORDS( {
   // core commands:
   { L"copy", Keyword::kw_Copy },
   { L"create", Keyword::kw_Create },
   { L"delete", Keyword::kw_Delete },
   { L"download", Keyword::kw_Download },
   { L"drop", Keyword::kw_Drop },
   { L"hide", Keyword::kw_Hide },
   { L"lock", Keyword::kw_Lock },
   { L"move", Keyword::kw_Move },
   { L"open", Keyword::kw_Open },
   { L"reaccess", Keyword::kw_Reaccess },
   { L"recreate", Keyword::kw_Recreate },
   { L"rechange", Keyword::kw_Rechange },
   { L"remodify", Keyword::kw_Remodify },
   { L"rename", Keyword::kw_Rename },
   { L"select", Keyword::kw_Select },
   { L"unhide", Keyword::kw_Unhide },
   { L"unlock", Keyword::kw_Unlock },
   // core command flags:
   { L"force", Keyword::kw_Force },
   { L"stack", Keyword::kw_Stack },
   // logic:
   { L"true", Keyword::kw_True },
   { L"false", Keyword::kw_False },
   { L"and", Keyword::kw_And },
   { L"or", Keyword::kw_Or },
   { L"xor", Keyword::kw_Xor },
   { L"not", Keyword::kw_Not },
   // other commands:
   { L"print", Keyword::kw_Print },
   { L"run", Keyword::kw_Run },
   { L"sleep", Keyword::kw_Sleep },
   // expression elements:
   { L"in", Keyword::kw_In },
   { L"like", Keyword::kw_Like },
   // command structs:
   { L"else", Keyword::kw_Else },
   { L"if", Keyword::kw_If },
   { L"inside", Keyword::kw_Inside },
   { L"times", Keyword::kw_Times },
   { L"while", Keyword::kw_While },
   // filthers:
   { L"every", Keyword::kw_Every },
   { L"final", Keyword::kw_Final },
   { L"limit", Keyword::kw_Limit },
   { L"order", Keyword::kw_Order },
   { L"skip", Keyword::kw_Skip },
   { L"where", Keyword::kw_Where },
   // rest:
   { L"as", Keyword::kw_As },
   { L"by", Keyword::kw_By },
   { L"from", Keyword::kw_From },
   { L"to", Keyword::kw_To },
   { L"extensionless", Keyword::kw_Extensionless },
   { L"with", Keyword::kw_With },
   // order:
   { L"asc", Keyword::kw_Asc },
   { L"desc", Keyword::kw_Desc },
   // one-word command:
   { L"break", Keyword::kw_Break },
   { L"continue", Keyword::kw_Continue },
   { L"exit", Keyword::kw_Exit },
   { L"error", Keyword::kw_Error }
}) { };

}
