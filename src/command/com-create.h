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

#ifndef COM_CREATE_H_INCLUDED
#define COM_CREATE_H_INCLUDED

#include "com-core.h"
#include "../attribute.h"


struct C_Create : Command
{
public:
   C_Create(const _boo& force, Attribute* attr)
      : forced(force), attribute(attr) { };

   void run() override;

private:
   const _boo forced;
   Attribute* attribute; // memory is freed in another object
};


struct C_Create_Stack : Command
{
public:
   C_Create_Stack(Attribute* attr)
      : attribute(attr) { };

   void run() override;

private:
   Attribute* attribute;
};


struct C_CreateFile : Command
{
public:
   C_CreateFile(const _boo& force, Attribute* attr)
      : forced(force), attribute(attr) { };

   void run() override;

private:
   const _boo forced;
   Attribute* attribute;
};


struct C_CreateFile_Stack : Command
{
public:
   C_CreateFile_Stack(Attribute* attr)
      : attribute(attr) { };

   void run() override;

private:
   Attribute* attribute;
};


struct C_CreateDirectory : Command
{
public:
   C_CreateDirectory(const _boo& force, Attribute* attr)
      : forced(force), attribute(attr) { };

   void run() override;

private:
   Attribute* attribute;
   const _boo forced;
};


struct C_CreateDirectory_Stack : Command
{
public:
   C_CreateDirectory_Stack(Attribute* attr)
      : attribute(attr) { };

   void run() override;

private:
   Attribute* attribute;
};

// others:

struct C_Create_String : Command
{
public:
   C_Create_String(Generator<_str>* el, const _boo& forc)
      : element(el), forced(forc) { };

   ~C_Create_String() {
      delete element;
   }

   void run() override;

private:
   Generator<_str>* element;
   const _boo forced;
};


struct C_CreateFile_String : Command
{
public:
   C_CreateFile_String(Generator<_str>* el, const _boo& forc)
      : element(el), forced(forc) { };

   ~C_CreateFile_String() {
      delete element;
   }

   void run() override;

private:
   Generator<_str>* element;
   const _boo forced;
};


struct C_CreateDirectory_String : Command
{
public:
   C_CreateDirectory_String(Generator<_str>* el, const _boo& forc)
      : element(el), forced(forc) { };

   ~C_CreateDirectory_String() {
      delete element;
   }

   void run() override;

private:
   Generator<_str>* element;
   const _boo forced;
};


struct C_Create_String_Stack : Command
{
public:
   C_Create_String_Stack (Generator<_str>* el)
      : element(el) { };

   ~C_Create_String_Stack() {
      delete element;
   }

   void run() override;

private:
   Generator<_str>* element;
};


struct C_CreateFile_String_Stack  : Command
{
public:
   C_CreateFile_String_Stack (Generator<_str>* el)
      : element(el) { };

   ~C_CreateFile_String_Stack() {
      delete element;
   }

   void run() override;

private:
   Generator<_str>* element;
};


struct C_CreateDirectory_String_Stack  : Command
{
public:
   C_CreateDirectory_String_Stack (Generator<_str>* el)
      : element(el) { };

   ~C_CreateDirectory_String_Stack() {
      delete element;
   }

   void run() override;

private:
   Generator<_str>* element;
};


// others multi


struct C_Create_List : Command
{
public:
   C_Create_List(Generator<_list>* el, const _boo& forc)
      : elements(el), forced(forc) { };

   ~C_Create_List() {
      delete elements;
   }

   void run() override;

private:
   Generator<_list>* elements;
   const _boo forced;
};


struct C_CreateFiles_List : Command
{
public:
   C_CreateFiles_List(Generator<_list>* el, const _boo& forc)
      : elements(el), forced(forc) { };

   ~C_CreateFiles_List() {
      delete elements;
   }

   void run() override;

private:
   Generator<_list>* elements;
   const _boo forced;
};


struct C_CreateDirectories_List : Command
{
public:
   C_CreateDirectories_List(Generator<_list>* el, const _boo& forc)
      : elements(el), forced(forc) { };

   ~C_CreateDirectories_List() {
      delete elements;
   }

   void run() override;

private:
   Generator<_list>* elements;
   const _boo forced;
};


struct C_Create_List_Stack : Command
{
public:
   C_Create_List_Stack (Generator<_list>* el)
      : elements(el) { };

   ~C_Create_List_Stack() {
      delete elements;
   }

   void run() override;

private:
   Generator<_list>* elements;
};


struct C_CreateFiles_List_Stack  : Command
{
public:
   C_CreateFiles_List_Stack (Generator<_list>* el)
      : elements(el) { };

   ~C_CreateFiles_List_Stack() {
      delete elements;
   }

   void run() override;

private:
   Generator<_list>* elements;
};


struct C_CreateDirectories_List_Stack  : Command
{
public:
   C_CreateDirectories_List_Stack (Generator<_list>* el)
      : elements(el) { };

   ~C_CreateDirectories_List_Stack() {
      delete elements;
   }

   void run() override;

private:
   Generator<_list>* elements;
};





#endif // COM_CREATE_H_INCLUDED
