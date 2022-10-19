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

#ifndef COM_MISC_H
#define COM_MISC_H

#include "com-listener.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "com-aggregate.h"
#include "../attribute.h"
#include "../uroboros.h"
#include "../var/var.h"


namespace uro::comm
{

struct C_PrintSingle : Command
{
public:
   C_PrintSingle(Generator<_str>* val, Uroboros& uro)
      : value(val), uroboros(uro) { };

   ~C_PrintSingle() {
      delete value;
   };

   void run() override;

private:
   Uroboros& uroboros;
   Generator<_str>* value;
};


struct C_PrintList : Command
{
public:
   C_PrintList(Generator<_list>* val, Uroboros& uro)
      : value(val), uroboros(uro) { };

   ~C_PrintList() {
      delete value;
   };

   void run() override;

private:
   Uroboros& uroboros;
   Generator<_list>* value;
};


struct C_PrintDefinition : Command
{
public:
   C_PrintDefinition(_def* val, Uroboros& uro)
      : value(val), uroboros(uro) { };

   ~C_PrintDefinition() {
      delete value;
   };

   void run() override;

private:
   Uroboros& uroboros;
   _def* value;
};


struct C_PrintThis_Str : Command
{
public:
   C_PrintThis_Str(Uroboros& uro)
      : uroboros(uro), variable(uro.vars.inner.this_s) { };
   void run() override;

private:
   uro::vars::Variable<_str>& variable;
   Uroboros& uroboros;
};


struct C_PrintThis_Num : Command
{
public:
   C_PrintThis_Num(Uroboros& uro)
      : uroboros(uro), variable(uro.vars.inner.this_n) { };
   void run() override;

private:
   uro::vars::Variable<_num>& variable;
   Uroboros& uroboros;
};


struct C_PrintThis_Tim : Command
{
public:
   C_PrintThis_Tim(Uroboros& uro)
      : uroboros(uro), variable(uro.vars.inner.this_t) { };
   void run() override;

private:
   uro::vars::Variable<_tim>& variable;
   Uroboros& uroboros;
};


struct C_SleepPeriod : Command
{
public:
   C_SleepPeriod(Generator<_per>* val, Uroboros& uro)
      : value(val), uroboros(uro) { };

   ~C_SleepPeriod() {
      delete value;
   };

   void run() override;

private:
   Uroboros& uroboros;
   Generator<_per>* value;
};


struct C_SleepMs : Command
{
public:
   C_SleepMs(Generator<_num>* val, Uroboros& uro)
      : value(val), uroboros(uro) { };

   ~C_SleepMs() {
      delete value;
   };

   void run() override;

private:
   Uroboros& uroboros;
   Generator<_num>* value;
};


struct C_Break : Command
{
public:
   C_Break(Uroboros& uro)
      : uroboros(uro) { };
   void run() override;

private:
   Uroboros& uroboros;
};


struct C_Continue : Command
{
public:
   C_Continue(Uroboros& uro)
      : uroboros(uro) { };
   void run() override;

private:
   Uroboros& uroboros;
};


struct C_Exit : Command
{
public:
   C_Exit(Uroboros& uro)
      : uroboros(uro) { };
   void run() override;

private:
   Uroboros& uroboros;
};


struct C_Error : Command
{
public:
   C_Error(Uroboros& uro)
      : uroboros(uro) { };
   void run() override;

private:
   Uroboros& uroboros;
};


struct C_ErrorWithExitCode : Command
{
public:
   C_ErrorWithExitCode(Generator<_num>* code, Uroboros& uro)
      : exitCode(code), uroboros(uro) { };

   ~C_ErrorWithExitCode() {
      delete exitCode;
   }

   void run() override;

private:
   Uroboros& uroboros;
   Generator<_num>* exitCode;
};


struct C_Run : Command_L
{
public:
   C_Run(Generator<_str>* val, Attribute* attr, Uroboros& uro)
      : value(val), attribute(attr), hasAttribute(attr != nullptr), Command_L(uro) { };

   ~C_Run() {
      delete value;
   };

   void run() override;

private:
   Generator<_str>* value;
   Attribute* attribute;
   const _bool hasAttribute;
};


struct C_RunWith : Command_L
{
public:
   C_RunWith(Generator<_str>* val, Uroboros& uro)
      : value(val), attribute(nullptr), hasAttribute(false), Command_L(uro) { };

   C_RunWith(Generator<_str>* val, Attribute* attr, Uroboros& uro)
      : value(val), attribute(attr), hasAttribute(attr != nullptr), Command_L(uro) { };

   ~C_RunWith() {
      delete value;
   };

   void run() override;

private:
   Generator<_str>* value;
   Attribute* attribute;
   const _bool hasAttribute;
};


struct C_RunWithWithString : Command_L
{
public:
   C_RunWithWithString(Generator<_str>* val, Generator<_str>* arg, Uroboros& uro)
      : value(val), argument(arg), attribute(nullptr), hasAttribute(false), Command_L(uro) { };

   C_RunWithWithString(Generator<_str>* val, Generator<_str>* arg, Attribute* attr, Uroboros& uro)
      : value(val), argument(arg), attribute(attr), hasAttribute(attr != nullptr), Command_L(uro) { };

   ~C_RunWithWithString() {
      delete value;
      delete argument;
   };

   void run() override;

private:
   Generator<_str>* value;
   Generator<_str>* argument;
   Attribute* attribute;
   const _bool hasAttribute;
};


struct C_RunWithWith : Command_L
{
public:
   C_RunWithWith(Generator<_str>* val, Generator<_list>* args, Uroboros& uro)
      : value(val), arguments(args), attribute(nullptr), hasAttribute(false), Command_L(uro) { };

   C_RunWithWith(Generator<_str>* val, Generator<_list>* args, Attribute* attr, Uroboros& uro)
      : value(val), arguments(args), attribute(attr), hasAttribute(attr != nullptr), Command_L(uro) { };

   ~C_RunWithWith() {
      delete value;
      delete arguments;
   };

   void run() override;

private:
   Generator<_str>* value;
   Generator<_list>* arguments;
   Attribute* attribute;
   const _bool hasAttribute;
};


struct C_RunWithUroboros : Command_L
{
public:
   C_RunWithUroboros(Uroboros& uro)
      : attribute(nullptr), hasAttribute(false), Command_L(uro) { };
   C_RunWithUroboros(Attribute* attr, Uroboros& uro)
      : attribute(attr), hasAttribute(attr != nullptr), Command_L(uro) { };

   void run() override;

private:
   Attribute* attribute;
   const _bool hasAttribute;
};


struct C_RunWithUroborosWithString : Command_L
{
public:
   C_RunWithUroborosWithString(Generator<_str>* arg, Uroboros& uro)
      : argument(arg), attribute(nullptr), hasAttribute(false), Command_L(uro) { };

   C_RunWithUroborosWithString(Generator<_str>* arg, Attribute* attr, Uroboros& uro)
      : argument(arg), attribute(attr), hasAttribute(attr != nullptr), Command_L(uro) { };

   ~C_RunWithUroborosWithString() {
      delete argument;
   };

   void run() override;

private:
   Generator<_str>* argument;
   Attribute* attribute;
   const _bool hasAttribute;
};


struct C_RunWithUroborosWith : Command_L
{
public:
   C_RunWithUroborosWith(Generator<_list>* args, Uroboros& uro)
      : arguments(args), attribute(nullptr), hasAttribute(false), Command_L(uro) { };

   C_RunWithUroborosWith(Generator<_list>* args, Attribute* attr, Uroboros& uro)
      : arguments(args), attribute(attr), hasAttribute(attr != nullptr), Command_L(uro) { };

   ~C_RunWithUroborosWith() {
      delete arguments;
   };

   void run() override;

private:
   Generator<_list>* arguments;
   Attribute* attribute;
   const _bool hasAttribute;
};

}

#endif /* COM_MISC_H */

