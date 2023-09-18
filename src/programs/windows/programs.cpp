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

#include "programs.h"
#include "programs-data.h"
#include "../../os.h"
#include "../../perun2.h"


namespace perun2::prog
{


WinProgram::WinProgram(p_perun2& p2, const p_list& nm) 
   : names(nm), perun2(p2), context(p2.contexts.globalVars) { };


p_bool WinProgram::isIcon(const p_str& value) const
{
   if (value.length() < 4) {
      return false;
   }

   const p_str iconSuffix = L".ico";
   return value.substr(value.length() - iconSuffix.length()) == iconSuffix;
};


p_str WinProgram::beforeLastComma(const p_str& value) const
{
   if (value.empty()) {
      return value;
   }

   for (p_int i = value.length() - 1; i >= 0; i--) {
      switch (value[i]) {
         case CHAR_SLASH:
         case CHAR_BACKSLASH: {
            return value;
         }
         case CHAR_COMMA: {
            return value.substr(0, i);
         }
      }

   }

   return value;
};


p_str WinProgram::firstArg(const p_str& value) const
{
   if (value.empty()) {
      return value;
   }

   p_bool started = false;
   p_size start = 0;

   for (p_size i = 0; i < value.size(); i++) {
      if (value[i] == CHAR_QUOTATION_MARK) {
         if (started) {
            return value.substr(start + 1, i - start - 1);
         }
         else {
            start = i;
            started = true;
         }
      }
   }

   return EMPTY_STRING;
};


p_bool WinProgram::saveValue(const Token& tk, const p_str& value)
{
   if (value.empty() || this->isIcon(value)) {
      return false;
   }

   if (! os_fileExists(value)) {
      return false;
   }

   p_str name = tk.getOriginString(this->perun2);
   toLower(name);
   this->context.strings[name]->value = value;
   return true;
};


p_bool WinProgram::takeValue(const Token& tk, p_riptr& registry, const p_str& name)
{
   const p_str v = registry->getRegistryValue(name);
   return this->saveValue(tk, v);
}


p_bool WinProgram::takeValueFirstArg(const Token& tk, p_riptr& registry, const p_str& name)
{
   const p_str v = registry->getRegistryValue(name);
   const p_str first = this->firstArg(v);
   return this->saveValue(tk, first);
}


p_bool WinProgram::takeValueBeforeLastComma(const Token& tk, p_riptr& registry, const p_str& name)
{
   const p_str v = registry->getRegistryValue(name);
   const p_str before = this->beforeLastComma(v);
   return this->saveValue(tk, before);
}


WinPrograms::WinPrograms(p_perun2& p2) : perun2(p2)
{ 
   this->programs.emplace_back(std::make_unique<WP_7zip>(p2));
   this->programs.emplace_back(std::make_unique<WP_Acrobat>(p2));
   this->programs.emplace_back(std::make_unique<WP_Audacity>(p2));
   this->programs.emplace_back(std::make_unique<WP_Firefox>(p2));
   this->programs.emplace_back(std::make_unique<WP_Gimp>(p2));
   this->programs.emplace_back(std::make_unique<WP_Notepad>(p2));
   this->programs.emplace_back(std::make_unique<WP_NotepadPlusPlus>(p2));
   this->programs.emplace_back(std::make_unique<WP_OpenOffice>(p2));
   this->programs.emplace_back(std::make_unique<WP_Paint>(p2));
   this->programs.emplace_back(std::make_unique<WP_Vlc>(p2));
   this->programs.emplace_back(std::make_unique<WP_Word>(p2));
};


void WinPrograms::actualize(const Token& tk)
{
   for (auto& program : this->programs) {
      if (tk.isWord(program->names, this->perun2)) {
         program->actualize(tk);
         return;
      }
   }
}


p_bool WinPrograms::haveName(const Token& tk) const
{
   for (const auto& program : this->programs) {
      if (tk.isWord(program->names, this->perun2)) {
         return true;
      }
   }

   return false;
}


void WinPrograms::insertVars(GlobalContext& ctx) const
{
   for (const auto& program : this->programs) {
      for (const p_str& name : program->names) {
         ctx.insertConstant<p_str>(name);
      }
   }
}


}
