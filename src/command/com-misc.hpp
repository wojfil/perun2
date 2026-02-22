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

#include "com-listener.hpp"
#include "../datatype/datatype.hpp"
#include "../datatype/generator.hpp"
#include "com-aggregate.hpp"
#include "com-core.hpp"
#include "../attribute.hpp"
#include "../perun2.hpp"


namespace perun2::comm
{

struct C_PrintSingle : Command
{
public:
   C_PrintSingle(p_genptr<p_str>& val, Perun2Process& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   Perun2Process& perun2;
   p_genptr<p_str> value;
};


struct C_PrintList : Command
{
public:
   C_PrintList(p_genptr<p_list>& val, Perun2Process& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   Perun2Process& perun2;
   p_genptr<p_list> value;
};


struct C_PrintDefinition : Command
{
public:
   C_PrintDefinition(p_defptr& val, Perun2Process& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   Perun2Process& perun2;
   p_defptr value;
};


struct C_PrintThis : Command
{
public:
   C_PrintThis(Perun2Process& p2, FileContext& ctx)
      : perun2(p2), context(ctx) { };
   void run() override;

private:
   FileContext& context;
   Perun2Process& perun2;
};


struct C_SleepPeriod : Command
{
public:
   C_SleepPeriod(p_genptr<p_per>& val, Perun2Process& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   Perun2Process& perun2;
   p_genptr<p_per> value;
};


struct C_SleepMs : Command
{
public:
   C_SleepMs(p_genptr<p_num>& val, Perun2Process& p2)
      : value(std::move(val)), perun2(p2) { };

   void run() override;

private:
   Perun2Process& perun2;
   p_genptr<p_num> value;
};


struct C_Break : Command
{
public:
   C_Break(Perun2Process& p2)
      : perun2(p2) { };
   void run() override;

private:
   Perun2Process& perun2;
};


struct C_Continue : Command
{
public:
   C_Continue(Perun2Process& p2)
      : perun2(p2) { };
   void run() override;

private:
   Perun2Process& perun2;
};


struct C_Exit : Command
{
public:
   C_Exit(Perun2Process& p2)
      : perun2(p2) { };
   void run() override;

private:
   Perun2Process& perun2;
};


struct C_Error : Command
{
public:
   C_Error(Perun2Process& p2)
      : perun2(p2) { };
   void run() override;

private:
   Perun2Process& perun2;
};


struct C_ErrorWithExitCode : Command
{
public:
   C_ErrorWithExitCode(p_genptr<p_num>& code, Perun2Process& p2)
      : exitCode(std::move(code)), perun2(p2) { };

   void run() override;

private:
   Perun2Process& perun2;
   p_genptr<p_num> exitCode;
};


struct RunBase
{
public:
   RunBase() = delete;
   RunBase(Perun2Process& p2);

protected:
   p_str getLocation();
   
   Perun2Process& perun2;

private:
   LocationContext* locationCtx;
};


struct C_Run : Command, RunBase
{
public:
   C_Run(p_genptr<p_str>& val, Perun2Process& p2)
      : RunBase(p2), value(std::move(val)) { };

   void run() override;

private:
   p_genptr<p_str> value;
};


struct C_RunWith : Command, RunBase
{
public:
   C_RunWith(p_genptr<p_str>& val, FileContext* ctx, Perun2Process& p2)
      : context(ctx), RunBase(p2), value(std::move(val)) { };

   void run() override;

private:
   p_genptr<p_str> value;
   FileContext* context;
};


struct C_RunWithWithString : Command, RunBase
{
public:
   C_RunWithWithString(p_genptr<p_str>& val, p_genptr<p_str>& arg, FileContext* ctx, Perun2Process& p2)
      : context(ctx), RunBase(p2), value(std::move(val)), argument(std::move(arg)) { };

   void run() override;

private:
   p_genptr<p_str> value;
   p_genptr<p_str> argument;
   FileContext* context;
};


struct C_RunWithWith : Command, RunBase
{
public:
   C_RunWithWith(p_genptr<p_str>& val, p_genptr<p_list>& arg, FileContext* ctx, Perun2Process& p2)
      : context(ctx), RunBase(p2), value(std::move(val)), arguments(std::move(arg)) { };

   void run() override;

private:
   p_genptr<p_str> value;
   p_genptr<p_list> arguments;
   FileContext* context;
};


struct C_RunWithPerun2 : Command, RunBase
{
public:
   C_RunWithPerun2(FileContext* ctx, Perun2Process& p2)
      : context(ctx), RunBase(p2) { };

   void run() override;

private:
   FileContext* context;
};


struct C_RunWithPerun2WithString : Command, RunBase
{
public:
   C_RunWithPerun2WithString(p_genptr<p_str>& arg, FileContext* ctx, Perun2Process& p2)
      : context(ctx), RunBase(p2), argument(std::move(arg)) { };

   void run() override;

private:
   p_genptr<p_str> argument;
   FileContext* context;
};


struct C_RunWithPerun2With : Command, RunBase
{
public:
   C_RunWithPerun2With(p_genptr<p_list>& arg, FileContext* ctx, Perun2Process& p2)
      : context(ctx), RunBase(p2), arguments(std::move(arg)) { };

   void run() override;

private:
   p_genptr<p_list> arguments;
   FileContext* context;
};


struct C_Popup : Command
{
public:
   C_Popup(p_genptr<p_str>& val)
      : value(std::move(val)) { };

   void run() override;

private:
   p_genptr<p_str> value;
};


}
