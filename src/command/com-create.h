/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Perun2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "com-core.h"
#include "com-listener.h"
#include "../attribute.h"
#include "../perun2.h"


namespace perun2::comm
{

struct C_Create : CoreCommand
{
public:
   C_Create(const p_bool force, FileContext* ctx, Perun2Process& p2)
      : forced(force), CoreCommand(true, ctx, p2) { };

   void run() override;

private:
   const p_bool forced;
};


struct C_Create_Stack : CoreCommand
{
public:
   C_Create_Stack(FileContext* ctx, Perun2Process& p2)
      : CoreCommand(true, ctx, p2) { };

   void run() override;
};


struct C_CreateFile : CoreCommand
{
public:
   C_CreateFile(const p_bool force, FileContext* ctx, Perun2Process& p2)
      : forced(force), CoreCommand(true, ctx, p2) { };

   void run() override;

private:
   const p_bool forced;
};


struct C_CreateFile_Stack : CoreCommand
{
public:
   C_CreateFile_Stack(FileContext* ctx, Perun2Process& p2)
      : CoreCommand(true, ctx, p2) { };

   void run() override;
};


struct C_CreateDirectory : CoreCommand
{
public:
   C_CreateDirectory(const p_bool force, FileContext* ctx, Perun2Process& p2)
      : forced(force), CoreCommand(true, ctx, p2) { };

   void run() override;

private:
   const p_bool forced;
};


struct C_CreateDirectory_Stack : CoreCommand
{
public:
   C_CreateDirectory_Stack(FileContext* ctx, Perun2Process& p2)
      : CoreCommand(true, ctx, p2) { };

   void run() override;
};

// others:


struct C_CreateCertain : Command
{
public:
   C_CreateCertain(Perun2Process& p2)
      : perun2(p2), locContext(p2.contexts.getLocationContext()) { };

protected:
   LocationContext* locContext;
   Perun2Process& perun2;
};


struct C_Create_String : C_CreateCertain
{
public:
   C_Create_String(p_genptr<p_str>& el, const p_bool forc, Perun2Process& p2)
      : element(std::move(el)), forced(forc), C_CreateCertain(p2) { };

   void run() override;

private:
   p_genptr<p_str> element;
   const p_bool forced;
};


struct C_CreateFile_String : C_CreateCertain
{
public:
   C_CreateFile_String(p_genptr<p_str>& el, const p_bool forc, Perun2Process& p2)
      : element(std::move(el)), forced(forc), C_CreateCertain(p2){ };

   void run() override;

private:
   p_genptr<p_str> element;
   const p_bool forced;
};


struct C_CreateDirectory_String : C_CreateCertain
{
public:
   C_CreateDirectory_String(p_genptr<p_str>& el, const p_bool forc, Perun2Process& p2)
      : element(std::move(el)), forced(forc), C_CreateCertain(p2) { };

   void run() override;

private:
   p_genptr<p_str> element;
   const p_bool forced;
};


struct C_Create_String_Stack : C_CreateCertain
{
public:
   C_Create_String_Stack (p_genptr<p_str>& el, Perun2Process& p2)
      : element(std::move(el)), C_CreateCertain(p2) { };

   void run() override;

private:
   p_genptr<p_str> element;
};


struct C_CreateFile_String_Stack  : C_CreateCertain
{
public:
   C_CreateFile_String_Stack (p_genptr<p_str>& el, Perun2Process& p2)
      : element(std::move(el)), C_CreateCertain(p2) { };

   void run() override;

private:
   p_genptr<p_str> element;
};


struct C_CreateDirectory_String_Stack  : C_CreateCertain
{
public:
   C_CreateDirectory_String_Stack (p_genptr<p_str>& el, Perun2Process& p2)
      : element(std::move(el)), C_CreateCertain(p2) { };

   void run() override;

private:
   p_genptr<p_str> element;
};


// others multi


struct C_Create_List : C_CreateCertain
{
public:
   C_Create_List(p_genptr<p_list>& el, const p_bool forc, Perun2Process& p2)
      : elements(std::move(el)), forced(forc), C_CreateCertain(p2) { };

   void run() override;

private:
   p_genptr<p_list> elements;
   const p_bool forced;
};


struct C_CreateFiles_List : C_CreateCertain
{
public:
   C_CreateFiles_List(p_genptr<p_list>& el, const p_bool forc, Perun2Process& p2)
      : elements(std::move(el)), forced(forc), C_CreateCertain(p2) { };

   void run() override;

private:
   p_genptr<p_list> elements;
   const p_bool forced;
};


struct C_CreateDirectories_List : C_CreateCertain
{
public:
   C_CreateDirectories_List(p_genptr<p_list>& el, const p_bool forc, Perun2Process& p2)
      : elements(std::move(el)), forced(forc), C_CreateCertain(p2) { };

   void run() override;

private:
   p_genptr<p_list> elements;
   const p_bool forced;
};


struct C_Create_List_Stack : C_CreateCertain
{
public:
   C_Create_List_Stack (p_genptr<p_list>& el, Perun2Process& p2)
      : elements(std::move(el)), C_CreateCertain(p2) { };

   void run() override;

private:
   p_genptr<p_list> elements;
};


struct C_CreateFiles_List_Stack : C_CreateCertain
{
public:
   C_CreateFiles_List_Stack (p_genptr<p_list>& el, Perun2Process& p2)
      : elements(std::move(el)), C_CreateCertain(p2) { };

   void run() override;

private:
   p_genptr<p_list> elements;
};


struct C_CreateDirectories_List_Stack  : C_CreateCertain
{
public:
   C_CreateDirectories_List_Stack (p_genptr<p_list>& el, Perun2Process& p2)
      : elements(std::move(el)), C_CreateCertain(p2) { };

   void run() override;

private:
   p_genptr<p_list> elements;
};

}
