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


const _uint32 ELEM_ALL = 0;
const _uint32 ELEM_DIRECTORIES = 1;
const _uint32 ELEM_FILES = 2;



struct Gen_ElementsAtLocation : DefinitionGenerator
{
public:
   Gen_ElementsAtLocation(const _uint32& el) : element(el) { };
   _def* generate(Generator<_str>* location) override;

private:
   const _uint32 element;
};


struct ElementsAtLocation : _def
{
public:
   ElementsAtLocation (Generator<_str>* loc, _uint32 elem)
      : location(loc), element(elem), first(true) {};

   ~ElementsAtLocation() {
      delete location;
   }

   void reset() override;
   _boo hasNext() override;

private:
   const _uint32 element;
   _boo first;
   Generator<_str>* location;
   WIN32_FIND_DATA data;
   HANDLE handle;
   _str prevThis;
   _num prevIndex;
   _num index;
   _num prevDepth;
};


struct Gen_RecursiveFiles : DefinitionGenerator
{
public:
   Gen_RecursiveFiles() { };

   _def* generate(Generator<_str>* location) override;
};


struct RecursiveFiles : _def
{
public:
   RecursiveFiles(Generator<_str>* loc)
      : location(loc), first(true),
        paths(_list()), bases(_list()), handles(std::vector<HANDLE>()) { };

   ~RecursiveFiles();

   void reset() override;
   _boo hasNext() override;

private:
   Generator<_str>* location;
   _boo first;
   _boo goDeeper;
   WIN32_FIND_DATA data;
   std::vector<HANDLE> handles;
   _list paths;
   _list bases;
   _num prevDepth;
   _str prevThis;
   _num prevIndex;
   _num index;
};


struct Gen_RecursiveDirectories : DefinitionGenerator
{
public:
   Gen_RecursiveDirectories() { };
   _def* generate(Generator<_str>* location) override;
};


struct RecursiveDirectories : _def
{
public:
   RecursiveDirectories(Generator<_str>* loc)
      : location(loc), first(true),
        paths(), bases(), handles() { };

   ~RecursiveDirectories();

   void reset() override;
   _boo hasNext() override;

private:
   Generator<_str>* location;
   _boo first;
   _boo goDeeper;
   WIN32_FIND_DATA data;
   std::vector<HANDLE> handles;
   _list paths;
   _list bases;
   _num prevDepth;
   _str prevThis;
   _num prevIndex;
   _num index;
};




#endif // GEN_OS_H_INCLUDED
