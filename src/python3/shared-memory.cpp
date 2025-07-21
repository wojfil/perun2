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
#include <cstring>


namespace perun2::shm
{

p_bool sharedMemoryExists(const p_str& name)
{
   HANDLE map = OpenFileMappingW(FILE_MAP_READ, FALSE, name.c_str());

   if (map == NULL) {
      DWORD err = GetLastError();

      if (err == ERROR_FILE_NOT_FOUND) {
         return false;
      }

      return false;
   }

   CloseHandle(map);
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


SharedMemory::SharedMemory(const FileContext& fctx, const LocationContext& lctx)
   : fileContext(fctx), locationContext(lctx) { };


void SharedMemory::makeMemoryId()
{
   this->memoryId = nextSharedMemoryId();
   this->name = str(SHM_NAME_HEAD, toStr(this->memoryId));
}

p_int SharedMemory::getMemoryId() const
{
   return this->memoryId;
}

p_bool SharedMemory::start()
{
   // This ugly solution to a race condition must stay here for a while.
   // When the main Python3 process starts, we know the exact moment of it from C++.
   // However, it takes some unknown time to make the shared memory.
   // Usually 15-25 ms delay here.

   const p_nint WAIT_UNIT = 5;
   p_nint wait = 2000;

   while (true) {
      this->map = OpenFileMappingW(
         FILE_MAP_ALL_ACCESS,
         FALSE,
         this->name.c_str()
      );

      if (this->map != NULL) {
         break;
      }

      wait -= WAIT_UNIT;
      if (wait <= 0) {
         break;
      }

      os_rawSleepForMs(WAIT_UNIT);
   }

   if (this->map == NULL) {
      return false;
   }

   this->pointer = MapViewOfFile(
      this->map,
      FILE_MAP_ALL_ACCESS,
      0, 0,
      TOTAL_SIZE
   );

   if (this->pointer == NULL) {
      CloseHandle(this->map);
      return false;
   }

   this->isRunning = true;
   return true;
}

p_bool SharedMemory::ask()
{
   if (! fileContext.v_exists->getValue()) {
      return false;
   }

   if (! os_directoryExists(locationContext.location->getValue())) {
      return false;
   }

   return true;
}

void SharedMemory::terminate()
{
   if (this->isRunning) {
      UnmapViewOfFile(this->pointer);
      CloseHandle(this->map);
   }
}

p_str SharedMemory::getLocation() const
{
   return this->locationContext.location->value;
}

p_int SharedMemory::readInt(const size_t offset) const
{
   const p_int* integers = static_cast<p_int*>(this->pointer);
   return integers[offset];
}

void SharedMemory::writeInt(const size_t offset, const p_int value)
{
   p_int* integers = static_cast<p_int*>(this->pointer);
   integers[offset] = value;
}

void SharedMemory::writeString(const size_t offset, const p_str& value)
{
   // The shared memory string areas have enough space for any file path.
   // I don't do guardian checks for string length;
   p_char* destination = reinterpret_cast<p_char*>(reinterpret_cast<char*>(this->pointer) + offset);
   memcpy(destination, value.c_str(), value.size() * sizeof(p_char));
}

}
