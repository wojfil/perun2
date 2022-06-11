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

#ifndef COM_CORE_H
#define COM_CORE_H

#include "com.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "../attribute.h"




// simple commands:

struct C_Simple : Command
{
public:
   C_Simple(const _boo& save)
      : saveChanges(save) { };

protected:
   const _boo saveChanges;
};

struct C_Delete : Command
{
public:
   C_Delete() : attribute(nullptr), hasAttribute(false) { };
   C_Delete(Attribute* attr) : attribute(attr), hasAttribute(true) { };

   void run() override;

private:
   Attribute* attribute;
   const _boo hasAttribute;
};

struct C_Drop : Command
{
public:
   C_Drop() : attribute(nullptr), hasAttribute(false)  { };
   C_Drop(Attribute* attr) : attribute(attr), hasAttribute(true) { };

   void run() override;

private:
   Attribute* attribute;
   const _boo hasAttribute;
};

struct C_Hide : C_Simple
{
   C_Hide(const _boo& save)
      : C_Simple(save) { };

   void run() override;
};

struct C_Lock : C_Simple
{
   C_Lock(const _boo& save)
      : C_Simple(save) { };

   void run() override;
};

struct C_Open : C_Simple
{
   C_Open() : C_Simple(true) { };

   void run() override;
};

struct C_Unlock : C_Simple
{
   C_Unlock(const _boo& save)
      : C_Simple(save) { };

   void run() override;
};

struct C_Unhide : C_Simple
{
   C_Unhide(const _boo& save)
      : C_Simple(save) { };

   void run() override;
};

struct C_OpenWith : Command
{
public:
   C_OpenWith(Generator<_str>* pro) : program(pro){ };

   ~C_OpenWith() {
      delete program;
   }

   void run() override;

protected:
   Generator<_str>* program;
};




// time commands:



struct C_TimeAlter : Command
{
public:
   C_TimeAlter(Generator<_tim>* ti, const _boo& save)
      : time(ti), saveChanges(save) { };

   ~C_TimeAlter() {
      delete time;
   }

protected:
   Generator<_tim>* time;
   const _boo saveChanges;
};


struct C_ReaccessTo : C_TimeAlter
{
   C_ReaccessTo(Generator<_tim>* ti, const _boo& save)
      : C_TimeAlter(ti, save) { };
   void run() override;
};


struct C_RechangeTo : C_TimeAlter
{
   C_RechangeTo(Generator<_tim>* ti, const _boo& save)
      : C_TimeAlter(ti, save) { };
   void run() override;
};


struct C_RecreateTo : C_TimeAlter
{
   C_RecreateTo(Generator<_tim>* ti, const _boo& save)
      : C_TimeAlter(ti, save) { };
   void run() override;
};


struct C_RemodifyTo : C_TimeAlter
{
   C_RemodifyTo(Generator<_tim>* ti, const _boo& save)
      : C_TimeAlter(ti, save) { };
   void run() override;
};


struct C_RenameTo : Command
{
public:
   C_RenameTo(Generator<_str>* na, const _boo& save, const _boo& forc,
      const _boo& extless)
      : name(na), saveChanges(save), forced(forc), extensionless(extless) { };

   ~C_RenameTo() {
      delete name;
   }

   void run() override;

protected:
   Generator<_str>* name;
   const _boo saveChanges;
   const _boo forced;
   const _boo extensionless;
};


struct C_RenameTo_Stack : Command
{
public:
   C_RenameTo_Stack(Generator<_str>* na, const _boo& save, const _boo& extless)
      : name(na), saveChanges(save), extensionless(extless) { };

   ~C_RenameTo_Stack() {
      delete name;
   }

   void run() override;

protected:
   Generator<_str>* name;
   const _boo saveChanges;
   const _boo extensionless;
};


struct C_MoveTo : Command
{
public:
   C_MoveTo(Generator<_str>* loc, const _boo& forc)
      : location(loc), hasAttribute(false), attribute(nullptr), forced(forc) { };

   C_MoveTo(Generator<_str>* loc, const _boo& forc, Attribute* attr)
      : location(loc), hasAttribute(true), attribute(attr), forced(forc) { };

   ~C_MoveTo() {
      delete location;
   }

   void run() override;

protected:
   Generator<_str>* location;
   const _boo hasAttribute;
   Attribute* attribute;
   const _boo forced;
};


struct C_MoveTo_Stack : Command
{
public:
   C_MoveTo_Stack(Generator<_str>* loc)
      : location(loc), hasAttribute(false), attribute(nullptr) { };

   C_MoveTo_Stack(Generator<_str>* loc, Attribute* attr)
      : location(loc), hasAttribute(true), attribute(attr) { };

   ~C_MoveTo_Stack() {
      delete location;
   }

   void run() override;

protected:
   Generator<_str>* location;
   const _boo hasAttribute;
   Attribute* attribute;
};


struct C_MoveToAs : Command
{
public:
   C_MoveToAs(Generator<_str>* loc, Generator<_str>* na, const _boo& forc, const _boo& extless)
      : location(loc), name(na), hasAttribute(false), attribute(nullptr), forced(forc), extensionless(extless) { };

   C_MoveToAs(Generator<_str>* loc, Generator<_str>* na, const _boo& forc, const _boo& extless, Attribute* attr)
      : location(loc), name(na), hasAttribute(true), attribute(attr), forced(forc), extensionless(extless) { };

   ~C_MoveToAs() {
      delete location;
      delete name;
   }

   void run() override;

protected:
   Generator<_str>* location;
   Generator<_str>* name;
   const _boo hasAttribute;
   Attribute* attribute;
   const _boo forced;
   const _boo extensionless;
};


struct C_MoveToAs_Stack : Command
{
public:
   C_MoveToAs_Stack(Generator<_str>* loc, Generator<_str>* na, const _boo& extless)
      : location(loc), name(na), hasAttribute(false), attribute(nullptr), extensionless(extless) { };

   C_MoveToAs_Stack(Generator<_str>* loc, Generator<_str>* na, const _boo& extless, Attribute* attr)
      : location(loc), name(na), hasAttribute(true), attribute(attr), extensionless(extless) { };

   ~C_MoveToAs_Stack() {
      delete location;
      delete name;
   }

   void run() override;

protected:
   Generator<_str>* location;
   Generator<_str>* name;
   const _boo hasAttribute;
   Attribute* attribute;
   const _boo extensionless;
};


struct C_DownloadFrom_String : Command
{
public:
   C_DownloadFrom_String(Generator<_str>* loc, Generator<_str>* el, const _boo& forc)
      : location(loc), element(el), forced(forc) { };

   ~C_DownloadFrom_String() {
      delete location;
      delete element;
   }

   void run() override;

protected:
   Generator<_str>* location;
   Generator<_str>* element;
   const _boo forced;
};


struct C_DownloadFrom_List : Command
{
public:
   C_DownloadFrom_List(Generator<_str>* loc, Generator<_list>* el, const _boo& forc)
      : location(loc), elements(el), forced(forc) { };

   ~C_DownloadFrom_List() {
      delete location;
      delete elements;
   }

   void run() override;

protected:
   Generator<_str>* location;
   Generator<_list>* elements;
   const _boo forced;
};


struct C_DownloadFrom_Definition : Command
{
public:
   C_DownloadFrom_Definition(Generator<_str>* loc, _def* el, const _boo& forc)
      : location(loc), elements(el), forced(forc) { };

   ~C_DownloadFrom_Definition() {
      delete location;
      delete elements;
   }

   void run() override;

protected:
   Generator<_str>* location;
   _def* elements;
   const _boo forced;
};


struct C_DownloadFrom_String_Stack : Command
{
public:
   C_DownloadFrom_String_Stack(Generator<_str>* loc, Generator<_str>* el)
      : location(loc), element(el) { };

   ~C_DownloadFrom_String_Stack() {
      delete location;
      delete element;
   }

   void run() override;

protected:
   Generator<_str>* location;
   Generator<_str>* element;
};


struct C_DownloadFrom_List_Stack : Command
{
public:
   C_DownloadFrom_List_Stack(Generator<_str>* loc, Generator<_list>* el)
      : location(loc), elements(el) { };

   ~C_DownloadFrom_List_Stack() {
      delete location;
      delete elements;
   }

   void run() override;

protected:
   Generator<_str>* location;
   Generator<_list>* elements;
};


struct C_DownloadFrom_Definition_Stack : Command
{
public:
   C_DownloadFrom_Definition_Stack(Generator<_str>* loc, _def* el)
      : location(loc), elements(el) { };

   ~C_DownloadFrom_Definition_Stack() {
      delete location;
      delete elements;
   }

   void run() override;

protected:
   Generator<_str>* location;
   _def* elements;
};


struct C_Download_String : Command
{
public:
   C_Download_String(Generator<_str>* el, const _boo& forc)
      : element(el), forced(forc) { };

   ~C_Download_String() {
      delete element;
   }

   void run() override;

protected:
   Generator<_str>* element;
   const _boo forced;
};


struct C_Download_List : Command
{
public:
   C_Download_List(Generator<_list>* el, const _boo& forc)
      : elements(el), forced(forc) { };

   ~C_Download_List() {
      delete elements;
   }

   void run() override;

protected:
   Generator<_list>* elements;
   const _boo forced;
};


struct C_Download_String_Stack : Command
{
public:
   C_Download_String_Stack(Generator<_str>* el) : element(el) { };

   ~C_Download_String_Stack() {
      delete element;
   }

   void run() override;

protected:
   Generator<_str>* element;
};


struct C_Download_List_Stack : Command
{
public:
   C_Download_List_Stack(Generator<_list>* el) : elements(el) { };

   ~C_Download_List_Stack() {
      delete elements;
   }

   void run() override;

protected:
   Generator<_list>* elements;
};


struct C_CopyTo : Command
{
public:
   C_CopyTo(Generator<_str>* loc, const _boo& save, const _boo& forc)
      : location(loc), saveChanges(save), forced(forc) { };

   ~C_CopyTo() {
      delete location;
   }

   void run() override;

protected:
   Generator<_str>* location;
   const _boo saveChanges;
   const _boo forced;
};


struct C_CopyTo_Stack : Command
{
public:
   C_CopyTo_Stack(Generator<_str>* loc, const _boo& save)
      : location(loc), saveChanges(save){ };

   ~C_CopyTo_Stack() {
      delete location;
   }

   void run() override;

protected:
   Generator<_str>* location;
   const _boo saveChanges;
};


struct C_CopyToAs : Command
{
public:
   C_CopyToAs(Generator<_str>* loc, Generator<_str>* na,
      const _boo& save, const _boo& forc, const _boo& extless)
      : location(loc), name(na), forced(forc), extensionless(extless) { };

   ~C_CopyToAs() {
      delete location;
      delete name;
   }

   void run() override;

protected:
   Generator<_str>* location;
   Generator<_str>* name;
   const _boo forced;
   const _boo extensionless;
};


struct C_CopyToAs_Stack : Command
{
public:
   C_CopyToAs_Stack(Generator<_str>* loc, Generator<_str>* na,
      const _boo& save, const _boo& extless)
      : location(loc), name(na), extensionless(extless) { };

   ~C_CopyToAs_Stack() {
      delete location;
      delete name;
   }

   void run() override;

protected:
   Generator<_str>* location;
   Generator<_str>* name;
   const _boo extensionless;
};


_str getCCName(const _str& path);
_str getCCNameShort(const _str& path);


#endif /* COM_CORE_H */

