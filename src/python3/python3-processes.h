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
#include <optional>
#include <unordered_map>
#include <iostream>
#include <thread>
#include <future>
#include <chrono>


namespace perun2
{
struct FileContext;
struct LocationContext;
struct Perun2Process;
}

namespace perun2::comm
{


enum Python3AskerResult 
{
   PAR_Good = 0,
   PAR_Bad = 1,
   PAR_Bad_PipeNotCreated = 2,
   PAR_Bad_ProcessNotStarted = 3
};



p_constexpr p_char PYTHON_ASKER_ROOT_FILE[] = L"asker.py";
p_constexpr p_char PYTHON_ANALYZER_ROOT_FILE[] = L"analyzer.py";



p_constexpr p_char SHM_NAME[] = L"Local\\MySharedMem";
p_constexpr int SHM_SIZE = 12;  // 3 * 4 bytes (int32)


struct AskablePython3Script
{
public:
   AskablePython3Script() = delete;
   AskablePython3Script(const FileContext& fctx, const LocationContext& lctx, Perun2Process& p2);
   
   void start(const p_str& askerScript, const p_str& funcName, 
      const p_str& filePath, const p_int line);

   void terminate();
   p_bool ask();

private:
   void python3StaticTypeAnalysis(const p_str& funcName, const p_str& filePath, const p_int line);
   p_str askerPython3RunCmd(const p_str& python, const p_str& path, const p_str& filePath) const;
   p_str getLocation() const;
   void startLoudly(std::promise<Python3AskerResult> midResultPromise, const p_str& command);
   void startSilently(std::promise<Python3AskerResult> midResultPromise, const p_str& command);

   const FileContext& fileContext;
   const LocationContext& locationContext;
   Perun2Process& perun2;
   std::unique_ptr<std::thread> thread;
};


struct Python3Processes
{
public:
   Python3Processes() = delete;
   Python3Processes(Perun2Process& p2);
   ~Python3Processes();

   AskablePython3Script& addAskableScript(const FileContext& fctx, const LocationContext& lctx, 
      const p_str& funcName, const p_str& filePath, const p_int line);



   void terminate();

private:
   
   Perun2Process& perun2;
   std::vector<AskablePython3Script> askableScripts;
};



}
