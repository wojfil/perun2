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

#include "win-programs.h"
#include "programs-data.h"
#include "../../os/os.h"
#include "../../perun2.h"
#include "../../datatype/text/text-parsing.h"


namespace perun2::prog
{


WinProgram::WinProgram(p_perun2& p2, const p_list& nm) 
   : names(nm), perun2(p2), context(p2.contexts.globalVars) { };


p_bool WinProgram::isIcon(const p_str& value) const
{
   if (value.length() < STRING_ICON_SUFFIX_LEN) {
      return false;
   }

   return value.substr(value.length() - STRING_ICON_SUFFIX_LEN) == STRING_ICON_SUFFIX;
};


p_bool WinProgram::saveValue(const p_str& value)
{
   if (value.empty() || this->isIcon(value)) {
      return false;
   }

   if (! os_fileExists(value)) {
      return false;
   }

   for (const p_str& name : this->names) {
      this->context.strings[name]->value = value;
   }

   return true;
};


p_bool WinProgram::takeValue(p_riptr& registry, const p_str& name)
{
   const p_str v = registry->getRegistryValue(name);
   return this->saveValue(v);
}


p_bool WinProgram::takeValueFirstArg(p_riptr& registry, const p_str& name)
{
   const p_str v = registry->getRegistryValue(name);
   const p_str first = str_firstArg(v);
   return this->saveValue(first);
}


p_bool WinProgram::takeValueBeforeLastComma(p_riptr& registry, const p_str& name)
{
   const p_str v = registry->getRegistryValue(name);
   const p_str before = str_beforeLastComma(v);
   return this->saveValue(before);
}


p_bool WinProgram::takeStartMenuLink(StartMenuLnk& link)
{
   if (! link.hasValue()) {
      return false;
   }

   return this->saveValue(link.getValue());
}


WindowsPrograms::WindowsPrograms(p_perun2& p2) : perun2(p2)
{ 
   this->programs.emplace_back(std::make_unique<WP_7zip>(p2));
   this->programs.emplace_back(std::make_unique<WP_Acrobat>(p2));
   this->programs.emplace_back(std::make_unique<WP_Audacity>(p2));
   this->programs.emplace_back(std::make_unique<WP_Firefox>(p2));
   this->programs.emplace_back(std::make_unique<WP_Gimp>(p2));
   this->programs.emplace_back(std::make_unique<WP_Inkscape>(p2));
   this->programs.emplace_back(std::make_unique<WP_Notepad>(p2));
   this->programs.emplace_back(std::make_unique<WP_NotepadPlusPlus>(p2));
   this->programs.emplace_back(std::make_unique<WP_OpenOffice>(p2));
   this->programs.emplace_back(std::make_unique<WP_Paint>(p2));
   this->programs.emplace_back(std::make_unique<WP_Photoshop>(p2));
   this->programs.emplace_back(std::make_unique<WP_Sumatra>(p2));
   this->programs.emplace_back(std::make_unique<WP_Vlc>(p2));
   this->programs.emplace_back(std::make_unique<WP_WinRAR>(p2));
   this->programs.emplace_back(std::make_unique<WP_Word>(p2));
};


void WindowsPrograms::actualize(const Token& tk)
{
   for (auto& program : this->programs) {
      if (tk.isWord(program->names, this->perun2)) {
         if (! program->loaded) {
            program->actualize();
            program->loaded = true;
         }

         return;
      }
   }
}


p_bool WindowsPrograms::haveName(const Token& tk) const
{
   for (const auto& program : this->programs) {
      if (tk.isWord(program->names, this->perun2)) {
         return true;
      }
   }

   return false;
}


void WindowsPrograms::insertVars(GlobalContext& ctx) const
{
   for (const auto& program : this->programs) {
      for (const p_str& name : program->names) {
         ctx.insertConstant<p_str>(name);
      }
   }
}


}
