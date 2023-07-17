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
   C_PrintSingle(_genptr<_str>& val, p_perun2& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   p_perun2& perun2;
   _genptr<_str> value;
};


struct C_PrintList : Command
{
public:
   C_PrintList(_genptr<_list>& val, p_perun2& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   p_perun2& perun2;
   _genptr<_list> value;
};


struct C_PrintDefinition : Command
{
public:
   C_PrintDefinition(_defptr& val, p_perun2& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   p_perun2& perun2;
   _defptr value;
};


struct C_PrintThis : Command
{
public:
   C_PrintThis(p_perun2& p2, FileContext& ctx)
      : perun2(p2), context(ctx) { };
   void run() override;

private:
   FileContext& context;
   p_perun2& perun2;
};


struct C_SleepPeriod : Command
{
public:
   C_SleepPeriod(_genptr<_per>& val, p_perun2& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   p_perun2& perun2;
   _genptr<_per> value;
};


struct C_SleepMs : Command
{
public:
   C_SleepMs(_genptr<_num>& val, p_perun2& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   p_perun2& perun2;
   _genptr<_num> value;
};


struct C_Break : Command
{
public:
   C_Break(p_perun2& p2)
      : perun2(p2) { };
   void run() override;

private:
   p_perun2& perun2;
};


struct C_Continue : Command
{
public:
   C_Continue(p_perun2& p2)
      : perun2(p2) { };
   void run() override;

private:
   p_perun2& perun2;
};


struct C_Exit : Command
{
public:
   C_Exit(p_perun2& p2)
      : perun2(p2) { };
   void run() override;

private:
   p_perun2& perun2;
};


struct C_Error : Command
{
public:
   C_Error(p_perun2& p2)
      : perun2(p2) { };
   void run() override;

private:
   p_perun2& perun2;
};


struct C_ErrorWithExitCode : Command
{
public:
   C_ErrorWithExitCode(_genptr<_num>& code, p_perun2& p2)
      : exitCode(std::move(code)), perun2(p2) { };

   void run() override;

private:
   p_perun2& perun2;
   _genptr<_num> exitCode;
};


struct RunBase
{
public:
   RunBase() = delete;
   RunBase(p_perun2& p2);

protected:
   _str getLocation();
   
   p_perun2& perun2;

private:
   LocationContext* locationCtx;
};


struct C_Run : Command, RunBase
{
public:
   C_Run(_genptr<_str>& val, p_perun2& p2)
      : RunBase(p2), value(std::move(val)) { };

   void run() override;

private:
   _genptr<_str> value;
};


struct C_RunWith : Command, RunBase
{
public:
   C_RunWith(_genptr<_str>& val, FileContext* ctx, p_perun2& p2)
      : context(ctx), RunBase(p2), value(std::move(val)) { };

   void run() override;

private:
   _genptr<_str> value;
   FileContext* context;
};


struct C_RunWithWithString : Command, RunBase
{
public:
   C_RunWithWithString(_genptr<_str>& val, _genptr<_str>& arg, FileContext* ctx, p_perun2& p2)
      : context(ctx), RunBase(p2), value(std::move(val)), argument(std::move(arg)) { };

   void run() override;

private:
   _genptr<_str> value;
   _genptr<_str> argument;
   FileContext* context;
};


struct C_RunWithWith : Command, RunBase
{
public:
   C_RunWithWith(_genptr<_str>& val, _genptr<_list>& arg, FileContext* ctx, p_perun2& p2)
      : context(ctx), RunBase(p2), value(std::move(val)), arguments(std::move(arg)) { };

   void run() override;

private:
   _genptr<_str> value;
   _genptr<_list> arguments;
   FileContext* context;
};


struct C_RunWithPerun2 : Command, RunBase
{
public:
   C_RunWithPerun2(FileContext* ctx, p_perun2& p2)
      : context(ctx), RunBase(p2) { };

   void run() override;

private:
   FileContext* context;
};


struct C_RunWithPerun2WithString : Command, RunBase
{
public:
   C_RunWithPerun2WithString(_genptr<_str>& arg, FileContext* ctx, p_perun2& p2)
      : context(ctx), RunBase(p2), argument(std::move(arg)) { };

   void run() override;

private:
   _genptr<_str> argument;
   FileContext* context;
};


struct C_RunWithPerun2With : Command, RunBase
{
public:
   C_RunWithPerun2With(_genptr<_list>& arg, FileContext* ctx, p_perun2& p2)
      : context(ctx), RunBase(p2), arguments(std::move(arg)) { };

   void run() override;

private:
   _genptr<_list> arguments;
   FileContext* context;
};

}

#endif /* COM_MISC_H */

