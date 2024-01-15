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

#ifndef GEN_OS_H_INCLUDED
#define GEN_OS_H_INCLUDED

#include "../patterns.h"
#include "../text/wildcard.h"
#include "../../os/os.h"
#include "../../arguments.h"
#include "../../context/ctx-file.h"
#include <windows.h>


namespace perun2
{
struct p_perun2;
}

namespace perun2::gen
{

#define P_GEN_OS_ARGS p_genptr<p_str>& loc, p_perun2& p2, const PathType pt, const p_str& pref
#define P_GEN_OS_ARGS_2 loc, p2, pt, pref
#define P_GEN_OS_ARGS_EXT p_genptr<p_str>& loc, p_perun2& p2, const p_str& patt, const PathType pt, const p_str& pref
#define P_GEN_OS_ARGS_EXT_2 loc, p2, patt, pt, pref



namespace os
{
   extern const p_str DEFAULT_PATTERN;
}


struct OsDefinition : p_def
{
public:
   OsDefinition() = delete;
   OsDefinition(P_GEN_OS_ARGS);
   FileContext* getFileContext() override;

protected:
   p_bool first = true;
   p_genptr<p_str> location;
   p_perun2& perun2;
   FileContext context;
   p_fdata data;
   p_num index;
   p_str baseLocation;

   const p_flags flags;
   const PathType pathType;
   const p_bool hasPrefix;
   const p_str prefix;
};


struct OsDefinitionPlain : OsDefinition
{
public:
   OsDefinitionPlain() = delete;
   OsDefinitionPlain(P_GEN_OS_ARGS_EXT)
      : OsDefinition(P_GEN_OS_ARGS_2),
        pattern(isExceptional(patt) ? gen::os::DEFAULT_PATTERN : patt),
        exceptional(isExceptional(patt)),
        comparer(patt.substr(1)) { };

   void reset() override;

protected:
   p_entry handle;
   const p_str pattern;

   // Windows API has some trouble with handling patterns that consist of only dots and asterisks
   // so, in this rare case, let us generate all files and directories with *
   // and then filter them with SimpleWildcardComparer
   const p_bool exceptional;
   SimpleWildcardComparer comparer;

private:
   p_bool isExceptional(const p_str& patt);
};


struct OsDefinitionRecursive : OsDefinition
{
public:
   OsDefinitionRecursive() = delete;
   OsDefinitionRecursive(P_GEN_OS_ARGS)
      : OsDefinition(P_GEN_OS_ARGS_2) { };

   p_bool setAction(p_daptr& act) override;
   void reset() override;

protected:
   p_bool goDeeper = false;
   std::vector<p_entry> handles;
   p_list paths;
   p_list bases;
};


struct Files : OsDefinitionPlain
{
public:
   Files() = delete;
   Files(P_GEN_OS_ARGS_EXT)
      : OsDefinitionPlain(P_GEN_OS_ARGS_EXT_2) {};

   p_bool hasNext() override;
};


struct Directories : OsDefinitionPlain
{
public:
   Directories() = delete;
   Directories(P_GEN_OS_ARGS_EXT)
      : OsDefinitionPlain(P_GEN_OS_ARGS_EXT_2) {};

   p_bool hasNext() override;
};


struct All : OsDefinitionPlain
{
public:
   All() = delete;
   All(P_GEN_OS_ARGS_EXT)
      : OsDefinitionPlain(P_GEN_OS_ARGS_EXT_2), retreats(0) {};
      
   All(P_GEN_OS_ARGS_EXT, const p_int retr)
      : OsDefinitionPlain(P_GEN_OS_ARGS_EXT_2), retreats(retr) {};

   p_bool hasNext() override;

private:
   const p_int retreats;
};


struct RecursiveFiles : OsDefinitionRecursive
{
public:
   RecursiveFiles() = delete;
   RecursiveFiles(P_GEN_OS_ARGS)
      : OsDefinitionRecursive(P_GEN_OS_ARGS_2) { };

   p_bool hasNext() override;
};


struct RecursiveDirectories : OsDefinitionRecursive
{
public:
   RecursiveDirectories() = delete;
   RecursiveDirectories(P_GEN_OS_ARGS)
      : OsDefinitionRecursive(P_GEN_OS_ARGS_2) { };

   p_bool hasNext() override;
};


struct RecursiveAll : OsDefinitionRecursive
{
public:
   RecursiveAll() = delete;
   RecursiveAll(P_GEN_OS_ARGS)
      : OsDefinitionRecursive(P_GEN_OS_ARGS_2) { };

   p_bool hasNext() override;

private:
   p_bool prevFile = false;
};

}

#endif // GEN_OS_H_INCLUDED
