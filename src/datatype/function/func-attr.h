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

#include "func-generic.h"
#include "../../perun2.h"


namespace perun2::func
{
   

struct F_Parent : Func_1<p_str>, Generator<p_str>
{
public:
   F_Parent(p_genptr<p_str>& a1, Perun2Process& p2) 
      : Func_1(a1), context(p2.contexts.getLocationContext()) { };
   p_str getValue() override;

private:
   LocationContext* context;
};


struct F_Path_1 : Func_1<p_str>, Generator<p_str>
{
public:
   F_Path_1(p_genptr<p_str>& a1, Perun2Process& p2) 
      : Func_1(a1), context(p2.contexts.getLocationContext()) { };
   p_str getValue() override;

private:
   LocationContext* context;
};


struct F_Path_2 : Generator<p_str>
{
public:
   F_Path_2(p_genptr<p_str>& val_1, p_genptr<p_str>& val_2)
      : value_1(std::move(val_1)), value_2(std::move(val_2)) { };

   p_str getValue() override;

private:
   p_genptr<p_str> value_1;
   p_genptr<p_str> value_2;
};


struct F_Path_3 : Generator<p_str>
{
public:
   F_Path_3(p_genptr<p_str>& val_1, p_genptr<p_str>& val_2, p_genptr<p_str>& val_3)
      : value_1(std::move(val_1)), value_2(std::move(val_2)), value_3(std::move(val_3)) { };

   p_str getValue() override;

private:
   p_genptr<p_str> value_1;
   p_genptr<p_str> value_2;
   p_genptr<p_str> value_3;
};


struct F_Path_4 : Generator<p_str>
{
public:
   F_Path_4(p_genptr<p_str>& val_1, p_genptr<p_str>& val_2, p_genptr<p_str>& val_3, p_genptr<p_str>& val_4)
      : value_1(std::move(val_1)), value_2(std::move(val_2)),
        value_3(std::move(val_3)), value_4(std::move(val_4)) { };

   p_str getValue() override;

private:
   p_genptr<p_str> value_1;
   p_genptr<p_str> value_2;
   p_genptr<p_str> value_3;
   p_genptr<p_str> value_4;
};


struct F_Path_Multi : Generator<p_str>
{
public:
   F_Path_Multi(std::vector<p_genptr<p_str>>& vals)
      : length(vals.size())
   {
      langutil::transferUniquePtrs(vals, this->values);
   };

   p_str getValue() override;

private:
   std::vector<p_genptr<p_str>> values;
   const p_int length;
};


struct F_SizeDefinition : Generator<p_num>
{
public:
   F_SizeDefinition(p_defptr& def, Perun2Process& p2)
      : definition(std::move(def)), perun2(p2), 
        context(p2.contexts.getLocationContext()) { };

   p_num getValue() override;

private:
   Perun2Process& perun2;
   LocationContext* context;
   p_defptr definition;
};


struct F_SizeList : Generator<p_num>
{
public:
   F_SizeList(p_genptr<p_list>& vals, Perun2Process& p2)
      : values(std::move(vals)), perun2(p2), 
        context(p2.contexts.getLocationContext()) { };

   p_num getValue() override;

private:
   Perun2Process& perun2;
   LocationContext* context;
   p_genptr<p_list> values;
};


struct F_Attribute
{
public:
   F_Attribute(Perun2Process& p2)
      : context(*p2.contexts.getFileContext()), perun2(p2) { };

protected:
   void checkExistence();
   FileContext& context;
   Perun2Process& perun2;
};


struct F_Attr_Size : F_Attribute, Generator<p_num>
{
public:
   F_Attr_Size(Perun2Process& p2) : F_Attribute(p2) { };
   p_num getValue() override;
};


struct F_Attr_Lifetime : F_Attribute, Generator<p_per>
{
public:
   F_Attr_Lifetime(Perun2Process& p2) : F_Attribute(p2) { };
   p_per getValue() override;
};


struct F_Attr_Access: F_Attribute, Generator<p_tim>
{
public:
   F_Attr_Access(Perun2Process& p2) : F_Attribute(p2) { };
   p_tim getValue() override;
};


struct F_Attr_Change: F_Attribute, Generator<p_tim>
{
public:
   F_Attr_Change(Perun2Process& p2) : F_Attribute(p2) { };
   p_tim getValue() override;
};


struct F_Attr_Creation: F_Attribute, Generator<p_tim>
{
public:
   F_Attr_Creation(Perun2Process& p2) : F_Attribute(p2) { };
   p_tim getValue() override;
};


struct F_Attr_Modification: F_Attribute, Generator<p_tim>
{
public:
   F_Attr_Modification(Perun2Process& p2) : F_Attribute(p2) { };
   p_tim getValue() override;
};


struct F_Attr_Archive: F_Attribute, Generator<p_bool>
{
public:
   F_Attr_Archive(Perun2Process& p2) : F_Attribute(p2) { };
   p_bool getValue() override;
};


struct F_Attr_Compressed: F_Attribute, Generator<p_bool>
{
public:
   F_Attr_Compressed(Perun2Process& p2) : F_Attribute(p2) { };
   p_bool getValue() override;
};


struct F_Attr_Empty: F_Attribute, Generator<p_bool>
{
public:
   F_Attr_Empty(Perun2Process& p2) : F_Attribute(p2) { };
   p_bool getValue() override;
};


struct F_Attr_Exists: F_Attribute, Generator<p_bool>
{
public:
   F_Attr_Exists(Perun2Process& p2) : F_Attribute(p2) { };
   p_bool getValue() override;
};


struct F_Attr_Encrypted: F_Attribute, Generator<p_bool>
{
public:
   F_Attr_Encrypted(Perun2Process& p2) : F_Attribute(p2) { };
   p_bool getValue() override;
};


struct F_Attr_Hidden: F_Attribute, Generator<p_bool>
{
public:
   F_Attr_Hidden(Perun2Process& p2) : F_Attribute(p2) { };
   p_bool getValue() override;
};


struct F_Attr_IsDirectory: F_Attribute, Generator<p_bool>
{
public:
   F_Attr_IsDirectory(Perun2Process& p2) : F_Attribute(p2) { };
   p_bool getValue() override;
};


struct F_Attr_IsFile: F_Attribute, Generator<p_bool>
{
public:
   F_Attr_IsFile(Perun2Process& p2) : F_Attribute(p2) { };
   p_bool getValue() override;
};


struct F_Attr_Readonly: F_Attribute, Generator<p_bool>
{
public:
   F_Attr_Readonly(Perun2Process& p2) : F_Attribute(p2) { };
   p_bool getValue() override;
};


}
