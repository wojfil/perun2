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

#include "programs-data.h"
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
      if (this->takeValueBeforeLastComma(tk, this->r_2, L"applicationicon")) {
         return;
      }
   }

   while (this->r_3->hasNext()) {
      if (this->takeValueBeforeLastComma(tk, this->r_3, L"applicationicon")) {
         return;
      }
   }

   while (this->r_4->hasNext()) {
      if (this->takeValueFirstArg(tk, this->r_4, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_5->hasNext()) {
      if (this->takeValueBeforeLastComma(tk, this->r_5, EMPTY_STRING)) {
         return;
      }
   }
};


}
