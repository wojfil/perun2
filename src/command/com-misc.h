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
   C_PrintSingle(_genptr<_str>& val, Uroboros& uro)
      : value(std::move(val)), uroboros(uro) { };

   void run() override;

private:
   Uroboros& uroboros;
   _genptr<_str> value;
};


struct C_PrintList : Command
{
public:
   C_PrintList(_genptr<_list>& val, Uroboros& uro)
      : value(std::move(val)), uroboros(uro) { };

   void run() override;

private:
   Uroboros& uroboros;
   _genptr<_list> value;
};


struct C_PrintDefinition : Command
{
public:
   C_PrintDefinition(_defptr& val, Uroboros& uro)
      : value(std::move(val)), uroboros(uro) { };

   void run() override;

private:
   Uroboros& uroboros;
   _defptr value;
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
   C_SleepPeriod(_genptr<_per>& val, Uroboros& uro)
      : value(std::move(val)), uroboros(uro) { };

   void run() override;

private:
   Uroboros& uroboros;
   _genptr<_per> value;
};


struct C_SleepMs : Command
{
public:
   C_SleepMs(_genptr<_num>& val, Uroboros& uro)
      : value(std::move(val)), uroboros(uro) { };

   void run() override;

private:
   Uroboros& uroboros;
   _genptr<_num> value;
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
   C_ErrorWithExitCode(_genptr<_num>& code, Uroboros& uro)
      : exitCode(std::move(code)), uroboros(uro) { };

   void run() override;

private:
   Uroboros& uroboros;
   _genptr<_num> exitCode;
};


struct C_Run : Command_L
{
public:
   C_Run(_genptr<_str>& val, Attribute* attr, Uroboros& uro)
      : value(std::move(val)), attribute(attr), hasAttribute(attr != nullptr), Command_L(uro) { };

   void run() override;

private:
   _genptr<_str> value;
   Attribute* attribute;
   const _bool hasAttribute;
};


struct C_RunWith : Command_L
{
public:
   C_RunWith(_genptr<_str>& val, Uroboros& uro)
      : value(std::move(val)), attribute(nullptr), hasAttribute(false), Command_L(uro) { };

   C_RunWith(_genptr<_str>& val, Attribute* attr, Uroboros& uro)
      : value(std::move(val)), attribute(attr), hasAttribute(attr != nullptr), Command_L(uro) { };

   void run() override;

private:
   _genptr<_str> value;
   Attribute* attribute;
   const _bool hasAttribute;
};


struct C_RunWithWithString : Command_L
{
public:
   C_RunWithWithString(_genptr<_str>& val, _genptr<_str>& arg, Uroboros& uro)
      : value(std::move(val)), argument(std::move(arg)), attribute(nullptr), hasAttribute(false), Command_L(uro) { };

   C_RunWithWithString(_genptr<_str>& val, _genptr<_str>& arg, Attribute* attr, Uroboros& uro)
      : value(std::move(val)), argument(std::move(arg)), attribute(attr), hasAttribute(attr != nullptr), Command_L(uro) { };

   void run() override;

private:
   _genptr<_str> value;
   _genptr<_str> argument;
   Attribute* attribute;
   const _bool hasAttribute;
};


struct C_RunWithWith : Command_L
{
public:
   C_RunWithWith(_genptr<_str>& val, _genptr<_list>& args, Uroboros& uro)
      : value(std::move(val)), arguments(std::move(args)), attribute(nullptr), hasAttribute(false), Command_L(uro) { };

   C_RunWithWith(_genptr<_str>& val, _genptr<_list>& args, Attribute* attr, Uroboros& uro)
      : value(std::move(val)), arguments(std::move(args)), attribute(attr), hasAttribute(attr != nullptr), Command_L(uro) { };

   void run() override;

private:
   _genptr<_str> value;
   _genptr<_list> arguments;
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
   C_RunWithUroborosWithString(_genptr<_str>& arg, Uroboros& uro)
      : argument(std::move(arg)), attribute(nullptr), hasAttribute(false), Command_L(uro) { };

   C_RunWithUroborosWithString(_genptr<_str>& arg, Attribute* attr, Uroboros& uro)
      : argument(std::move(arg)), attribute(attr), hasAttribute(attr != nullptr), Command_L(uro) { };

   void run() override;

private:
   _genptr<_str> argument;
   Attribute* attribute;
   const _bool hasAttribute;
};


struct C_RunWithUroborosWith : Command_L
{
public:
   C_RunWithUroborosWith(_genptr<_list>& args, Uroboros& uro)
      : arguments(std::move(args)), attribute(nullptr), hasAttribute(false), Command_L(uro) { };

   C_RunWithUroborosWith(_genptr<_list>& args, Attribute* attr, Uroboros& uro)
      : arguments(std::move(args)), attribute(attr), hasAttribute(attr != nullptr), Command_L(uro) { };

   void run() override;

private:
   _genptr<_list> arguments;
   Attribute* attribute;
   const _bool hasAttribute;
};

}

#endif /* COM_MISC_H */

