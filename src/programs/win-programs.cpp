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

#include "win-programs.h"
#include "../os.h"
#include "../perun2.h"


namespace perun2::prog
{

p_list splitRegistryPath(const p_str& value)
{
   p_list result;
   p_str token;
   
   for (const p_char c : value) {
      if (c == CHAR_SLASH || c == CHAR_BACKSLASH) {
         if (!token.empty()) {
            result.push_back(token);
            token.clear();
         }
      } 
      else {
         token += c;
      }
   }

   result.push_back(token);
   return result;
};


void addRegistryPattern(p_riptr& result, const RegistryRootType rootType, const p_str& pattern)
{
   const p_list segments = splitRegistryPath(pattern);
   p_str token;
   
   for (const p_str& sg : segments) {
      if (sg.find(CHAR_ASTERISK) != p_str::npos || sg.find(CHAR_HASH) != p_str::npos) {
         if (! token.empty()) {
            if (result) {
               p_riptr prev = std::move(result);
               result = std::make_unique<SingleRegistry>(rootType, prev, token);
            }
            else {
               result = std::make_unique<SingleRegistryRoot>(rootType, token);
            }

            token.clear();
         }

         if (result) {
            p_riptr prev = std::move(result);
            result = std::make_unique<MultiRegistry>(rootType, prev, sg);
         }
         else {
            result = std::make_unique<MultiRegistryRoot>(rootType, sg);
         }
      }
      else {
         if (token.empty()) {
            token = sg;
         }
         else {
            token = str(token, CHAR_BACKSLASH, sg);
         }
      }
   }

   if (! token.empty()) {
      if (result) {
         p_riptr prev = std::move(result);
         result = std::make_unique<SingleRegistry>(rootType, prev, token);
      }
      else {
         result = std::make_unique<SingleRegistryRoot>(rootType, token);
      }
   }
};


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


WinPrograms::WinPrograms(p_perun2& p2) : perun2(p2)
{ 
   this->programs.emplace_back(std::make_unique<WP_Gimp>(p2));
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


void WinPrograms::insertVars(GlobalContext& ctx)
{
   for (const auto& program : this->programs) {
      for (const p_str& name : program->names) {
         ctx.insertConstant<p_str>(name);
      }
   }
}


WP_Gimp::WP_Gimp(p_perun2& p2) : WinProgram(p2, { L"gimp" }) 
{

   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, L"software/gimp #/capabilities");
   addRegistryPattern(this->r_2, RegistryRootType::ClassesRoot, L"gimp*/defaulticon");
};

void WP_Gimp::actualize(const Token& tk)
{
   while (this->r_1->hasNext()) {
      const p_str value = this->r_1->getRegistryValue(L"applicationicon");
      const p_str before = this->beforeLastComma(value);

      if (this->saveValue(tk, before)) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      const p_str value = this->r_2->getRegistryValue(EMPTY_STRING);
      const p_str before = this->beforeLastComma(value);

      if (this->saveValue(tk, before)) {
         return;
      }
   }
};


}
