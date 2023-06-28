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

#ifndef GEN_OS_H_INCLUDED
#define GEN_OS_H_INCLUDED

#include "../patterns.h"
#include "../wildcard.h"
#include "../../os.h"
#include "../../context.h"
#include "../../arguments.h"
#include <windows.h>


namespace perun2
{
struct _p2;
}

namespace perun2::gen
{

#define P_GEN_OS_ARGS _genptr<_str>& loc, _p2& p2, const _bool abs, const _str& pref
#define P_GEN_OS_ARGS_2 loc, p2, abs, pref
#define P_GEN_OS_ARGS_EXT _genptr<_str>& loc, _p2& p2, const _str& patt, const _bool abs, const _str& pref
#define P_GEN_OS_ARGS_EXT_2 loc, p2, patt, abs, pref


namespace os
{
   _constexpr _bool IS_ABSOLUTE_PATH =            true;
   _constexpr _bool IS_RELATIVE_PATH =            false;
   _constexpr _bool IS_FINAL =                    true;
   _constexpr _bool IS_NOT_FINAL =                false;

   _str getDefaultPattern();
}


struct OsDefinition : _def
{
public:
   OsDefinition() = delete;
   OsDefinition(P_GEN_OS_ARGS);
   FileContext* getFileContext() override;

protected:
   _bool first = true;
   _genptr<_str> location;
   _p2& perun2;
   FileContext context;
   _fdata data;
   _num index;
   _str baseLocation;

   const _flags flags;
   const _bool isAbsolute;
   const _bool hasPrefix;
   const _str prefix;
};


struct OsDefinitionPlain : OsDefinition
{
public:
   OsDefinitionPlain() = delete;
   OsDefinitionPlain(P_GEN_OS_ARGS_EXT)
      : OsDefinition(P_GEN_OS_ARGS_2),
        pattern(isExceptional(patt) ? gen::os::getDefaultPattern() : patt),
        exceptional(isExceptional(patt)),
        comparer(patt.substr(1)) { };

   void reset() override;

protected:
   _entry handle;
   const _str pattern;

   // Windows API has some trouble with handling patterns that consist of only dots and asterisks
   // so, in this rare case, let us generate all files and directories with *
   // and then filter them with SimpleWildcardComparer
   const _bool exceptional;
   SimpleWildcardComparer comparer;

private:
   _bool isExceptional(const _str& patt);
};


struct OsDefinitionRecursive : OsDefinition
{
public:
   OsDefinitionRecursive() = delete;
   OsDefinitionRecursive(P_GEN_OS_ARGS)
      : OsDefinition(P_GEN_OS_ARGS_2) { };

   void reset() override;

protected:
   _bool goDeeper = false;
   std::vector<_entry> handles;
   _list paths;
   _list bases;
};


struct Files : OsDefinitionPlain
{
public:
   Files() = delete;
   Files(P_GEN_OS_ARGS_EXT)
      : OsDefinitionPlain(P_GEN_OS_ARGS_EXT_2) {};

   _bool hasNext() override;
};


struct Directories : OsDefinitionPlain
{
public:
   Directories() = delete;
   Directories(P_GEN_OS_ARGS_EXT)
      : OsDefinitionPlain(P_GEN_OS_ARGS_EXT_2) {};

   _bool hasNext() override;
};


struct All : OsDefinitionPlain
{
public:
   All() = delete;
   All(P_GEN_OS_ARGS_EXT)
      : OsDefinitionPlain(P_GEN_OS_ARGS_EXT_2), retreats(0) {};
      
   All(P_GEN_OS_ARGS_EXT, const _int retr)
      : OsDefinitionPlain(P_GEN_OS_ARGS_EXT_2), retreats(retr) {};

   _bool hasNext() override;

private:
   const _int retreats;
};


struct RecursiveFiles : OsDefinitionRecursive
{
public:
   RecursiveFiles() = delete;
   RecursiveFiles(P_GEN_OS_ARGS)
      : OsDefinitionRecursive(P_GEN_OS_ARGS_2) { };

   _bool hasNext() override;
};


struct RecursiveDirectories : OsDefinitionRecursive
{
public:
   RecursiveDirectories() = delete;
   RecursiveDirectories(P_GEN_OS_ARGS)
      : OsDefinitionRecursive(P_GEN_OS_ARGS_2) { };

   _bool hasNext() override;
};


struct RecursiveAll : OsDefinitionRecursive
{
public:
   RecursiveAll() = delete;
   RecursiveAll(P_GEN_OS_ARGS)
      : OsDefinitionRecursive(P_GEN_OS_ARGS_2) { };

   _bool hasNext() override;

private:
   _bool prevFile = false;
};

}

#endif // GEN_OS_H_INCLUDED
