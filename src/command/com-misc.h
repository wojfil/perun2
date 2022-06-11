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

#include "com.h"
#include "../datatype/datatype.h"
#include "../datatype/generator.h"
#include "aggregate.h"
#include "../attribute.h"
#include "../var/var.h"


struct C_PrintSingle : Command
{
public:
   C_PrintSingle(Generator<_str>* val) : value(val) { };

   ~C_PrintSingle() {
      delete value;
   };

   void run() override;

private:
   Generator<_str>* value;
};


struct C_PrintList : Command
{
public:
   C_PrintList(Generator<_list>* val) : value(val) { };

   ~C_PrintList() {
      delete value;
   };

   void run() override;

private:
   Generator<_list>* value;
};


struct C_PrintDefinition : Command
{
public:
   C_PrintDefinition(_def* val) : value(val) { };

   ~C_PrintDefinition() {
      delete value;
   };

   void run() override;

private:
   _def* value;
};


struct C_PrintThis_Str : Command
{
public:
   C_PrintThis_Str() { };
   void run() override;
};


struct C_PrintThis_Num : Command
{
public:
   C_PrintThis_Num() { };
   void run() override;
};



struct C_PrintThis_Tim : Command
{
public:
   C_PrintThis_Tim() { };
   void run() override;
};


struct C_SleepPeriod : Command
{
public:
   C_SleepPeriod(Generator<_per>* val) : value(val) { };

   ~C_SleepPeriod() {
      delete value;
   };

   void run() override;

private:
   Generator<_per>* value;
};


struct C_SleepMs : Command
{
public:
   C_SleepMs(Generator<_num>* val) : value(val) { };

   ~C_SleepMs() {
      delete value;
   };

   void run() override;

private:
   Generator<_num>* value;
};


struct C_Break : Command
{
public:
   C_Break() { };
   void run() override;
};


struct C_Continue : Command
{
public:
   C_Continue() { };
   void run() override;
};


struct C_Exit : Command
{
public:
   C_Exit() { };
   void run() override;
};


struct C_Error : Command
{
public:
   C_Error() { };
   void run() override;
};


struct C_ErrorWithExitCode : Command
{
public:
   C_ErrorWithExitCode(Generator<_num>* code)
      : exitCode(code) { };

   ~C_ErrorWithExitCode() {
      delete exitCode;
   }

   void run() override;

private:
   Generator<_num>* exitCode;
};


struct C_Run : Command
{
public:
   C_Run(Generator<_str>* val, Attribute* attr) : value(val),
      attribute(attr), hasAttribute(attr != nullptr) { };

   ~C_Run() {
      delete value;
   };

   void run() override;

private:
   Generator<_str>* value;
   Attribute* attribute;
   const _boo hasAttribute;
};


struct C_RunWith : Command
{
public:
   C_RunWith(Generator<_str>* val) : value(val),
      attribute(nullptr), hasAttribute(false) { };

   C_RunWith(Generator<_str>* val, Attribute* attr) : value(val),
      attribute(attr), hasAttribute(attr != nullptr) { };

   ~C_RunWith() {
      delete value;
   };

   void run() override;

private:
   Generator<_str>* value;
   Attribute* attribute;
   const _boo hasAttribute;
};


struct C_RunWithWithString : Command
{
public:
   C_RunWithWithString(Generator<_str>* val, Generator<_str>* arg)
      : value(val), argument(arg), attribute(nullptr), hasAttribute(false) { };

   C_RunWithWithString(Generator<_str>* val, Generator<_str>* arg, Attribute* attr)
      : value(val), argument(arg), attribute(attr), hasAttribute(attr != nullptr) { };

   ~C_RunWithWithString() {
      delete value;
      delete argument;
   };

   void run() override;

private:
   Generator<_str>* value;
   Generator<_str>* argument;
   Attribute* attribute;
   const _boo hasAttribute;
};


struct C_RunWithWith : Command
{
public:
   C_RunWithWith(Generator<_str>* val, Generator<_list>* args)
      : value(val), arguments(args), attribute(nullptr), hasAttribute(false) { };

   C_RunWithWith(Generator<_str>* val, Generator<_list>* args, Attribute* attr)
      : value(val), arguments(args), attribute(attr), hasAttribute(attr != nullptr) { };

   ~C_RunWithWith() {
      delete value;
      delete arguments;
   };

   void run() override;

private:
   Generator<_str>* value;
   Generator<_list>* arguments;
   Attribute* attribute;
   const _boo hasAttribute;
};


struct C_RunWithUroboros : Command
{
public:
   C_RunWithUroboros() : attribute(nullptr), hasAttribute(false) { };
   C_RunWithUroboros(Attribute* attr) : attribute(attr), hasAttribute(attr != nullptr) { };

   void run() override;

private:
   Attribute* attribute;
   const _boo hasAttribute;
};


struct C_RunWithUroborosWithString : Command
{
public:
   C_RunWithUroborosWithString(Generator<_str>* arg)
      : argument(arg), attribute(nullptr), hasAttribute(false) { };

   C_RunWithUroborosWithString(Generator<_str>* arg, Attribute* attr)
      : argument(arg), attribute(attr), hasAttribute(attr != nullptr) { };

   ~C_RunWithUroborosWithString() {
      delete argument;
   };

   void run() override;

private:
   Generator<_str>* argument;
   Attribute* attribute;
   const _boo hasAttribute;
};


struct C_RunWithUroborosWith : Command
{
public:
   C_RunWithUroborosWith(Generator<_list>* args)
      : arguments(args), attribute(nullptr), hasAttribute(false) { };

   C_RunWithUroborosWith(Generator<_list>* args, Attribute* attr)
      : arguments(args), attribute(attr), hasAttribute(attr != nullptr) { };

   ~C_RunWithUroborosWith() {
      delete arguments;
   };

   void run() override;

private:
   Generator<_list>* arguments;
   Attribute* attribute;
   const _boo hasAttribute;
};


struct C_Process : Command
{
public:
   C_Process(Generator<_str>* val) : value(val) { };

   ~C_Process() {
      delete value;
   };

   void run() override;

private:
   Generator<_str>* value;
};



#endif /* COM_MISC_H */

