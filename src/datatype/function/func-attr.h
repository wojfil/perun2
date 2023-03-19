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

#ifndef FUNC_ATTR_H_INCLUDED
#define FUNC_ATTR_H_INCLUDED

#include "func-generic.h"
#include "../../perun2.h"


namespace perun2::func
{
   

struct F_Parent : Func_1<_str>, Generator<_str>
{
public:
   F_Parent(_genptr<_str>& a1, _p2& p2) 
      : Func_1(a1), context(p2.contexts.getLocationContext()) { };
   _str getValue() override;

private:
   LocationContext* context;
};


struct F_Path_1 : Func_1<_str>, Generator<_str>
{
public:
   F_Path_1(_genptr<_str>& a1, _p2& p2) 
      : Func_1(a1), context(p2.contexts.getLocationContext()) { };
   _str getValue() override;

private:
   LocationContext* context;
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
   F_SizeDefinition(_defptr& def, _p2& p2)
      : definition(std::move(def)), perun2(p2), 
        context(p2.contexts.getLocationContext()) { };

   _num getValue() override;

private:
   _p2& perun2;
   LocationContext* context;
   _defptr definition;
};


struct F_SizeList : Generator<_num>
{
public:
   F_SizeList(_genptr<_list>& vals, _p2& p2)
      : values(std::move(vals)), perun2(p2), 
        context(p2.contexts.getLocationContext()) { };

   _num getValue() override;

private:
   _p2& perun2;
   LocationContext* context;
   _genptr<_list> values;
};


struct F_Attribute
{
public:
   F_Attribute(_p2& p2)
      : context(*p2.contexts.getFileContext()), perun2(p2) { };

protected:
   void checkExistence();
   FileContext& context;
   _p2& perun2;
};


struct F_Attr_Size : F_Attribute, Generator<_num>
{
public:
   F_Attr_Size(_p2& p2) : F_Attribute(p2) { };
   _num getValue() override;
};


struct F_Attr_Lifetime : F_Attribute, Generator<_per>
{
public:
   F_Attr_Lifetime(_p2& p2) : F_Attribute(p2) { };
   _per getValue() override;
};


struct F_Attr_Access: F_Attribute, Generator<_tim>
{
public:
   F_Attr_Access(_p2& p2) : F_Attribute(p2) { };
   _tim getValue() override;
};


struct F_Attr_Change: F_Attribute, Generator<_tim>
{
public:
   F_Attr_Change(_p2& p2) : F_Attribute(p2) { };
   _tim getValue() override;
};


struct F_Attr_Creation: F_Attribute, Generator<_tim>
{
public:
   F_Attr_Creation(_p2& p2) : F_Attribute(p2) { };
   _tim getValue() override;
};


struct F_Attr_Modification: F_Attribute, Generator<_tim>
{
public:
   F_Attr_Modification(_p2& p2) : F_Attribute(p2) { };
   _tim getValue() override;
};


struct F_Attr_Archive: F_Attribute, Generator<_bool>
{
public:
   F_Attr_Archive(_p2& p2) : F_Attribute(p2) { };
   _bool getValue() override;
};


struct F_Attr_Compressed: F_Attribute, Generator<_bool>
{
public:
   F_Attr_Compressed(_p2& p2) : F_Attribute(p2) { };
   _bool getValue() override;
};


struct F_Attr_Empty: F_Attribute, Generator<_bool>
{
public:
   F_Attr_Empty(_p2& p2) : F_Attribute(p2) { };
   _bool getValue() override;
};


struct F_Attr_Exists: F_Attribute, Generator<_bool>
{
public:
   F_Attr_Exists(_p2& p2) : F_Attribute(p2) { };
   _bool getValue() override;
};


struct F_Attr_Encrypted: F_Attribute, Generator<_bool>
{
public:
   F_Attr_Encrypted(_p2& p2) : F_Attribute(p2) { };
   _bool getValue() override;
};


struct F_Attr_Hidden: F_Attribute, Generator<_bool>
{
public:
   F_Attr_Hidden(_p2& p2) : F_Attribute(p2) { };
   _bool getValue() override;
};


struct F_Attr_IsDirectory: F_Attribute, Generator<_bool>
{
public:
   F_Attr_IsDirectory(_p2& p2) : F_Attribute(p2) { };
   _bool getValue() override;
};


struct F_Attr_IsFile: F_Attribute, Generator<_bool>
{
public:
   F_Attr_IsFile(_p2& p2) : F_Attribute(p2) { };
   _bool getValue() override;
};


struct F_Attr_Readonly: F_Attribute, Generator<_bool>
{
public:
   F_Attr_Readonly(_p2& p2) : F_Attribute(p2) { };
   _bool getValue() override;
};


}

#endif // FUNC_ATTR_H_INCLUDED
