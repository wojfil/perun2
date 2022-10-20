/*
    This file is part of Uroboros.
    Uroboros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GEN_OS_H_INCLUDED
#define GEN_OS_H_INCLUDED


#include "../generator.h"
#include "../datatype.h"
#include "../../patterns.h"
#include <windows.h>


namespace uro
{
struct Uroboros;
struct InnerVariables;
}

namespace uro::gen
{

#define P_OS_ARGS_NO_PREFIX Generator<_str>* loc, Uroboros& uro, const _str& patt, const _bool& abs
#define P_OS_ARGS_PREFIX Generator<_str>* loc, Uroboros& uro, const _str& patt, const _bool& abs, const _str& pref
#define P_OS_SHORT_ARGS_NO_PREFIX loc, uro, patt, abs
#define P_OS_SHORT_ARGS_PREFIX loc, uro, patt, abs, pref


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
   DefinitionGenerator(const OsElement& el, Uroboros& uro);

   _def* generateDefault() const;
   _def* generatePattern(Generator<_str>* location, const OsElement& element, 
      const _str& pattern, const _bool& isAbsolute) const;
   _def* generatePattern(Generator<_str>* location, const OsElement& element, 
      const _str& pattern, const _bool& isAbsolute, const _str& prefix) const;

private:
   Uroboros& uroboros;
   const OsElement element_;
};


struct OsDefinition : _def
{
public:
   OsDefinition() = delete;
   OsDefinition(P_OS_ARGS_NO_PREFIX);
   OsDefinition(P_OS_ARGS_PREFIX);

   ~OsDefinition() {
      delete location;
   }

   _fdata* getDataPtr();

protected:
   _bool first;
   Generator<_str>* location;
   Uroboros& uroboros;
   InnerVariables& inner;
   _fdata data;
   _numi index;
   _str baseLocation;
   P_MEMORY_MEMBER;

   const _str pattern;
   const _uint32 flags;
   const _bool isAbsolute;
   const _bool hasPrefix;
   const _str prefix;
};


struct OsDefinitionPlain : OsDefinition
{
public:
   OsDefinitionPlain() = delete;
   OsDefinitionPlain(P_OS_ARGS_NO_PREFIX) : OsDefinition(P_OS_SHORT_ARGS_NO_PREFIX) { };
   OsDefinitionPlain(P_OS_ARGS_PREFIX) : OsDefinition(P_OS_SHORT_ARGS_PREFIX) { };

   void reset() override;

protected:
   HANDLE handle;
};


struct OsDefinitionRecursive : OsDefinition
{
public:
   OsDefinitionRecursive() = delete;
   OsDefinitionRecursive(P_OS_ARGS_NO_PREFIX) : OsDefinition(P_OS_SHORT_ARGS_NO_PREFIX) { };
   OsDefinitionRecursive(P_OS_ARGS_PREFIX) : OsDefinition(P_OS_SHORT_ARGS_PREFIX) { };

   void reset() override;

protected:
   _bool goDeeper = false;
   std::vector<HANDLE> handles;
   _list paths;
   _list bases;
};


struct Uro_Files : OsDefinitionPlain
{
public:
   Uro_Files() = delete;
   Uro_Files(P_OS_ARGS_NO_PREFIX) : OsDefinitionPlain(P_OS_SHORT_ARGS_NO_PREFIX) {};
   Uro_Files(P_OS_ARGS_PREFIX) : OsDefinitionPlain(P_OS_SHORT_ARGS_PREFIX) {};

   _bool hasNext() override;
};


struct Uro_Directories : OsDefinitionPlain
{
public:
   Uro_Directories() = delete;
   Uro_Directories(P_OS_ARGS_NO_PREFIX) : OsDefinitionPlain(P_OS_SHORT_ARGS_NO_PREFIX) {};
   Uro_Directories(P_OS_ARGS_PREFIX) : OsDefinitionPlain(P_OS_SHORT_ARGS_PREFIX) {};

   _bool hasNext() override;
};


struct Uro_All : OsDefinitionPlain
{
public:
   Uro_All() = delete;
   Uro_All(P_OS_ARGS_NO_PREFIX) : OsDefinitionPlain(P_OS_SHORT_ARGS_NO_PREFIX) {};
   Uro_All(P_OS_ARGS_PREFIX) : OsDefinitionPlain(P_OS_SHORT_ARGS_PREFIX) {};

   _bool hasNext() override;
};


struct Uro_RecursiveFiles : OsDefinitionRecursive
{
public:
   Uro_RecursiveFiles() = delete;
   Uro_RecursiveFiles(P_OS_ARGS_NO_PREFIX) : OsDefinitionRecursive(P_OS_SHORT_ARGS_NO_PREFIX) { };
   Uro_RecursiveFiles(P_OS_ARGS_PREFIX) : OsDefinitionRecursive(P_OS_SHORT_ARGS_PREFIX) { };

   _bool hasNext() override;
};


struct Uro_RecursiveDirectories : OsDefinitionRecursive
{
public:
   Uro_RecursiveDirectories() = delete;
   Uro_RecursiveDirectories(P_OS_ARGS_NO_PREFIX) : OsDefinitionRecursive(P_OS_SHORT_ARGS_NO_PREFIX) { };
   Uro_RecursiveDirectories(P_OS_ARGS_PREFIX) : OsDefinitionRecursive(P_OS_SHORT_ARGS_PREFIX) { };

   _bool hasNext() override;
};


struct Uro_RecursiveAll : OsDefinitionRecursive
{
public:
   Uro_RecursiveAll() = delete;
   Uro_RecursiveAll(P_OS_ARGS_NO_PREFIX) : OsDefinitionRecursive(P_OS_SHORT_ARGS_NO_PREFIX) { };
   Uro_RecursiveAll(P_OS_ARGS_PREFIX) : OsDefinitionRecursive(P_OS_SHORT_ARGS_PREFIX) { };

   _bool hasNext() override;

private:
   _bool prevFile = false;
};

}

#endif // GEN_OS_H_INCLUDED
