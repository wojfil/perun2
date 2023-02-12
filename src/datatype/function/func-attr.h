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

#ifndef FUNC_ATTR_H_INCLUDED
#define FUNC_ATTR_H_INCLUDED

#include "func-generic.h"
#include "../../uroboros.h"


namespace uro::func
{


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
      langutil::transferUniquePtrs(vals, this->values);
   };

   _str getValue() override;

private:
   std::vector<_genptr<_str>> values;
   const _int length;
};


struct F_SizeDefinition : Generator<_num>
{
public:
   F_SizeDefinition(_defptr& def, _uro& uro)
      : definition(std::move(def)), uroboros(uro), 
        context(uro.contextes.getLocationContext()) { };

   _num getValue() override;

private:
   _uro& uroboros;
   LocationContext* context;
   _defptr definition;
};


struct F_SizeList : Generator<_num>
{
public:
   F_SizeList(_genptr<_list>& vals, _uro& uro)
      : values(std::move(vals)), uroboros(uro), 
        context(uro.contextes.getLocationContext()) { };

   _num getValue() override;

private:
   _uro& uroboros;
   LocationContext* context;
   _genptr<_list> values;
};


}

#endif // FUNC_ATTR_H_INCLUDED
