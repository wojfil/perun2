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

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include "start-menu.h"
#include "../../os/os.h"
#include <windows.h>
#include <shobjidl.h>
#include <objbase.h>
#include <Shlobj.h>
#include <iostream>


namespace perun2::prog
{


StartMenuLnk::StartMenuLnk(const p_str& path)
   : relativePath(path) { };
   

p_bool StartMenuLnk::hasValue()
{

#define P_FAILURE    this->state = Logic::False; return false;


   if (this->state != Logic::Unknown) {
      return this->state == Logic::True;
   }

   p_str linkFilePath;
   if (!this->getFilePath(linkFilePath)) {
      P_FAILURE;
   }

   IShellLink* shellLink = nullptr;
   HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, 
      CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&shellLink);

   if (FAILED(hr)) {
      CoUninitialize();
      CoInitializeEx(0, COINIT_MULTITHREADED);
      P_FAILURE;
   }

   IPersistFile* persistFile = nullptr;
   hr = shellLink->QueryInterface(IID_IPersistFile, (LPVOID*)&persistFile);

   if (FAILED(hr)) {
      shellLink->Release();
      CoUninitialize();
      CoInitializeEx(0, COINIT_MULTITHREADED);
      P_FAILURE;
   }

   hr = persistFile->Load(linkFilePath.c_str(), STGM_READ);

   if (FAILED(hr)) {
      shellLink->Release();
      persistFile->Release();
      CoUninitialize();
      CoInitializeEx(0, COINIT_MULTITHREADED);
      P_FAILURE;
   }

   p_char targetPath[MAX_PATH];
   hr = shellLink->GetPath(targetPath, MAX_PATH, NULL, SLGP_UNCPRIORITY);
   
   persistFile->Release();
   shellLink->Release();
   CoUninitialize();
   CoInitializeEx(0, COINIT_MULTITHREADED);

   if (SUCCEEDED(hr)) {
      result = targetPath;
      this->state = Logic::True;
      return true;
   }

   P_FAILURE;
}


p_str StartMenuLnk::getValue()
{
   return this->result;
}


p_str StartMenuLnk::getPathRoot(const p_bool allUsers) const
{
   p_char* startMenuPath;

   if (SUCCEEDED(SHGetKnownFolderPath(
      allUsers ? FOLDERID_CommonStartMenu : FOLDERID_StartMenu, 0, NULL, &startMenuPath))) 
   {
      CoTaskMemFree(static_cast<void*>(startMenuPath));
      return startMenuPath;
   }

   return EMPTY_STRING;
}


p_bool StartMenuLnk::getFilePath(p_str& path) const
{
   const p_str allUsersPath = this->getPathRoot(true);
   if (! allUsersPath.empty()) {
      path = str(allUsersPath, OS_SEPARATOR, STRING_PROGRAMS, OS_SEPARATOR, this->relativePath);
      if (os_fileExists(path)) {
         return true;
      }

      path = str(allUsersPath, OS_SEPARATOR, this->relativePath);
      if (os_fileExists(path)) {
         return true;
      }
   }

   const p_str currentUserPath = this->getPathRoot(false);
   if (! currentUserPath.empty()) {
      path = str(currentUserPath, OS_SEPARATOR, STRING_PROGRAMS, OS_SEPARATOR, this->relativePath);
      if (os_fileExists(path)) {
         return true;
      }

      path = str(currentUserPath, OS_SEPARATOR, this->relativePath);
      if (os_fileExists(path)) {
         return true;
      }
   }

   return false;
}


}
