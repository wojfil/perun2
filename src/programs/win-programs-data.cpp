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

#include "win-programs-data.h"
#include "registry.h"


namespace perun2::prog
{


WP_Gimp::WP_Gimp(p_perun2& p2) : WinProgram(p2, { L"gimp" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, L"software/microsoft/windows/currentversion/uninstall/gimp*");
   addRegistryPattern(this->r_2, RegistryRootType::CurrentUser, L"software/gimp #/capabilities");
   addRegistryPattern(this->r_3, RegistryRootType::LocalMachine, L"software/gimp #/capabilities");
   addRegistryPattern(this->r_4, RegistryRootType::ClassesRoot, L"gimp*/shell/open/command");
   addRegistryPattern(this->r_5, RegistryRootType::ClassesRoot, L"gimp*/defaulticon");
};


void WP_Gimp::actualize(const Token& tk)
{
   while (this->r_1->hasNext()) {
      if (this->takeValue(tk, this->r_1, L"displayicon")) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      const p_str value = this->r_2->getRegistryValue(L"applicationicon");
      const p_str before = this->beforeLastComma(value);

      if (this->saveValue(tk, before)) {
         return;
      }
   }

   while (this->r_3->hasNext()) {
      const p_str value = this->r_3->getRegistryValue(L"applicationicon");
      const p_str before = this->beforeLastComma(value);

      if (this->saveValue(tk, before)) {
         return;
      }
   }

   while (this->r_4->hasNext()) {
      const p_str value = this->r_4->getRegistryValue(EMPTY_STRING);
      const p_str first = this->firstArg(value);

      if (this->saveValue(tk, first)) {
         return;
      }
   }

   while (this->r_5->hasNext()) {
      const p_str value = this->r_5->getRegistryValue(EMPTY_STRING);
      const p_str before = this->beforeLastComma(value);

      if (this->saveValue(tk, before)) {
         return;
      }
   }
};


}
