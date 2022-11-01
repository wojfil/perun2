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
   AttrFunction(_genptr<_str>& val, Uroboros& uro)
      : value(std::move(val)), uroboros(uro), inner(uro.vars.inner) { };

protected:
   _str getPath();

   Uroboros& uroboros;
   InnerVariables& inner;
   _genptr<_str> value;
};


struct F_Path : AttrFunction, Generator<_str>
{
public:
   F_Path(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _str getValue() override;
};


struct F_Path_2 : Generator<_str>
{
public:
   F_Path_2(_genptr<_str>& val_1, _genptr<_str>& val_2)
      : value_1(std::move(val_1)), value_2(std::move(val_2)) { };

   _str getValue() override;

private:
   _genptr<_str> value_1;
   _genptr<_str> value_2;
};


struct F_Path_3 : Generator<_str>
{
public:
   F_Path_3(_genptr<_str>& val_1, _genptr<_str>& val_2, _genptr<_str>& val_3)
      : value_1(std::move(val_1)), value_2(std::move(val_2)), value_3(std::move(val_3)) { };

   _str getValue() override;

private:
   _genptr<_str> value_1;
   _genptr<_str> value_2;
   _genptr<_str> value_3;
};


struct F_Path_4 : Generator<_str>
{
public:
   F_Path_4(_genptr<_str>& val_1, _genptr<_str>& val_2, _genptr<_str>& val_3, _genptr<_str>& val_4)
      : value_1(std::move(val_1)), value_2(std::move(val_2)), 
        value_3(std::move(val_3)), value_4(std::move(val_4)) { };

   _str getValue() override;

private:
   _genptr<_str> value_1;
   _genptr<_str> value_2;
   _genptr<_str> value_3;
   _genptr<_str> value_4;
};


struct F_Path_Multi : Generator<_str>
{
public:
   F_Path_Multi(std::vector<_genptr<_str>>& vals)
      : length(vals.size()) 
   { 
      transferGenPtrs(vals, this->values);
   };

   _str getValue() override;

private:
   std::vector<_genptr<_str>> values;
   const _int length;
};


struct F_Drive : AttrFunction, Generator<_str>
{
public:
   F_Drive(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _str getValue() override;
};


struct F_Extension : AttrFunction, Generator<_str>
{
public:
   F_Extension(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _str getValue() override;
};


struct F_Fullname : AttrFunction, Generator<_str>
{
public:
   F_Fullname(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _str getValue() override;
};


struct F_Name : AttrFunction, Generator<_str>
{
public:
   F_Name(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _str getValue() override;
};


struct F_Parent : AttrFunction, Generator<_str>
{
public:
   F_Parent(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _str getValue() override;
};


struct F_Size : AttrFunction, Generator<_num>
{
public:
   F_Size(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _num getValue() override;
};


struct F_SizeDefinition : Generator<_num>
{
public:
   F_SizeDefinition(_defptr& def, Uroboros& uro)
      : definition(std::move(def)), uroboros(uro), inner(uro.vars.inner) { };

   _num getValue() override;

private:
   Uroboros& uroboros;
   InnerVariables& inner;
   _defptr definition;
};


struct F_SizeList : Generator<_num>
{
public:
   F_SizeList(_genptr<_list>& vals, Uroboros& uro)
      : values(std::move(vals)), uroboros(uro), inner(uro.vars.inner) { };

   _num getValue() override;

private:
   Uroboros& uroboros;
   InnerVariables& inner;
   _genptr<_list> values;
};


struct F_Lifetime : AttrFunction, Generator<_per>
{
public:
   F_Lifetime(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _per getValue() override;
};


struct F_Access : AttrFunction, Generator<_tim>
{
public:
   F_Access(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _tim getValue() override;
};


struct F_Change : AttrFunction, Generator<_tim>
{
public:
   F_Change(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _tim getValue() override;
};


struct F_Creation : AttrFunction, Generator<_tim>
{
public:
   F_Creation(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _tim getValue() override;
};


struct F_Modification : AttrFunction, Generator<_tim>
{
public:
   F_Modification(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _tim getValue() override;
};


struct F_Archive : AttrFunction, Generator<_bool>
{
public:
   F_Archive(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_Compressed : AttrFunction, Generator<_bool>
{
public:
   F_Compressed(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_Empty : AttrFunction, Generator<_bool>
{
public:
   F_Empty(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_Encrypted : AttrFunction, Generator<_bool>
{
public:
   F_Encrypted(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_Exist : Generator<_bool>
{
public:
   F_Exist(_genptr<_list>& val, Uroboros& uro)
      : value(std::move(val)), uroboros(uro), inner(uro.vars.inner) { };

   _bool getValue() override;

private:
   Uroboros& uroboros;
   InnerVariables& inner;
   _genptr<_list> value;
};


struct F_Exists : AttrFunction, Generator<_bool>
{
public:
   F_Exists(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_Hidden : AttrFunction, Generator<_bool>
{
public:
   F_Hidden(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_IsDirectory : AttrFunction, Generator<_bool>
{
public:
   F_IsDirectory(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_IsFile : AttrFunction, Generator<_bool>
{
public:
   F_IsFile(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};


struct F_Readonly : AttrFunction, Generator<_bool>
{
public:
   F_Readonly(_genptr<_str>& val, Uroboros& uro)
      : AttrFunction(val, uro) { };

   _bool getValue() override;
};

}

#endif // FUNC_ATTR_H_INCLUDED
