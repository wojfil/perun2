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

#ifndef WIN_PROGRAMS_H_INCLUDED
#define WIN_PROGRAMS_H_INCLUDED

#include "registry.h"
#include "../token.h"


namespace perun2
{
   struct VarsContext;
   struct GlobalContext;
   struct p_perun2;
}

namespace perun2::prog
{


struct WinProgram
{
public:
   WinProgram(p_perun2& p2, const p_list& nm);
   virtual void actualize(const Token& tk) = 0;

   p_bool isIcon(const p_str& value) const;
   p_str beforeLastComma(const p_str& value) const;
   p_str firstArg(const p_str& value) const;
   p_bool saveValue(const Token& tk, const p_str& value);

   const p_list names;

private:
   p_perun2& perun2;
   VarsContext& context;
};


struct WinPrograms
{
public:
   WinPrograms(p_perun2& p2);
   void actualize(const Token& tk);
   p_bool haveName(const Token& tk) const;
   void insertVars(GlobalContext& ctx);

private:
   p_perun2& perun2;
   std::vector<std::unique_ptr<WinProgram>> programs;
};


}

#endif // WIN_PROGRAMS_H_INCLUDED
