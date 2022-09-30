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

#ifndef FUNC_ATTR_H_INCLUDED
#define FUNC_ATTR_H_INCLUDED

#include "func-generic.h"
#include "../../uroboros.h"


namespace uro::func
{

struct AttrFunction
{
public:
   AttrFunction(Generator<_str>* val, Uroboros* uro)
      : value(val), uroboros(uro), inner(&uro->vars.inner) { };

   ~AttrFunction();

protected:
   Uroboros* uroboros;
   InnerVariables* inner;
   _str getPath();
   Generator<_str>* value;
};


struct F_Path : AttrFunction, Generator<_str>
{
public:
   F_Path(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _str getValue() override;
};


struct F_Path_2 : Generator<_str>
{
public:
   F_Path_2(Generator<_str>* val_1, Generator<_str>* val_2)
      : value_1(val_1), value_2(val_2) { };
   ~F_Path_2();

   _str getValue() override;

private:
   Generator<_str>* value_1;
   Generator<_str>* value_2;
};


struct F_Path_3 : Generator<_str>
{
public:
   F_Path_3(Generator<_str>* val_1, Generator<_str>* val_2, Generator<_str>* val_3)
      : value_1(val_1), value_2(val_2), value_3(val_3) { };
   ~F_Path_3();

   _str getValue() override;

private:
   Generator<_str>* value_1;
   Generator<_str>* value_2;
   Generator<_str>* value_3;
};


struct F_Path_4 : Generator<_str>
{
public:
   F_Path_4(Generator<_str>* val_1, Generator<_str>* val_2,
      Generator<_str>* val_3, Generator<_str>* val_4)
      : value_1(val_1), value_2(val_2),
      value_3(val_3), value_4(val_4) { };

   ~F_Path_4();

   _str getValue() override;

private:
   Generator<_str>* value_1;
   Generator<_str>* value_2;
   Generator<_str>* value_3;
   Generator<_str>* value_4;
};


struct F_Path_Multi : Generator<_str>
{
public:
   F_Path_Multi(std::vector<Generator<_str>*>* vals)
      : values(vals), length(vals->size()) { };
   ~F_Path_Multi();

   _str getValue() override;

private:
   std::vector<Generator<_str>*>* values;
   const _int length;
};


struct F_Drive : AttrFunction, Generator<_str>
{
public:
   F_Drive(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _str getValue() override;
};


struct F_Extension : AttrFunction, Generator<_str>
{
public:
   F_Extension(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _str getValue() override;
};


struct F_Fullname : AttrFunction, Generator<_str>
{
public:
   F_Fullname(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _str getValue() override;
};


struct F_Name : AttrFunction, Generator<_str>
{
public:
   F_Name(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _str getValue() override;
};


struct F_Parent : AttrFunction, Generator<_str>
{
public:
   F_Parent(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _str getValue() override;
};


struct F_Size : AttrFunction, Generator<_num>
{
public:
   F_Size(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _num getValue() override;
};


struct F_SizeDefinition : Generator<_num>
{
public:
   F_SizeDefinition(_def* def, Uroboros* uro)
      : definition(def), uroboros(uro), inner(&uro->vars.inner) { };

   ~F_SizeDefinition() {
      delete definition;
   }

   _num getValue() override;

private:
   Uroboros* uroboros;
   InnerVariables* inner;
   _def* definition;
};


struct F_SizeList : Generator<_num>
{
public:
   F_SizeList(Generator<_list>* vals, Uroboros* uro)
      : values(vals), uroboros(uro), inner(&uro->vars.inner) { };

   ~F_SizeList() {
      delete values;
   }

   _num getValue() override;

private:
   Uroboros* uroboros;
   InnerVariables* inner;
   Generator<_list>* values;
};


struct F_Lifetime : AttrFunction, Generator<_per>
{
public:
   F_Lifetime(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _per getValue() override;
};


struct F_Access : AttrFunction, Generator<_tim>
{
public:
   F_Access(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _tim getValue() override;
};


struct F_Change : AttrFunction, Generator<_tim>
{
public:
   F_Change(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _tim getValue() override;
};


struct F_Creation : AttrFunction, Generator<_tim>
{
public:
   F_Creation(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _tim getValue() override;
};


struct F_Modification : AttrFunction, Generator<_tim>
{
public:
   F_Modification(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _tim getValue() override;
};


struct F_Archive : AttrFunction, Generator<_bool>
{
public:
   F_Archive(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_Compressed : AttrFunction, Generator<_bool>
{
public:
   F_Compressed(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_Empty : AttrFunction, Generator<_bool>
{
public:
   F_Empty(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_Encrypted : AttrFunction, Generator<_bool>
{
public:
   F_Encrypted(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_Exist : Generator<_bool>
{
public:
   F_Exist(Generator<_list>* val, Uroboros* uro)
      : value(val), uroboros(uro), inner(&uro->vars.inner) { };

   ~F_Exist();

   _bool getValue() override;

private:
   Uroboros* uroboros;
   InnerVariables* inner;
   Generator<_list>* value;
};


struct F_Exists : AttrFunction, Generator<_bool>
{
public:
   F_Exists(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_Hidden : AttrFunction, Generator<_bool>
{
public:
   F_Hidden(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_IsDirectory : AttrFunction, Generator<_bool>
{
public:
   F_IsDirectory(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_IsFile : AttrFunction, Generator<_bool>
{
public:
   F_IsFile(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_Readonly : AttrFunction, Generator<_bool>
{
public:
   F_Readonly(Generator<_str>* val, Uroboros* uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};

}

#endif // FUNC_ATTR_H_INCLUDED
