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

#ifndef COM_CREATE_H_INCLUDED
#define COM_CREATE_H_INCLUDED

#include "com-core.h"
#include "com-listener.h"
#include "../attribute.h"
#include "../uroboros.h"


namespace uro::comm
{

struct C_Create : CoreCommand
{
public:
   C_Create(const _bool& force, FileContext* ctx, _uro& uro)
      : forced(force), CoreCommand(true, ctx, uro) { };

   void run() override;

private:
   const _bool forced;
};


struct C_Create_Stack : CoreCommand
{
public:
   C_Create_Stack(FileContext* ctx, _uro& uro)
      : CoreCommand(true, ctx, uro) { };

   void run() override;
};


struct C_CreateFile : CoreCommand
{
public:
   C_CreateFile(const _bool& force, FileContext* ctx, _uro& uro)
      : forced(force), CoreCommand(true, ctx, uro) { };

   void run() override;

private:
   const _bool forced;
};


struct C_CreateFile_Stack : CoreCommand
{
public:
   C_CreateFile_Stack(FileContext* ctx, _uro& uro)
      : CoreCommand(true, ctx, uro) { };

   void run() override;
};


struct C_CreateDirectory : CoreCommand
{
public:
   C_CreateDirectory(const _bool& force, FileContext* ctx, _uro& uro)
      : forced(force), CoreCommand(true, ctx, uro) { };

   void run() override;

private:
   const _bool forced;
};


struct C_CreateDirectory_Stack : CoreCommand
{
public:
   C_CreateDirectory_Stack(FileContext* ctx, _uro& uro)
      : CoreCommand(true, ctx, uro) { };

   void run() override;
};

// others:


struct C_CreateCertain : Command
{
public:
   C_CreateCertain(_uro& uro)
      : uroboros(uro), locContext(uro.contextes.getLocationContext()) { };

protected:
   LocationContext* locContext;
   _uro& uroboros;
};


struct C_Create_String : C_CreateCertain
{
public:
   C_Create_String(_genptr<_str>& el, const _bool& forc, _uro& uro)
      : element(std::move(el)), forced(forc), C_CreateCertain(uro) { };

   void run() override;

private:
   _genptr<_str> element;
   const _bool forced;
};


struct C_CreateFile_String : C_CreateCertain
{
public:
   C_CreateFile_String(_genptr<_str>& el, const _bool& forc, _uro& uro)
      : element(std::move(el)), forced(forc), C_CreateCertain(uro){ };

   void run() override;

private:
   _genptr<_str> element;
   const _bool forced;
};


struct C_CreateDirectory_String : C_CreateCertain
{
public:
   C_CreateDirectory_String(_genptr<_str>& el, const _bool& forc, _uro& uro)
      : element(std::move(el)), forced(forc), C_CreateCertain(uro) { };

   void run() override;

private:
   _genptr<_str> element;
   const _bool forced;
};


struct C_Create_String_Stack : C_CreateCertain
{
public:
   C_Create_String_Stack (_genptr<_str>& el, _uro& uro)
      : element(std::move(el)), C_CreateCertain(uro) { };

   void run() override;

private:
   _genptr<_str> element;
};


struct C_CreateFile_String_Stack  : C_CreateCertain
{
public:
   C_CreateFile_String_Stack (_genptr<_str>& el, _uro& uro)
      : element(std::move(el)), C_CreateCertain(uro) { };

   void run() override;

private:
   _genptr<_str> element;
};


struct C_CreateDirectory_String_Stack  : C_CreateCertain
{
public:
   C_CreateDirectory_String_Stack (_genptr<_str>& el, _uro& uro)
      : element(std::move(el)), C_CreateCertain(uro) { };

   void run() override;

private:
   _genptr<_str> element;
};


// others multi


struct C_Create_List : C_CreateCertain
{
public:
   C_Create_List(_genptr<_list>& el, const _bool& forc, _uro& uro)
      : elements(std::move(el)), forced(forc), C_CreateCertain(uro) { };

   void run() override;

private:
   _genptr<_list> elements;
   const _bool forced;
};


struct C_CreateFiles_List : C_CreateCertain
{
public:
   C_CreateFiles_List(_genptr<_list>& el, const _bool& forc, _uro& uro)
      : elements(std::move(el)), forced(forc), C_CreateCertain(uro) { };

   void run() override;

private:
   _genptr<_list> elements;
   const _bool forced;
};


struct C_CreateDirectories_List : C_CreateCertain
{
public:
   C_CreateDirectories_List(_genptr<_list>& el, const _bool& forc, _uro& uro)
      : elements(std::move(el)), forced(forc), C_CreateCertain(uro) { };

   void run() override;

private:
   _genptr<_list> elements;
   const _bool forced;
};


struct C_Create_List_Stack : C_CreateCertain
{
public:
   C_Create_List_Stack (_genptr<_list>& el, _uro& uro)
      : elements(std::move(el)), C_CreateCertain(uro) { };

   void run() override;

private:
   _genptr<_list> elements;
};


struct C_CreateFiles_List_Stack : C_CreateCertain
{
public:
   C_CreateFiles_List_Stack (_genptr<_list>& el, _uro& uro)
      : elements(std::move(el)), C_CreateCertain(uro) { };

   void run() override;

private:
   _genptr<_list> elements;
};


struct C_CreateDirectories_List_Stack  : C_CreateCertain
{
public:
   C_CreateDirectories_List_Stack (_genptr<_list>& el, _uro& uro)
      : elements(std::move(el)), C_CreateCertain(uro) { };

   void run() override;

private:
   _genptr<_list> elements;
};

}

#endif // COM_CREATE_H_INCLUDED
