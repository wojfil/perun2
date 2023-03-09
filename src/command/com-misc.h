/*
    This file is part of Perun2.
    Perun2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Peruns2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Perun2. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COM_MISC_H
#define COM_MISC_H

#include "com-listener.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "com-aggregate.h"
#include "com-core.h"
#include "../attribute.h"
#include "../perun2.h"


namespace perun2::comm
{

struct C_PrintSingle : Command
{
public:
   C_PrintSingle(_genptr<_str>& val, _p2& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   _p2& perun2;
   _genptr<_str> value;
};


struct C_PrintList : Command
{
public:
   C_PrintList(_genptr<_list>& val, _p2& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   _p2& perun2;
   _genptr<_list> value;
};


struct C_PrintDefinition : Command
{
public:
   C_PrintDefinition(_defptr& val, _p2& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   _p2& perun2;
   _defptr value;
};


struct C_PrintThis : Command
{
public:
   C_PrintThis(_p2& p2, FileContext& ctx)
      : perun2(p2), context(ctx) { };
   void run() override;

private:
   FileContext& context;
   _p2& perun2;
};


struct C_SleepPeriod : Command
{
public:
   C_SleepPeriod(_genptr<_per>& val, _p2& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   _p2& perun2;
   _genptr<_per> value;
};


struct C_SleepMs : Command
{
public:
   C_SleepMs(_genptr<_num>& val, _p2& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   _p2& perun2;
   _genptr<_num> value;
};


struct C_Break : Command
{
public:
   C_Break(_p2& p2)
      : perun2(p2) { };
   void run() override;

private:
   _p2& perun2;
};


struct C_Continue : Command
{
public:
   C_Continue(_p2& p2)
      : perun2(p2) { };
   void run() override;

private:
   _p2& perun2;
};


struct C_Exit : Command
{
public:
   C_Exit(_p2& p2)
      : perun2(p2) { };
   void run() override;

private:
   _p2& perun2;
};


struct C_Error : Command
{
public:
   C_Error(_p2& p2)
      : perun2(p2) { };
   void run() override;

private:
   _p2& perun2;
};


struct C_ErrorWithExitCode : Command
{
public:
   C_ErrorWithExitCode(_genptr<_num>& code, _p2& p2)
      : exitCode(std::move(code)), perun2(p2) { };

   void run() override;

private:
   _p2& perun2;
   _genptr<_num> exitCode;
};


struct RunBase
{
public:
   RunBase() = delete;
   RunBase(_p2& p2);

protected:
   void reloadContexts();
   _str getLocation();
   _str getUro2Cmd();

   const _str uro2Base;

private:
   LocationContext* locationCtx;
   std::vector<FileContext*> fileCtxs;
};


struct C_Run : CoreCommand, RunBase
{
public:
   C_Run(_genptr<_str>& val, FileContext* ctx, _p2& p2)
      : CoreCommand(ctx, p2), RunBase(p2), value(std::move(val)) { };

   void run() override;

private:
   _genptr<_str> value;
};


struct C_RunWith : CoreCommand, RunBase
{
public:
   C_RunWith(_genptr<_str>& val, FileContext* ctx, _p2& p2)
      : CoreCommand(ctx, p2), RunBase(p2), value(std::move(val)) { };

   void run() override;

private:
   _genptr<_str> value;
};


struct C_RunWithWithString : CoreCommand, RunBase
{
public:
   C_RunWithWithString(_genptr<_str>& val, _genptr<_str>& arg, FileContext* ctx, _p2& p2)
      : CoreCommand(ctx, p2), RunBase(p2), value(std::move(val)), argument(std::move(arg)) { };

   void run() override;

private:
   _genptr<_str> value;
   _genptr<_str> argument;
};


struct C_RunWithWith : CoreCommand, RunBase
{
public:
   C_RunWithWith(_genptr<_str>& val, _genptr<_list>& arg, FileContext* ctx, _p2& p2)
      : CoreCommand(ctx, p2), RunBase(p2), value(std::move(val)), arguments(std::move(arg)) { };

   void run() override;

private:
   _genptr<_str> value;
   _genptr<_list> arguments;
};


struct C_RunWithPerun2 : CoreCommand, RunBase
{
public:
   C_RunWithPerun2(FileContext* ctx, _p2& p2)
      : CoreCommand(ctx, p2), RunBase(p2) { };

   void run() override;
};


struct C_RunWithPerun2WithString : CoreCommand, RunBase
{
public:
   C_RunWithPerun2WithString(_genptr<_str>& arg, FileContext* ctx, _p2& p2)
      : CoreCommand(ctx, p2), RunBase(p2), argument(std::move(arg)) { };

   void run() override;

private:
   _genptr<_str> argument;
};


struct C_RunWithPerun2With : CoreCommand, RunBase
{
public:
   C_RunWithPerun2With(_genptr<_list>& arg, FileContext* ctx, _p2& p2)
      : CoreCommand(ctx, p2), RunBase(p2), arguments(std::move(arg)) { };

   void run() override;

private:
   _genptr<_list> arguments;
};

}

#endif /* COM_MISC_H */

