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
#include "../def-gen.h"
#include <windows.h>


inline constexpr _uint32 ELEM_NONE = 0;
inline constexpr _uint32 ELEM_ALL = 1;
inline constexpr _uint32 ELEM_DIRECTORIES = 2;
inline constexpr _uint32 ELEM_FILES = 3;
inline constexpr _uint32 ELEM_RECURSIVE_ALL = 4;
inline constexpr _uint32 ELEM_RECURSIVE_DIRECTORIES = 5;
inline constexpr _uint32 ELEM_RECURSIVE_FILES = 6;

struct Uroboros;
struct InnerVariables;


struct DefinitionGenerator
{
public:
   DefinitionGenerator(const _uint32& el, Uroboros* uro)
      : element_(el), uroboros(uro) { };

   _def* generateDefault() const;
   _def* generatePattern(Generator<_str>* location, const _uint32& element, const _str& pattern) const;

private:
   Uroboros* uroboros;
   const _uint32 element_;
};


struct OsDefinition : _def
{
public:
   OsDefinition(Generator<_str>* loc, Uroboros* uro, const _str& patt);

   ~OsDefinition() {
      delete location;
   }

protected:
   _boo first;
   Generator<_str>* location;
   Uroboros* uroboros;
   InnerVariables* inner;
   WIN32_FIND_DATA data;
   _str prevThis;
   _numi prevIndex;
   _numi index;
   _numi prevDepth;

   const _str pattern;
   const _uint32 flags;
   // just save a copy of flags in the memory
   // they are constant, so a pointer is unnecessary
};


struct OsDefinitionPlain : OsDefinition
{
public:
   OsDefinitionPlain(Generator<_str>* loc, Uroboros* uro, const _str& patt)
      : OsDefinition(loc, uro, patt) { };

   void reset() override;

protected:
   HANDLE handle;
};


struct OsDefinitionRecursive : OsDefinition
{
public:
   OsDefinitionRecursive(Generator<_str>* loc, Uroboros* uro, const _str& patt)
      : OsDefinition(loc, uro, patt), goDeeper(false), paths(_list()),
        bases(_list()), handles(std::vector<HANDLE>()) { };

   void reset() override;

protected:
   _boo goDeeper;
   std::vector<HANDLE> handles;
   _list paths;
   _list bases;
};


struct Uro_Files : OsDefinitionPlain
{
public:
   Uro_Files(Generator<_str>* loc, Uroboros* uro, const _str& patt)
      : OsDefinitionPlain(loc, uro, patt) {};

   _boo hasNext() override;
};


struct Uro_Directories : OsDefinitionPlain
{
public:
   Uro_Directories(Generator<_str>* loc, Uroboros* uro, const _str& patt)
      : OsDefinitionPlain(loc, uro, patt) {};

   _boo hasNext() override;
};


struct Uro_All : OsDefinitionPlain
{
public:
   Uro_All(Generator<_str>* loc, Uroboros* uro, const _str& patt)
      : OsDefinitionPlain(loc, uro, patt) {};

   _boo hasNext() override;
};


struct Uro_RecursiveFiles : OsDefinitionRecursive
{
public:
   Uro_RecursiveFiles(Generator<_str>* loc, Uroboros* uro, const _str& patt)
      : OsDefinitionRecursive(loc, uro, patt) { };

   _boo hasNext() override;
};


struct Uro_RecursiveDirectories : OsDefinitionRecursive
{
public:
   Uro_RecursiveDirectories(Generator<_str>* loc, Uroboros* uro, const _str& patt)
      : OsDefinitionRecursive(loc, uro, patt) { };

   _boo hasNext() override;
};


#endif // GEN_OS_H_INCLUDED
