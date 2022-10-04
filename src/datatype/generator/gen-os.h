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
   DefinitionGenerator(const OsElement& el, uro::Uroboros* uro)
      : element_(el), uroboros(uro) { };

   _def* generateDefault() const;
   _def* generatePattern(Generator<_str>* location, const OsElement& element, 
      const _str& pattern, const _bool& isAbsolute) const;

private:
   uro::Uroboros* uroboros;
   const OsElement element_;
};


struct OsDefinition : _def
{
public:
   OsDefinition(Generator<_str>* loc, uro::Uroboros* uro, const _str& patt, const _bool& abs);

   ~OsDefinition() {
      delete location;
   }

   _fdata* getDataPtr();

protected:
   _bool first;
   Generator<_str>* location;
   uro::Uroboros* uroboros;
   uro::InnerVariables* inner;
   _fdata data;
   _numi index;
   P_MEMORY_MEMBER;

   const _str pattern;
   const _uint32 flags; // a copy of Uroboros command-line flags
   _str baseLocation;
   const _bool isAbsolute;
};


struct OsDefinitionPlain : OsDefinition
{
public:
   OsDefinitionPlain(Generator<_str>* loc, uro::Uroboros* uro, const _str& patt, const _bool& abs)
      : OsDefinition(loc, uro, patt, abs) { };

   void reset() override;

protected:
   HANDLE handle;
};


struct OsDefinitionRecursive : OsDefinition
{
public:
   OsDefinitionRecursive(Generator<_str>* loc, uro::Uroboros* uro, const _str& patt, const _bool& abs)
      : OsDefinition(loc, uro, patt, abs) { };

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
   Uro_Files(Generator<_str>* loc, uro::Uroboros* uro, const _str& patt, const _bool& abs)
      : OsDefinitionPlain(loc, uro, patt, abs) {};

   _bool hasNext() override;
};


struct Uro_Directories : OsDefinitionPlain
{
public:
   Uro_Directories(Generator<_str>* loc, uro::Uroboros* uro, const _str& patt, const _bool& abs)
      : OsDefinitionPlain(loc, uro, patt, abs) {};

   _bool hasNext() override;
};


struct Uro_All : OsDefinitionPlain
{
public:
   Uro_All(Generator<_str>* loc, uro::Uroboros* uro, const _str& patt, const _bool& abs)
      : OsDefinitionPlain(loc, uro, patt, abs) {};

   _bool hasNext() override;
};


struct Uro_RecursiveFiles : OsDefinitionRecursive
{
public:
   Uro_RecursiveFiles(Generator<_str>* loc, uro::Uroboros* uro, const _str& patt, const _bool& abs)
      : OsDefinitionRecursive(loc, uro, patt, abs) { };

   _bool hasNext() override;
};


struct Uro_RecursiveDirectories : OsDefinitionRecursive
{
public:
   Uro_RecursiveDirectories(Generator<_str>* loc, uro::Uroboros* uro, const _str& patt, const _bool& abs)
      : OsDefinitionRecursive(loc, uro, patt, abs) { };

   _bool hasNext() override;
};


struct Uro_RecursiveAll : OsDefinitionRecursive
{
public:
   Uro_RecursiveAll(Generator<_str>* loc, uro::Uroboros* uro, const _str& patt, const _bool& abs)
      : OsDefinitionRecursive(loc, uro, patt, abs) { };

   _bool hasNext() override;

private:
   _bool prevFile = false;
};

}

#endif // GEN_OS_H_INCLUDED
