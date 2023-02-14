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

#ifndef COM_CORE_H
#define COM_CORE_H

#include "com-listener.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "../attribute.h"
#include "../uroboros.h"


namespace uro::comm
{

struct CoreCommand : Command
{
public:
   CoreCommand() = delete;
   CoreCommand(FileContext* ctx, _uro& uro)
      : saveChanges(true), context(ctx), uroboros(uro), locationContext(uro.contexts.getLocationContext()) { };
   CoreCommand(const _bool& save, FileContext* ctx, _uro& uro)
      : saveChanges(save), context(ctx), uroboros(uro), locationContext(uro.contexts.getLocationContext()) { };

protected:

   FileContext* context;
   const _bool saveChanges;
   _uro& uroboros;
   LocationContext const* locationContext;
};

struct C_Delete : CoreCommand
{
public:
   C_Delete(const _bool& save, FileContext* ctx, _uro& uro)
      : CoreCommand(save, ctx, uro) { };

   void run() override;
};

struct C_Drop : CoreCommand
{
public:
   C_Drop(const _bool& save, FileContext* ctx, _uro& uro)
      : CoreCommand(save, ctx, uro) { };

   void run() override;
};

struct C_Hide : CoreCommand
{
   C_Hide(const _bool& save, FileContext* ctx, _uro& uro)
      : CoreCommand(save, ctx, uro) { };

   void run() override;
};

struct C_Lock : CoreCommand
{
   C_Lock(const _bool& save, FileContext* ctx, _uro& uro)
      : CoreCommand(save, ctx, uro) { };

   void run() override;
};

struct C_Open : CoreCommand
{
   C_Open(const _bool& save, FileContext* ctx, _uro& uro)
      : CoreCommand(save, ctx, uro) { };

   void run() override;
};

struct C_Unlock : CoreCommand
{
   C_Unlock(const _bool& save, FileContext* ctx, _uro& uro)
      : CoreCommand(save, ctx, uro) { };

   void run() override;
};

struct C_Unhide : CoreCommand
{
   C_Unhide(const _bool& save, FileContext* ctx, _uro& uro)
      : CoreCommand(save, ctx, uro) { };

   void run() override;
};

struct C_OpenWith : CoreCommand
{
public:
   C_OpenWith(_genptr<_str>& pro, const _bool& save, FileContext* ctx, _uro& uro)
      : program(std::move(pro)), CoreCommand(save, ctx, uro) { };

   void run() override;

protected:
   _genptr<_str> program;
};




// time commands:



struct C_TimeAlter : CoreCommand
{
public:
   C_TimeAlter(_genptr<_tim>& ti, const _bool& save, FileContext* ctx, _uro& uro)
      : time(std::move(ti)), CoreCommand(save, ctx, uro) { };

protected:
   _genptr<_tim> time;
};


struct C_ReaccessTo : C_TimeAlter
{
   C_ReaccessTo(_genptr<_tim>& ti, const _bool& save, FileContext* ctx, _uro& uro)
      : C_TimeAlter(ti, save, ctx, uro) { };

   void run() override;
};


struct C_RechangeTo : C_TimeAlter
{
   C_RechangeTo(_genptr<_tim>& ti, const _bool& save, FileContext* ctx, _uro& uro)
      : C_TimeAlter(ti, save, ctx, uro) { };

   void run() override;
};


struct C_RecreateTo : C_TimeAlter
{
   C_RecreateTo(_genptr<_tim>& ti, const _bool& save, FileContext* ctx, _uro& uro)
      : C_TimeAlter(ti, save, ctx, uro) { };

   void run() override;
};


struct C_RemodifyTo : C_TimeAlter
{
   C_RemodifyTo(_genptr<_tim>& ti, const _bool& save, FileContext* ctx, _uro& uro)
      : C_TimeAlter(ti, save, ctx, uro) { };

   void run() override;
};


struct C_RenameTo : CoreCommand
{
public:
   C_RenameTo(_genptr<_str>& na, const _bool& save, const _bool& forc,
      const _bool& extless, FileContext* ctx, _uro& uro)
      : name(std::move(na)), saveChanges(save), forced(forc), extensionless(extless), CoreCommand(save, ctx, uro) { };

   void run() override;

protected:
   _genptr<_str> name;
   const _bool saveChanges;
   const _bool forced;
   const _bool extensionless;
};


struct C_RenameTo_Stack : CoreCommand
{
public:
   C_RenameTo_Stack(_genptr<_str>& na, const _bool& save, const _bool& extless, FileContext* ctx, _uro& uro)
      : name(std::move(na)), saveChanges(save), extensionless(extless), CoreCommand(save, ctx, uro) { };

   void run() override;

protected:
   _genptr<_str> name;
   const _bool saveChanges;
   const _bool extensionless;
};


struct C_MoveTo : CoreCommand
{
public:
   C_MoveTo(_genptr<_str>& loc, const _bool& forc, FileContext* ctx, _uro& uro)
      : location(std::move(loc)), forced(forc), CoreCommand(ctx, uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   const _bool forced;
};


struct C_MoveTo_Stack : CoreCommand
{
public:
   C_MoveTo_Stack(_genptr<_str>& loc, FileContext* ctx, _uro& uro)
      : location(std::move(loc)), CoreCommand(ctx, uro) { };

   void run() override;

protected:
   _genptr<_str> location;
};


struct C_MoveToAs : CoreCommand
{
public:
   C_MoveToAs(_genptr<_str>& loc, _genptr<_str>& na, const _bool& forc,
      const _bool& extless, FileContext* ctx, _uro& uro)
      : location(std::move(loc)), name(std::move(na)),
        forced(forc), extensionless(extless), CoreCommand(ctx, uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_str> name;
   const _bool forced;
   const _bool extensionless;
};


struct C_MoveToAs_Stack : CoreCommand
{
public:
   C_MoveToAs_Stack(_genptr<_str>& loc, _genptr<_str>& na, const _bool& extless, FileContext* ctx, _uro& uro)
      : location(std::move(loc)), name(std::move(na)), extensionless(extless), CoreCommand(ctx, uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_str> name;
   const _bool extensionless;
};


struct C_DownloadBase : Command
{
public:
   C_DownloadBase(_uro& uro)
      : uroboros(uro), locContext(uro.contexts.getLocationContext()) { };

protected:
   LocationContext* locContext;
   _uro& uroboros;
};


struct C_DownloadFrom_String : C_DownloadBase
{
public:
   C_DownloadFrom_String(_genptr<_str>& loc, _genptr<_str>& el, const _bool& forc, _uro& uro)
      : location(std::move(loc)), element(std::move(el)), forced(forc), C_DownloadBase(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_str> element;
   const _bool forced;
};


struct C_DownloadFrom_List : C_DownloadBase
{
public:
   C_DownloadFrom_List(_genptr<_str>& loc, _genptr<_list>& el, const _bool& forc, _uro& uro)
      : location(std::move(loc)), elements(std::move(el)), forced(forc), C_DownloadBase(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_list> elements;
   const _bool forced;
};


struct C_DownloadFrom_Definition : C_DownloadBase
{
public:
   C_DownloadFrom_Definition(_genptr<_str>& loc, _defptr& el, const _bool& forc, _uro& uro)
      : location(std::move(loc)), elements(std::move(el)), forced(forc), C_DownloadBase(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _defptr elements;
   const _bool forced;
};


struct C_DownloadFrom_String_Stack : C_DownloadBase
{
public:
   C_DownloadFrom_String_Stack(_genptr<_str>& loc, _genptr<_str>& el, _uro& uro)
      : location(std::move(loc)), element(std::move(el)), C_DownloadBase(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_str> element;
};


struct C_DownloadFrom_List_Stack : C_DownloadBase
{
public:
   C_DownloadFrom_List_Stack(_genptr<_str>& loc, _genptr<_list>& el, _uro& uro)
      : location(std::move(loc)), elements(std::move(el)), C_DownloadBase(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_list> elements;
};


struct C_DownloadFrom_Definition_Stack : C_DownloadBase
{
public:
   C_DownloadFrom_Definition_Stack(_genptr<_str>& loc, _defptr& el, _uro& uro)
      : location(std::move(loc)), elements(std::move(el)), C_DownloadBase(uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _defptr elements;
};


struct C_Download_String : C_DownloadBase
{
public:
   C_Download_String(_genptr<_str>& el, const _bool& forc, _uro& uro)
      : element(std::move(el)), forced(forc), C_DownloadBase(uro) { };

   void run() override;

protected:
   _genptr<_str> element;
   const _bool forced;
};


struct C_Download_List : C_DownloadBase
{
public:
   C_Download_List(_genptr<_list>& el, const _bool& forc, _uro& uro)
      : elements(std::move(el)), forced(forc), C_DownloadBase(uro) { };

   void run() override;

protected:
   _genptr<_list> elements;
   const _bool forced;
};


struct C_Download_String_Stack : C_DownloadBase
{
public:
   C_Download_String_Stack(_genptr<_str>& el, _uro& uro)
      : element(std::move(el)), C_DownloadBase(uro) { };

   void run() override;

protected:
   _genptr<_str> element;
};


struct C_Download_List_Stack : C_DownloadBase
{
public:
   C_Download_List_Stack(_genptr<_list>& el, _uro& uro)
      : elements(std::move(el)), C_DownloadBase(uro) { };

   void run() override;

protected:
   _genptr<_list> elements;
};


struct C_CopyTo : CoreCommand
{
public:
   C_CopyTo(_genptr<_str>& loc, const _bool& save,
      const _bool& forc, FileContext* ctx, _uro& uro)
      : location(std::move(loc)), saveChanges(save), forced(forc), CoreCommand(save, ctx, uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   const _bool saveChanges;
   const _bool forced;
};


struct C_CopyTo_Stack : CoreCommand
{
public:
   C_CopyTo_Stack(_genptr<_str>& loc, const _bool& save, FileContext* ctx, _uro& uro)
      : location(std::move(loc)), saveChanges(save), CoreCommand(save, ctx, uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   const _bool saveChanges;
};


struct C_CopyToAs : CoreCommand
{
public:
   C_CopyToAs(_genptr<_str>& loc, _genptr<_str>& na,
      const _bool& save, const _bool& forc, const _bool& extless, FileContext* ctx, _uro& uro)
      : location(std::move(loc)), name(std::move(na)), forced(forc), extensionless(extless), CoreCommand(save, ctx, uro) { };

   void run() override;

protected:
   _genptr<_str> location;
   _genptr<_str> name;
   const _bool forced;
   const _bool extensionless;
};


struct C_CopyToAs_Stack : CoreCommand
{
public:
   C_CopyToAs_Stack(_genptr<_str>& loc, _genptr<_str>& na,
      const _bool& save, const _bool& extless, FileContext* ctx, _uro& uro)
      : location(std::move(loc)), name(std::move(na)), extensionless(extless), CoreCommand(save, ctx, uro) { };

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

