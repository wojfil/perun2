/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GEN_OS_H_INCLUDED
#define GEN_OS_H_INCLUDED

#include "../datatype.h"
#include "../patterns.h"
#include "../../os.h"
#include <windows.h>


namespace uro
{
struct _uro;
struct InnerVariables;
}

namespace uro::gen
{

#define P_GEN_OS_ARGS _genptr<_str>& loc, _uro& uro, const _bool& abs, const _str& pref
#define P_GEN_OS_ARGS_2 loc, uro, abs, pref
#define P_GEN_OS_ARGS_EXT _genptr<_str>& loc, _uro& uro, const _str& patt, const _bool& abs, const _str& pref
#define P_GEN_OS_ARGS_EXT_2 loc, uro, patt, abs, pref


namespace os
{
   inline constexpr _bool IS_ABSOLUTE_PATH =            true;
   inline constexpr _bool IS_RELATIVE_PATH =            false;
   static const _str NO_PREFIX =                        EMPTY_STRING;
   static const _str DEFAULT_PATTERN =                  OS_SEPARATOR_ASTERISK;
   inline constexpr _bool IS_FINAL =                    true;
   inline constexpr _bool IS_NOT_FINAL =                false;
}


enum OsElement
{
   oe_None = 0,
   oe_All,
   oe_Directories,
   oe_Files,
   oe_RecursiveAll,
   oe_RecursiveDirectories,
   oe_RecursiveFiles
};


struct DefinitionGenerator
{
public:
   DefinitionGenerator() = delete;
   DefinitionGenerator(const OsElement& el, _uro& uro);

   _bool generate(_defptr& result) const;

private:
   _uro& uroboros;
   const OsElement element_;
};


struct OsDefinition : _def
{
public:
   OsDefinition() = delete;
   OsDefinition(P_GEN_OS_ARGS);
   _fdata* getDataPtr();

protected:
   _bool first = true;
   _genptr<_str> location;
   _uro& uroboros;
   InnerVariables& inner;
   _fdata data;
   _numi index;
   _str baseLocation;
   P_MEMORY_MEMBER;

   const _uint32 flags;
   const _bool isAbsolute;
   const _bool hasPrefix;
   const _str prefix;
};


struct OsDefinitionPlain : OsDefinition
{
public:
   OsDefinitionPlain() = delete;
   OsDefinitionPlain(P_GEN_OS_ARGS_EXT) 
      : OsDefinition(P_GEN_OS_ARGS_2), pattern(patt) { };

   void reset() override;

protected:
   HANDLE handle;
   const _str pattern;
};


struct OsDefinitionRecursive : OsDefinition
{
public:
   OsDefinitionRecursive() = delete;
   OsDefinitionRecursive(P_GEN_OS_ARGS) 
      : OsDefinition(P_GEN_OS_ARGS_2) { };

   void reset() override;

protected:
   void setDepth();

   _numi depth;
   _bool goDeeper = false;
   std::vector<HANDLE> handles;
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
      : OsDefinitionPlain(P_GEN_OS_ARGS_EXT_2) {};

   _bool hasNext() override;
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
