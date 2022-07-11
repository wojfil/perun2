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
#include "../../uroboros.h"
#include <windows.h>


const _uint32 ELEM_ALL = 0;
const _uint32 ELEM_DIRECTORIES = 1;
const _uint32 ELEM_FILES = 2;


struct OsDefinition : _def
{
public:
   OsDefinition(Generator<_str>* loc, Uroboros* uro)
      : first(true), location(loc), uroboros(uro),
        inner(&uro->vars.inner), flags(uro->flags) { };

protected:
   _boo first;
   Generator<_str>* location;
   Uroboros* uroboros;
   InnerVariables* inner;

   const _uint32 flags;
   // just save a copy of flags in the memory
   // they are constant, so a pointer is unnecessary
};


struct Gen_ElementsAtLocation : DefinitionGenerator
{
public:
   Gen_ElementsAtLocation(const _uint32& el, Uroboros* uro)
      : element(el), uroboros(uro) { };

   _def* generate(Generator<_str>* location) override;

private:
   Uroboros* uroboros;
   const _uint32 element;
};


struct ElementsAtLocation : OsDefinition
{
public:
   ElementsAtLocation (Generator<_str>* loc, _uint32 elem, Uroboros* uro)
      : element(elem), OsDefinition(loc, uro) {};

   ~ElementsAtLocation() {
      delete location;
   }

   void reset() override;
   _boo hasNext() override;

private:
   const _uint32 element;
   WIN32_FIND_DATA data;
   HANDLE handle;
   _str prevThis;
   _numi prevIndex;
   _numi index;
   _numi prevDepth;
};


struct Gen_RecursiveFiles : DefinitionGenerator
{
public:
   Gen_RecursiveFiles(Uroboros* uro) : uroboros(uro) { };

   _def* generate(Generator<_str>* location) override;

private:
   Uroboros* uroboros;
};


struct RecursiveFiles : OsDefinition
{
public:
   RecursiveFiles(Generator<_str>* loc, Uroboros* uro)
      : OsDefinition(loc, uro), paths(_list()),
        bases(_list()), handles(std::vector<HANDLE>()) { };

   ~RecursiveFiles();

   void reset() override;
   _boo hasNext() override;

private:
   _boo goDeeper;
   WIN32_FIND_DATA data;
   std::vector<HANDLE> handles;
   _list paths;
   _list bases;
   _numi prevDepth;
   _str prevThis;
   _numi prevIndex;
   _numi index;
};


struct Gen_RecursiveDirectories : DefinitionGenerator
{
public:
   Gen_RecursiveDirectories(Uroboros* uro) : uroboros(uro) { };
   _def* generate(Generator<_str>* location) override;

private:
   Uroboros* uroboros;
};


struct RecursiveDirectories : OsDefinition
{
public:
   RecursiveDirectories(Generator<_str>* loc, Uroboros* uro)
      : OsDefinition(loc, uro), paths(), bases(), handles() { };

   ~RecursiveDirectories();

   void reset() override;
   _boo hasNext() override;

private:
   _boo goDeeper;
   WIN32_FIND_DATA data;
   std::vector<HANDLE> handles;
   _list paths;
   _list bases;
   _numi prevDepth;
   _str prevThis;
   _numi prevIndex;
   _numi index;
};


#endif // GEN_OS_H_INCLUDED
