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

#include "../../../include/perun2/datatype/generator/gen-list.hpp"
#include "../../../include/perun2/perun2.hpp"
#include "../../../include/perun2/util.hpp"


namespace perun2::gen
{

Join_StrStr::Join_StrStr(p_genptr<p_str>& lef, p_genptr<p_str>& rig)
   : left(std::move(lef)), right(std::move(rig)) { };

Join_StrList::Join_StrList(p_genptr<p_str>& lef, p_genptr<p_list>& rig)
   : left(std::move(lef)), right(std::move(rig)) { };

Join_ListStr::Join_ListStr(p_genptr<p_list>& lef, p_genptr<p_str>& rig)
   : left(std::move(lef)), right(std::move(rig)) { };

Join_ListList::Join_ListList(p_genptr<p_list>& lef, p_genptr<p_list>& rig)
   : left(std::move(lef)), right(std::move(rig)) { };

ListFilter_Where::ListFilter_Where(p_genptr<p_bool>& cond, p_genptr<p_list>& li, p_fcptr& ctx, Perun2Process& p2)
   : condition(std::move(cond)), list(std::move(li)), context(std::move(ctx)), perun2(p2) { };

ListFilter_Limit::ListFilter_Limit(p_genptr<p_list>& li, p_genptr<p_num>& num)
   : list(std::move(li)), number(std::move(num)) { };

ListFilter_Skip::ListFilter_Skip(p_genptr<p_list>& li, p_genptr<p_num>& num)
   : list(std::move(li)), number(std::move(num)) { };

ListFilter_Every::ListFilter_Every(p_genptr<p_list>& li, p_genptr<p_num>& num)
   : list(std::move(li)), number(std::move(num)) { };

ListFilter_Final::ListFilter_Final(p_genptr<p_list>& li, p_genptr<p_num>& num)
   : list(std::move(li)), number(std::move(num)) { };


p_list Join_StrStr::getValue()
{
   return { left->getValue(), right->getValue() };
};


p_list Join_StrList::getValue()
{
   p_list v = right->getValue();
   v.insert(v.begin(), left->getValue());
   return v;
};


p_list Join_ListStr::getValue()
{
   p_list v = left->getValue();
   v.emplace_back(right->getValue());
   return v;
};


p_list Join_ListList::getValue()
{
   p_list v = left->getValue();
   langutil::appendVector(v, right->getValue());
   return v;
};


p_list ListFilter_Where::getValue() 
{
   const p_list values = list->getValue();
   p_list result;
   result.reserve(values.size());
   const p_nint length = static_cast<p_nint>(values.size());

   this->context->resetIndex();
   p_nint index = NINT_ZERO;

   while (this->perun2.isRunning() && index != length) {
      const p_str& unit = values[static_cast<p_size>(index)];
      this->context->index->value.value.i = index;
      this->context->loadData(unit);

      if (condition->getValue()) {
         result.emplace_back(unit);
      }

      index++;
   }

   return result;
};


p_list ListFilter_Limit::getValue() 
{
   const p_num n = number->getValue();
   if (n.isNaN()) {
      return p_list();
   }

   const p_nint limit = n.toInt();

   if (limit <= NINT_ZERO) {
      return p_list();
   }

   const p_list lst = list->getValue();

   return limit >= static_cast<p_nint>(lst.size())
      ? lst
      : p_list(lst.begin(), lst.begin() + limit);
};


p_list ListFilter_Skip::getValue()
{
   const p_num n = number->getValue();
   if (n.isNaN()) {
      return p_list();
   }

   const p_nint skip = n.toInt();
   const p_list lst = list->getValue();

   if (skip <= NINT_ZERO) {
      return lst;
   }

   return skip >= static_cast<p_nint>(lst.size())
      ? p_list()
      : p_list(lst.begin() + skip, lst.end());
};


p_list ListFilter_Every::getValue()
{
   const p_num n = number->getValue();
   if (n.isNaN()) {
      return p_list();
   }

   const p_nint every = n.toInt();
   const p_list lst = list->getValue();

   if (every <= NINT_ONE) {
      return lst;
   }

   const p_size baseSize = lst.size();
   const p_size newSize = (baseSize / every) + ((baseSize % every == 0) ? 0 : 1);
   p_list result(newSize);

   for (p_size i = 0; i < newSize; i++) {
      result[i] = lst[i * every];
   }

   return result;
};


p_list ListFilter_Final::getValue()
{
   const p_num n = number->getValue();
   if (n.isNaN()) {
      return p_list();
   }

   const p_nint fin = n.toInt();

   if (fin <= NINT_ZERO) {
      return p_list();
   }

   const p_list lst = list->getValue();

   return fin >= static_cast<p_nint>(lst.size())
      ? lst
      : p_list(lst.end() - fin, lst.end());
};


}
