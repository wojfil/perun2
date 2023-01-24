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

#include "com-listener.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "../attribute.h"
#include "../uroboros.h"


namespace uro::comm
{

// simple commands:

struct C_Simple : Command_L
{
public:
   C_Simple(const _bool& save, _uro& uro)
      : saveChanges(save), Command_L(uro) { };

protected:
   const _bool saveChanges;
};

struct C_Delete : Command_L
{
public:
   C_Delete(_uro& uro)
      : attribute(nullptr), hasAttribute(false), Command_L(uro) { };
   C_Delete(Attribute* attr, _uro& uro)
      : attribute(attr), hasAttribute(true), Command_L(uro) { };

   void run() override;

private:
   Attribute* attribute;
   const _bool hasAttribute;
};

struct C_Drop : Command_L
{
public:
   C_Drop(_uro& uro)
      : attribute(nullptr), hasAttribute(false), Command_L(uro)  { };
   C_Drop(Attribute* attr, _uro& uro)
      : attribute(attr), hasAttribute(true), Command_L(uro) { };

   void run() override;

private:
   Attribute* attribute;
   const _bool hasAttribute;
};

struct C_Hide : C_Simple
{
   C_Hide(const _bool& save, _uro& uro)
      : C_Simple(save, uro) { };

   void run() override;
};

struct C_Lock : C_Simple
{
   C_Lock(const _bool& save, _uro& uro)
      : C_Simple(save, uro) { };

   void run() override;
};

struct C_Open : C_Simple
{
   C_Open(_uro& uro)
      : C_Simple(true, uro) { };

   void run() override;
};

struct C_Unlock : C_Simple
{
   C_Unlock(const _bool& save, _uro& uro)
      : C_Simple(save, uro) { };

   void run() override;
};

struct C_Unhide : C_Simple
{
   C_Unhide(const _bool& save, _uro& uro)
      : C_Simple(save, uro) { };

   void run() override;
};

struct C_OpenWith : Command_L
{
public:
   C_OpenWith(_genptr<_str>& pro, _uro& uro)
      : program(std::move(pro)), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> program;
};




// time commands:



struct C_TimeAlter : Command_L
{
public:
   C_TimeAlter(_genptr<_tim>& ti, const _bool& save, _uro& uro)
      : time(std::move(ti)), saveChanges(save), Command_L(uro) { };

protected:
   _genptr<_tim> time;
   const _bool saveChanges;
};


struct C_ReaccessTo : C_TimeAlter
{
   C_ReaccessTo(_genptr<_tim>& ti, const _bool& save, _uro& uro)
      : C_TimeAlter(ti, save, uro) { };

   void run() override;
};


struct C_RechangeTo : C_TimeAlter
{
   C_RechangeTo(_genptr<_tim>& ti, const _bool& save, _uro& uro)
      : C_TimeAlter(ti, save, uro) { };

   void run() override;
};


struct C_RecreateTo : C_TimeAlter
{
   C_RecreateTo(_genptr<_tim>& ti, const _bool& save, _uro& uro)
      : C_TimeAlter(ti, save, uro) { };

   void run() override;
};


struct C_RemodifyTo : C_TimeAlter
{
   C_RemodifyTo(_genptr<_tim>& ti, const _bool& save, _uro& uro)
      : C_TimeAlter(ti, save, uro) { };

   void run() override;
};


struct C_RenameTo : Command_L
{
public:
   C_RenameTo(_genptr<_str>& na, const _bool& save, const _bool& forc,
      const _bool& extless, _uro& uro)
      : name(std::move(na)), saveChanges(save), forced(forc), extensionless(extless), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> name;
   const _bool saveChanges;
   const _bool forced;
   const _bool extensionless;
};


struct C_RenameTo_Stack : Command_L
{
public:
   C_RenameTo_Stack(_genptr<_str>& na, const _bool& save, const _bool& extless, _uro& uro)
      : name(std::move(na)), saveChanges(save), extensionless(extless), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> name;
   const _bool saveChanges;
   const _bool extensionless;
};


struct C_MoveTo : Command_L
{
public:
   C_MoveTo(_genptr<_str>& loc, const _bool& forc, _uro& uro)
      : location(std::move(loc)), hasAttribute(false), attribute(nullptr), forced(forc), Command_L(uro) { };

   C_MoveTo(_genptr<_str>& loc, const _bool& forc, Attribute* attr, _uro& uro)
      : location(std::move(loc)), hasAttribute(true), attribute(attr), forced(forc), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   const _bool hasAttribute;
   Attribute* attribute;
   const _bool forced;
};


struct C_MoveTo_Stack : Command_L
{
public:
   C_MoveTo_Stack(_genptr<_str>& loc, _uro& uro)
      : location(std::move(loc)), hasAttribute(false), attribute(nullptr), Command_L(uro) { };

   C_MoveTo_Stack(_genptr<_str>& loc, Attribute* attr, _uro& uro)
      : location(std::move(loc)), hasAttribute(true), attribute(attr), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   const _bool hasAttribute;
   Attribute* attribute;
};


struct C_MoveToAs : Command_L
{
public:
   C_MoveToAs(_genptr<_str>& loc, _genptr<_str>& na, const _bool& forc,
      const _bool& extless, _uro& uro)
      : location(std::move(loc)), name(std::move(na)), hasAttribute(false),
        attribute(nullptr), forced(forc), extensionless(extless), Command_L(uro) { };

   C_MoveToAs(_genptr<_str>& loc, _genptr<_str>& na, const _bool& forc,
      const _bool& extless, Attribute* attr, _uro& uro)
      : location(std::move(loc)), name(std::move(na)), hasAttribute(true),
        attribute(attr), forced(forc), extensionless(extless), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_str> name;
   const _bool hasAttribute;
   Attribute* attribute;
   const _bool forced;
   const _bool extensionless;
};


struct C_MoveToAs_Stack : Command_L
{
public:
   C_MoveToAs_Stack(_genptr<_str>& loc, _genptr<_str>& na, const _bool& extless, _uro& uro)
      : location(std::move(loc)), name(std::move(na)), hasAttribute(false), attribute(nullptr), extensionless(extless), Command_L(uro) { };

   C_MoveToAs_Stack(_genptr<_str>& loc, _genptr<_str>& na, const _bool& extless, Attribute* attr, _uro& uro)
      : location(std::move(loc)), name(std::move(na)), hasAttribute(true), attribute(attr), extensionless(extless), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_str> name;
   const _bool hasAttribute;
   Attribute* attribute;
   const _bool extensionless;
};


struct C_DownloadFrom_String : Command_L
{
public:
   C_DownloadFrom_String(_genptr<_str>& loc, _genptr<_str>& el, const _bool& forc, _uro& uro)
      : location(std::move(loc)), element(std::move(el)), forced(forc), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_str> element;
   const _bool forced;
};


struct C_DownloadFrom_List : Command_L
{
public:
   C_DownloadFrom_List(_genptr<_str>& loc, _genptr<_list>& el, const _bool& forc, _uro& uro)
      : location(std::move(loc)), elements(std::move(el)), forced(forc), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_list> elements;
   const _bool forced;
};


struct C_DownloadFrom_Definition : Command_L
{
public:
   C_DownloadFrom_Definition(_genptr<_str>& loc, _defptr& el, const _bool& forc, _uro& uro)
      : location(std::move(loc)), elements(std::move(el)), forced(forc), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _defptr elements;
   const _bool forced;
};


struct C_DownloadFrom_String_Stack : Command_L
{
public:
   C_DownloadFrom_String_Stack(_genptr<_str>& loc, _genptr<_str>& el, _uro& uro)
      : location(std::move(loc)), element(std::move(el)), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_str> element;
};


struct C_DownloadFrom_List_Stack : Command_L
{
public:
   C_DownloadFrom_List_Stack(_genptr<_str>& loc, _genptr<_list>& el, _uro& uro)
      : location(std::move(loc)), elements(std::move(el)), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_list> elements;
};


struct C_DownloadFrom_Definition_Stack : Command_L
{
public:
   C_DownloadFrom_Definition_Stack(_genptr<_str>& loc, _defptr& el, _uro& uro)
      : location(std::move(loc)), elements(std::move(el)), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _defptr elements;
};


struct C_Download_String : Command_L
{
public:
   C_Download_String(_genptr<_str>& el, const _bool& forc, _uro& uro)
      : element(std::move(el)), forced(forc), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> element;
   const _bool forced;
};


struct C_Download_List : Command_L
{
public:
   C_Download_List(_genptr<_list>& el, const _bool& forc, _uro& uro)
      : elements(std::move(el)), forced(forc), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_list> elements;
   const _bool forced;
};


struct C_Download_String_Stack : Command_L
{
public:
   C_Download_String_Stack(_genptr<_str>& el, _uro& uro)
      : element(std::move(el)), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> element;
};


struct C_Download_List_Stack : Command_L
{
public:
   C_Download_List_Stack(_genptr<_list>& el, _uro& uro)
      : elements(std::move(el)), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_list> elements;
};


struct C_CopyTo : Command_L
{
public:
   C_CopyTo(_genptr<_str>& loc, const _bool& save,
      const _bool& forc, _uro& uro)
      : location(std::move(loc)), saveChanges(save), forced(forc), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   const _bool saveChanges;
   const _bool forced;
};


struct C_CopyTo_Stack : Command_L
{
public:
   C_CopyTo_Stack(_genptr<_str>& loc, const _bool& save, _uro& uro)
      : location(std::move(loc)), saveChanges(save), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   const _bool saveChanges;
};


struct C_CopyToAs : Command_L
{
public:
   C_CopyToAs(_genptr<_str>& loc, _genptr<_str>& na,
      const _bool& save, const _bool& forc, const _bool& extless, _uro& uro)
      : location(std::move(loc)), name(std::move(na)), forced(forc), extensionless(extless), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_str> name;
   const _bool forced;
   const _bool extensionless;
};


struct C_CopyToAs_Stack : Command_L
{
public:
   C_CopyToAs_Stack(_genptr<_str>& loc, _genptr<_str>& na,
      const _bool& save, const _bool& extless, _uro& uro)
      : location(std::move(loc)), name(std::move(na)), extensionless(extless), Command_L(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_str> name;
   const _bool extensionless;
};


_str getCCName(const _str& path);
_str getCCNameShort(const _str& path);

}

#endif /* COM_CORE_H */

