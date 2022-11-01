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
#include "com-listener.h"
#include "../attribute.h"
#include "../uroboros.h"


namespace uro::comm
{

struct C_Create : Command_L
{
public:
   C_Create(const _bool& force, Attribute* attr, Uroboros& uro)
      : forced(force), attribute(attr), Command_L(uro) { };

   void run() override;

private:
   const _bool forced;
   Attribute* attribute; // memory is freed in another object
};


struct C_Create_Stack : Command_L
{
public:
   C_Create_Stack(Attribute* attr, Uroboros& uro)
      : attribute(attr), Command_L(uro) { };

   void run() override;

private:
   Attribute* attribute;
};


struct C_CreateFile : Command_L
{
public:
   C_CreateFile(const _bool& force, Attribute* attr, Uroboros& uro)
      : forced(force), attribute(attr), Command_L(uro) { };

   void run() override;

private:
   const _bool forced;
   Attribute* attribute;
};


struct C_CreateFile_Stack : Command_L
{
public:
   C_CreateFile_Stack(Attribute* attr, Uroboros& uro)
      : attribute(attr), Command_L(uro) { };

   void run() override;

private:
   Attribute* attribute;
};


struct C_CreateDirectory : Command_L
{
public:
   C_CreateDirectory(const _bool& force, Attribute* attr, Uroboros& uro)
      : forced(force), attribute(attr), Command_L(uro) { };

   void run() override;

private:
   Attribute* attribute;
   const _bool forced;
};


struct C_CreateDirectory_Stack : Command_L
{
public:
   C_CreateDirectory_Stack(Attribute* attr, Uroboros& uro)
      : attribute(attr), Command_L(uro) { };

   void run() override;

private:
   Attribute* attribute;
};

// others:

struct C_Create_String : Command_L
{
public:
   C_Create_String(_genptr<_str>& el, const _bool& forc, Uroboros& uro)
      : element(std::move(el)), forced(forc), Command_L(uro) { };

   void run() override;

private:
   _genptr<_str> element;
   const _bool forced;
};


struct C_CreateFile_String : Command_L
{
public:
   C_CreateFile_String(_genptr<_str>& el, const _bool& forc, Uroboros& uro)
      : element(std::move(el)), forced(forc), Command_L(uro) { };

   void run() override;

private:
   _genptr<_str> element;
   const _bool forced;
};


struct C_CreateDirectory_String : Command_L
{
public:
   C_CreateDirectory_String(_genptr<_str>& el, const _bool& forc, Uroboros& uro)
      : element(std::move(el)), forced(forc), Command_L(uro) { };

   void run() override;

private:
   _genptr<_str> element;
   const _bool forced;
};


struct C_Create_String_Stack : Command_L
{
public:
   C_Create_String_Stack (_genptr<_str>& el, Uroboros& uro)
      : element(std::move(el)), Command_L(uro) { };

   void run() override;

private:
   _genptr<_str> element;
};


struct C_CreateFile_String_Stack  : Command_L
{
public:
   C_CreateFile_String_Stack (_genptr<_str>& el, Uroboros& uro)
      : element(std::move(el)), Command_L(uro) { };

   void run() override;

private:
   _genptr<_str> element;
};


struct C_CreateDirectory_String_Stack  : Command_L
{
public:
   C_CreateDirectory_String_Stack (_genptr<_str>& el, Uroboros& uro)
      : element(std::move(el)), Command_L(uro) { };

   void run() override;

private:
   _genptr<_str> element;
};


// others multi


struct C_Create_List : Command_L
{
public:
   C_Create_List(_genptr<_list>& el, const _bool& forc, Uroboros& uro)
      : elements(std::move(el)), forced(forc), Command_L(uro) { };

   void run() override;

private:
   _genptr<_list> elements;
   const _bool forced;
};


struct C_CreateFiles_List : Command_L
{
public:
   C_CreateFiles_List(_genptr<_list>& el, const _bool& forc, Uroboros& uro)
      : elements(std::move(el)), forced(forc), Command_L(uro) { };

   void run() override;

private:
   _genptr<_list> elements;
   const _bool forced;
};


struct C_CreateDirectories_List : Command_L
{
public:
   C_CreateDirectories_List(_genptr<_list>& el, const _bool& forc, Uroboros& uro)
      : elements(std::move(el)), forced(forc), Command_L(uro) { };

   void run() override;

private:
   _genptr<_list> elements;
   const _bool forced;
};


struct C_Create_List_Stack : Command_L
{
public:
   C_Create_List_Stack (_genptr<_list>& el, Uroboros& uro)
      : elements(std::move(el)), Command_L(uro) { };

   void run() override;

private:
   _genptr<_list> elements;
};


struct C_CreateFiles_List_Stack  : Command_L
{
public:
   C_CreateFiles_List_Stack (_genptr<_list>& el, Uroboros& uro)
      : elements(std::move(el)), Command_L(uro) { };

   void run() override;

private:
   _genptr<_list> elements;
};


struct C_CreateDirectories_List_Stack  : Command_L
{
public:
   C_CreateDirectories_List_Stack (_genptr<_list>& el, Uroboros& uro)
      : elements(std::move(el)), Command_L(uro) { };

   void run() override;

private:
   _genptr<_list> elements;
};

}

#endif // COM_CREATE_H_INCLUDED
