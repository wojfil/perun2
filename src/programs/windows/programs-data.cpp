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


WP_7zip::WP_7zip(p_perun2& p2) : WinProgram(p2, { L"7zip" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, L"software/microsoft/windows/currentversion/uninstall/7-zip");
};


void WP_7zip::actualize(const Token& tk)
{
   while (this->r_1->hasNext()) {
      if (this->takeValue(tk, this->r_1, L"displayicon")) {
         return;
      }
   }
};


WP_Acrobat::WP_Acrobat(p_perun2& p2) : WinProgram(p2, { L"acrobat", L"acrobatreader", L"adobeacrobat", L"adobeacrobatreader" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, L"software/adobe/adobe acrobat/*/installer");
   addRegistryPattern(this->r_2, RegistryRootType::LocalMachine, L"software/classes/acrobat.*/shell/open/command");
   addRegistryPattern(this->r_3, RegistryRootType::LocalMachine, L"software/classes/acrobat/shell/open/command");
};


void WP_Acrobat::actualize(const Token& tk)
{
   while (this->r_1->hasNext()) {
      if (this->takeValue(tk, this->r_1, L"acrobat.exe")) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValueFirstArg(tk, this->r_2, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_3->hasNext()) {
      if (this->takeValueFirstArg(tk, this->r_3, EMPTY_STRING)) {
         return;
      }
   }
};


WP_Audacity::WP_Audacity(p_perun2& p2) : WinProgram(p2, { L"audacity" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, L"software/microsoft/windows/currentversion/uninstall/audacity*");
   addRegistryPattern(this->r_2, RegistryRootType::ClassesRoot, L"audacity*/shell/open/command");
};


void WP_Audacity::actualize(const Token& tk)
{
   while (this->r_1->hasNext()) {
      if (this->takeValue(tk, this->r_1, L"displayicon")) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValueFirstArg(tk, this->r_2, EMPTY_STRING)) {
         return;
      }
   }
};


WP_Firefox::WP_Firefox(p_perun2& p2) : WinProgram(p2, { L"firefox", L"mozillafirefox" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, L"software/microsoft/windows/currentversion/uninstall/mozilla firefox*");
   addRegistryPattern(this->r_2, RegistryRootType::LocalMachine, L"software/mozilla/mozilla firefox #/bin");
   addRegistryPattern(this->r_3, RegistryRootType::LocalMachine, L"software/mozilla/mozilla firefox/*/main");
   addRegistryPattern(this->r_4, RegistryRootType::CurrentUser, L"software/mozilla/mozilla firefox/*/main");
   addRegistryPattern(this->r_5, RegistryRootType::CurrentUser, L"software/mozilla/mozilla firefox */bin");
};


void WP_Firefox::actualize(const Token& tk)
{
   while (this->r_1->hasNext()) {
      if (this->takeValueBeforeLastComma(tk, this->r_1, L"displayicon")) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValue(tk, this->r_2, L"pathtoexe")) {
         return;
      }
   }

   while (this->r_3->hasNext()) {
      if (this->takeValue(tk, this->r_3, L"pathtoexe")) {
         return;
      }
   }

   while (this->r_4->hasNext()) {
      if (this->takeValue(tk, this->r_4, L"pathtoexe")) {
         return;
      }
   }

   while (this->r_5->hasNext()) {
      if (this->takeValue(tk, this->r_5, L"pathtoexe")) {
         return;
      }
   }
};


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


WP_NotepadPlusPlus::WP_NotepadPlusPlus(p_perun2& p2) : WinProgram(p2, { L"notepadplusplus" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, L"software/microsoft/windows/currentversion/uninstall/notepad++");
};


void WP_NotepadPlusPlus::actualize(const Token& tk)
{
   while (this->r_1->hasNext()) {
      if (this->takeValue(tk, this->r_1, L"displayicon")) {
         return;
      }
   }
};


WP_OpenOffice::WP_OpenOffice(p_perun2& p2) : WinProgram(p2, { L"openoffice" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, L"software/wow6432node/openoffice/openoffice/*");
   addRegistryPattern(this->r_2, RegistryRootType::ClassesRoot, L"openoffice*/defaulticon");
};


void WP_OpenOffice::actualize(const Token& tk)
{
   while (this->r_1->hasNext()) {
      if (this->takeValue(tk, this->r_1, L"path")) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValueBeforeLastComma(tk, this->r_2, EMPTY_STRING)) {
         return;
      }
   }
};


WP_Vlc::WP_Vlc(p_perun2& p2) : WinProgram(p2, { L"vlc", L"vlcmediaplayer" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, L"software/microsoft/windows/currentversion/uninstall/vlc media player");
   addRegistryPattern(this->r_2, RegistryRootType::ClassesRoot, L"vlc*/defaulticon");
   addRegistryPattern(this->r_3, RegistryRootType::ClassesRoot, L"vlc*/shell/open/command");
   addRegistryPattern(this->r_4, RegistryRootType::ClassesRoot, L"vlc*/shell/playwithvlc/command");
};


void WP_Vlc::actualize(const Token& tk)
{
   while (this->r_1->hasNext()) {
      if (this->takeValue(tk, this->r_1, L"displayicon")) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValueFirstArg(tk, this->r_2, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_3->hasNext()) {
      if (this->takeValueFirstArg(tk, this->r_3, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_4->hasNext()) {
      if (this->takeValueFirstArg(tk, this->r_4, EMPTY_STRING)) {
         return;
      }
   }
};


WP_Word::WP_Word(p_perun2& p2) : WinProgram(p2, { L"word", L"msword", L"microsoftword" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, L"software/classes/wordmhtmlfile/defaulticon");
   addRegistryPattern(this->r_2, RegistryRootType::LocalMachine, L"software/classes/wordhtmltemplate/shell/open/command");
   addRegistryPattern(this->r_3, RegistryRootType::LocalMachine, L"software/classes/wordhtmlfile/shell/open/command");
};


void WP_Word::actualize(const Token& tk)
{
   while (this->r_1->hasNext()) {
      if (this->takeValueBeforeLastComma(tk, this->r_1, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValueFirstArg(tk, this->r_2, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_3->hasNext()) {
      if (this->takeValueFirstArg(tk, this->r_3, EMPTY_STRING)) {
         return;
      }
   }
};


}
