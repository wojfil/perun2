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

#pragma once

#include "../datatype/datatype.h"
#include <iostream>
#include <windows.h>



namespace perun2
{
struct FileContext;
struct LocationContext;
struct Perun2Process;
}

namespace perun2::shm
{


static p_constexpr p_char SHM_NAME_HEAD[] = L"Local\\SharedMemoryPerunPython";


static p_constexpr p_int STATUS_PERUN_NULL = 0;
static p_constexpr p_int STATUS_PERUN_IDLE = 1;
static p_constexpr p_int STATUS_PERUN_ASKS = 2;

static p_constexpr p_int STATUS_PYTHON_NULL = 0;
static p_constexpr p_int STATUS_PYTHON_IDLE = 1;
static p_constexpr p_int STATUS_PYTHON_WORKING = 2;
static p_constexpr p_int STATUS_PYTHON_RESPONDED = 3;
static p_constexpr p_int STATUS_PYTHON_ERROR = 4;

static p_constexpr p_int STATUS_LOCATION_NOT_SET = 0;
static p_constexpr p_int STATUS_LOCATION_STILL_THE_SAME = 1;
static p_constexpr p_int STATUS_LOCATION_CHANGED = 2;

static p_constexpr p_int STRING_LENGTH = 33000;

static p_constexpr p_int OFFSET_PERUN_STATUS =          sizeof(p_int) * 0;
static p_constexpr p_int OFFSET_PYTHON_STATUS =         sizeof(p_int) * 1;
static p_constexpr p_int OFFSET_RESULT =                sizeof(p_int) * 2;
static p_constexpr p_int OFFSET_LOCATION_STATUS =       sizeof(p_int) * 3;
static p_constexpr p_int OFFSET_LENGTH_FILE_PATH =      sizeof(p_int) * 4;
static p_constexpr p_int OFFSET_LENGTH_LOCATION_PATH =  sizeof(p_int) * 5;
static p_constexpr p_int OFFSET_FILE_PATH =             sizeof(p_int) * 6;
static p_constexpr p_int OFFSET_LOCATION_PATH =         sizeof(p_int) * 6 + (STRING_LENGTH * sizeof(p_char));

static p_constexpr p_int AMOUNT_INTS = 7;
static p_constexpr p_int AMOUNT_STRINGS = 2;

static p_constexpr p_int TOTAL_SIZE = AMOUNT_INTS * sizeof(p_int) + AMOUNT_STRINGS * STRING_LENGTH * sizeof(p_char);


static p_bool sharedMemoryExists(const p_str& name);
static p_int nextSharedMemoryId();


struct SharedMemory
{
public:
   SharedMemory() = delete;
   SharedMemory(const FileContext& fctx, const LocationContext& lctx);

   void makeMemoryId();
   p_int getMemoryId() const;
   p_bool start();
   p_bool ask();
   void terminate();

private:
   p_str getLocation() const;
   p_int readInt(const size_t offset) const;
   void writeInt(const size_t offset, const p_int value);
   void writeString(const size_t offset, const p_str& value);

   const FileContext& fileContext;
   const LocationContext& locationContext;
   p_int memoryId = -1;
   p_str name;

   p_bool isRunning = false;
   HANDLE map;
   void* pointer;
};


}
