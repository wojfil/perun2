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

#include "programs-data.h"
#include "registry.h"
#include "../../os/os.h"


namespace perun2::prog
{


WP_7zip::WP_7zip(Perun2Process& p2) : WinProgram(p2, { U"7zip" }),
   startMenuLink(U"7-zip\\7-zip file manager.lnk")
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, U"software/microsoft/windows/currentversion/uninstall/7-zip");
};


void WP_7zip::actualize()
{
   if (this->takeStartMenuLink(this->startMenuLink)) {
      return;
   }
   
   while (this->r_1->hasNext()) {
      if (this->takeValue(this->r_1, U"displayicon")) {
         return;
      }
   }
};


WP_Acrobat::WP_Acrobat(Perun2Process& p2) : WinProgram(p2, { U"acrobat", U"acrobatreader", U"adobeacrobat", U"adobeacrobatreader" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, U"software/adobe/adobe acrobat/*/installer");
   addRegistryPattern(this->r_2, RegistryRootType::LocalMachine, U"software/classes/acrobat.*/shell/open/command");
   addRegistryPattern(this->r_3, RegistryRootType::LocalMachine, U"software/classes/acrobat/shell/open/command");
};


void WP_Acrobat::actualize()
{
   while (this->r_1->hasNext()) {
      if (this->takeValue(this->r_1, U"acrobat.exe")) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValueFirstArg(this->r_2, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_3->hasNext()) {
      if (this->takeValueFirstArg(this->r_3, EMPTY_STRING)) {
         return;
      }
   }
};


WP_Audacity::WP_Audacity(Perun2Process& p2) : WinProgram(p2, { U"audacity" }),
   startMenuLink(U"audacity.lnk")
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, U"software/microsoft/windows/currentversion/uninstall/audacity*");
   addRegistryPattern(this->r_2, RegistryRootType::ClassesRoot, U"audacity*/shell/open/command");
};


void WP_Audacity::actualize()
{
   if (this->takeStartMenuLink(this->startMenuLink)) {
      return;
   }
   
   while (this->r_1->hasNext()) {
      if (this->takeValue(this->r_1, U"displayicon")) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValueFirstArg(this->r_2, EMPTY_STRING)) {
         return;
      }
   }
};


WP_Firefox::WP_Firefox(Perun2Process& p2) : WinProgram(p2, { U"firefox", U"mozillafirefox" }),
   startMenuLink(U"firefox.lnk")
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, U"software/microsoft/windows/currentversion/uninstall/mozilla firefox*");
   addRegistryPattern(this->r_2, RegistryRootType::LocalMachine, U"software/mozilla/mozilla firefox #/bin");
   addRegistryPattern(this->r_3, RegistryRootType::LocalMachine, U"software/mozilla/mozilla firefox/*/main");
   addRegistryPattern(this->r_4, RegistryRootType::CurrentUser, U"software/mozilla/mozilla firefox/*/main");
   addRegistryPattern(this->r_5, RegistryRootType::CurrentUser, U"software/mozilla/mozilla firefox */bin");
};


void WP_Firefox::actualize()
{
   if (this->takeStartMenuLink(this->startMenuLink)) {
      return;
   }
   
   while (this->r_1->hasNext()) {
      if (this->takeValueBeforeLastComma(this->r_1, U"displayicon")) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValue(this->r_2, U"pathtoexe")) {
         return;
      }
   }

   while (this->r_3->hasNext()) {
      if (this->takeValue(this->r_3, U"pathtoexe")) {
         return;
      }
   }

   while (this->r_4->hasNext()) {
      if (this->takeValue(this->r_4, U"pathtoexe")) {
         return;
      }
   }

   while (this->r_5->hasNext()) {
      if (this->takeValue(this->r_5, U"pathtoexe")) {
         return;
      }
   }
};


WP_Gimp::WP_Gimp(Perun2Process& p2) : WinProgram(p2, { U"gimp" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, U"software/microsoft/windows/currentversion/uninstall/gimp*");
   addRegistryPattern(this->r_2, RegistryRootType::CurrentUser, U"software/gimp #/capabilities");
   addRegistryPattern(this->r_3, RegistryRootType::LocalMachine, U"software/gimp #/capabilities");
   addRegistryPattern(this->r_4, RegistryRootType::ClassesRoot, U"gimp*/shell/open/command");
   addRegistryPattern(this->r_5, RegistryRootType::ClassesRoot, U"gimp*/defaulticon");
};


void WP_Gimp::actualize()
{
   while (this->r_1->hasNext()) {
      if (this->takeValue(this->r_1, U"displayicon")) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValueBeforeLastComma(this->r_2, U"applicationicon")) {
         return;
      }
   }

   while (this->r_3->hasNext()) {
      if (this->takeValueBeforeLastComma(this->r_3, U"applicationicon")) {
         return;
      }
   }

   while (this->r_4->hasNext()) {
      if (this->takeValueFirstArg(this->r_4, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_5->hasNext()) {
      if (this->takeValueBeforeLastComma(this->r_5, EMPTY_STRING)) {
         return;
      }
   }
};


WP_Inkscape::WP_Inkscape(Perun2Process& p2) : WinProgram(p2, { U"inkscape" }) ,
   startMenuLink(U"inkscape\\inkscape.lnk")
{
   addRegistryPattern(this->r_1, RegistryRootType::ClassesRoot, U"inkscape.*/shell/open/command");
};


void WP_Inkscape::actualize()
{
   if (this->takeStartMenuLink(this->startMenuLink)) {
      return;
   }
   
   while (this->r_1->hasNext()) {
      if (this->takeValueFirstArg(this->r_1, EMPTY_STRING)) {
         return;
      }
   }
};


WP_Notepad::WP_Notepad(Perun2Process& p2) : WinProgram(p2, { U"notepad" }) { };


void WP_Notepad::actualize()
{
   const p_str system32 = os_system32Path();
   if (! system32.empty()) {
      const p_str path = str(system32, OS_SEPARATOR, U"notepad.exe");
      this->saveValue(path);
   }
};


WP_NotepadPlusPlus::WP_NotepadPlusPlus(Perun2Process& p2) : WinProgram(p2, { U"notepadplusplus" }),
   startMenuLink(U"notepad++.lnk")
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, U"software/microsoft/windows/currentversion/uninstall/notepad++");
};


void WP_NotepadPlusPlus::actualize()
{
   if (this->takeStartMenuLink(this->startMenuLink)) {
      return;
   }

   while (this->r_1->hasNext()) {
      if (this->takeValue(this->r_1, U"displayicon")) {
         return;
      }
   }
};


WP_OpenOffice::WP_OpenOffice(Perun2Process& p2) : WinProgram(p2, { U"openoffice" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, U"software/wow6432node/openoffice/openoffice/*");
   addRegistryPattern(this->r_2, RegistryRootType::ClassesRoot, U"openoffice*/defaulticon");
};


void WP_OpenOffice::actualize()
{
   while (this->r_1->hasNext()) {
      if (this->takeValue(this->r_1, U"path")) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValueBeforeLastComma(this->r_2, EMPTY_STRING)) {
         return;
      }
   }
};


WP_Paint::WP_Paint(Perun2Process& p2) : WinProgram(p2, { U"paint", U"mspaint" }) { };


void WP_Paint::actualize()
{
   const p_str system32 = os_system32Path();
   if (! system32.empty()) {
      const p_str path = str(system32, OS_SEPARATOR, U"mspaint.exe");
      this->saveValue(path);
   }
};


WP_Photoshop::WP_Photoshop(Perun2Process& p2) : WinProgram(p2, { U"photoshop", U"adobephotoshop" }) 
{ 
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, U"software/classes/photoshop.*file*/defaulticon");
   addRegistryPattern(this->r_2, RegistryRootType::LocalMachine, U"software/classes/photoshop.*file*/shell/open/command");
   addRegistryPattern(this->r_3, RegistryRootType::ClassesRoot, U"photoshop.*file*/defaulticon");
   addRegistryPattern(this->r_4, RegistryRootType::ClassesRoot, U"photoshop.*file*/shell/open/command");
};


void WP_Photoshop::actualize()
{
   while (this->r_1->hasNext()) {
      if (this->takeValueBeforeLastComma(this->r_1, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValueFirstArg(this->r_2, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_3->hasNext()) {
      if (this->takeValueBeforeLastComma(this->r_3, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_4->hasNext()) {
      if (this->takeValueFirstArg(this->r_4, EMPTY_STRING)) {
         return;
      }
   }
};


WP_Sumatra::WP_Sumatra(Perun2Process& p2) : WinProgram(p2, { U"sumatra", U"sumatrapdf", U"sumatrapdfreader" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::CurrentUser, U"software/microsoft/windows/currentversion/uninstall/sumatrapdf");
   addRegistryPattern(this->r_2, RegistryRootType::ClassesRoot, U"sumatrapdf.*/shell/open/command");
};


void WP_Sumatra::actualize()
{
   while (this->r_1->hasNext()) {
      if (this->takeValue(this->r_1, U"displayicon")) {
         return;
      }
      if (this->takeValueFirstArg(this->r_1, U"uninstallstring")) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValueFirstArg(this->r_2, EMPTY_STRING)) {
         return;
      }
   }
};


WP_Vlc::WP_Vlc(Perun2Process& p2) : WinProgram(p2, { U"vlc", U"vlcmediaplayer" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, U"software/microsoft/windows/currentversion/uninstall/vlc media player");
   addRegistryPattern(this->r_2, RegistryRootType::ClassesRoot, U"vlc*/defaulticon");
   addRegistryPattern(this->r_3, RegistryRootType::ClassesRoot, U"vlc*/shell/open/command");
   addRegistryPattern(this->r_4, RegistryRootType::ClassesRoot, U"vlc*/shell/playwithvlc/command");
};


void WP_Vlc::actualize()
{
   while (this->r_1->hasNext()) {
      if (this->takeValue(this->r_1, U"displayicon")) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValueFirstArg(this->r_2, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_3->hasNext()) {
      if (this->takeValueFirstArg(this->r_3, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_4->hasNext()) {
      if (this->takeValueFirstArg(this->r_4, EMPTY_STRING)) {
         return;
      }
   }
};


WP_WinRAR::WP_WinRAR(Perun2Process& p2) : WinProgram(p2, { U"winrar" }) ,
   startMenuLink(U"winrar\\winrar.lnk")
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, U"software/winrar");
   addRegistryPattern(this->r_2, RegistryRootType::LocalMachine, U"software/microsoft/windows/currentversion/uninstall/winrar*");
   addRegistryPattern(this->r_3, RegistryRootType::ClassesRoot, U"winrar*/shell/open/command");
   addRegistryPattern(this->r_4, RegistryRootType::ClassesRoot, U"winrar*/defaulticon");
};


void WP_WinRAR::actualize()
{
   if (this->takeStartMenuLink(this->startMenuLink)) {
      return;
   }
   
   while (this->r_1->hasNext()) {
      if (this->takeValue(this->r_1, U"exe64")) {
         return;
      }
      if (this->takeValue(this->r_1, U"exe32")) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValue(this->r_2, U"displayicon")) {
         return;
      }
   }

   while (this->r_3->hasNext()) {
      if (this->takeValueFirstArg(this->r_3, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_4->hasNext()) {
      if (this->takeValueBeforeLastComma(this->r_4, EMPTY_STRING)) {
         return;
      }
   }
};


WP_Word::WP_Word(Perun2Process& p2) : WinProgram(p2, { U"word", U"msword", U"microsoftword" }) 
{
   addRegistryPattern(this->r_1, RegistryRootType::LocalMachine, U"software/classes/wordmhtmlfile/defaulticon");
   addRegistryPattern(this->r_2, RegistryRootType::LocalMachine, U"software/classes/wordhtmltemplate/shell/open/command");
   addRegistryPattern(this->r_3, RegistryRootType::LocalMachine, U"software/classes/wordhtmlfile/shell/open/command");
};


void WP_Word::actualize()
{
   while (this->r_1->hasNext()) {
      if (this->takeValueBeforeLastComma(this->r_1, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_2->hasNext()) {
      if (this->takeValueFirstArg(this->r_2, EMPTY_STRING)) {
         return;
      }
   }

   while (this->r_3->hasNext()) {
      if (this->takeValueFirstArg(this->r_3, EMPTY_STRING)) {
         return;
      }
   }
};


}
