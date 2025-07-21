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

#include "shared-memory.h"
#include "../context/ctx-file.h"
#include "../command/com-execute.h"
#include "../perun2.h"
#include "com-python3.h"
#include <iostream>
#include <windows.h>


namespace perun2::shm
{

p_bool sharedMemoryExists(const p_str& name)
{
   HANDLE hMap = OpenFileMappingW(FILE_MAP_READ, FALSE, name.c_str());

   if (hMap == NULL) {
      DWORD err = GetLastError();

      if (err == ERROR_FILE_NOT_FOUND) {
         return false;
      }

      return false;
   }

   CloseHandle(hMap);
   return true;
}


p_int nextSharedMemoryId()
{
   p_int id = 0;
   p_str name = str(SHM_NAME_HEAD, toStr(id));

   while (sharedMemoryExists(name)) {
      id++;
      name = str(SHM_NAME_HEAD, toStr(id));
   }

   return id;
}




}
