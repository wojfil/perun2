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

#include "gen-list.h"
#include "../../perun2.h"
#include "../../util.h"


namespace perun2::gen
{

Join_StrStr::Join_StrStr(_genptr<_str>& lef, _genptr<_str>& rig)
   : left(std::move(lef)), right(std::move(rig)) { };

Join_StrList::Join_StrList(_genptr<_str>& lef, _genptr<_list>& rig)
   : left(std::move(lef)), right(std::move(rig)) { };

Join_ListStr::Join_ListStr(_genptr<_list>& lef, _genptr<_str>& rig)
   : left(std::move(lef)), right(std::move(rig)) { };

Join_ListList::Join_ListList(_genptr<_list>& lef, _genptr<_list>& rig)
   : left(std::move(lef)), right(std::move(rig)) { };

ListFilter_Where::ListFilter_Where(_genptr<_bool>& cond, _genptr<_list>& li, _fcptr& ctx, _p2& p2)
   : condition(std::move(cond)), list(std::move(li)), context(std::move(ctx)), perun2(p2) { };

ListFilter_Limit::ListFilter_Limit(_genptr<_list>& li, _genptr<_num>& num)
   : list(std::move(li)), number(std::move(num)) { };

ListFilter_Skip::ListFilter_Skip(_genptr<_list>& li, _genptr<_num>& num)
   : list(std::move(li)), number(std::move(num)) { };

ListFilter_Every::ListFilter_Every(_genptr<_list>& li, _genptr<_num>& num)
   : list(std::move(li)), number(std::move(num)) { };

ListFilter_Final::ListFilter_Final(_genptr<_list>& li, _genptr<_num>& num)
   : list(std::move(li)), number(std::move(num)) { };


_list Join_StrStr::getValue()
{
   return { left->getValue(), right->getValue() };
};


_list Join_StrList::getValue()
{
   _list v = right->getValue();
   v.insert(v.begin(), left->getValue());
   return v;
};


_list Join_ListStr::getValue()
{
   _list v = left->getValue();
   v.emplace_back(right->getValue());
   return v;
};


_list Join_ListList::getValue()
{
   _list v = left->getValue();
   langutil::appendVector(v, right->getValue());
   return v;
};


_list ListFilter_Where::getValue() 
{
   const _list values = list->getValue();
   _list result;
   result.reserve(values.size());
   const _nint length = static_cast<_nint>(values.size());

   this->context->resetIndex();
   _nint index = NINT_ZERO;

   while (this->perun2.state == State::s_Running && index != length) {
      const _str& unit = values[static_cast<_size>(index)];
      this->context->index->value.value.i = index;
      this->context->loadData(unit);

      if (condition->getValue()) {
         result.emplace_back(unit);
      }

      index++;
   }

   return result;
};


_list ListFilter_Limit::getValue() 
{
   const _nint n = number->getValue().toInt();

   if (n <= NINT_ZERO) {
      return _list();
   }

   const _list lst = list->getValue();

   return n >= static_cast<_nint>(lst.size())
      ? lst
      : _list(lst.begin(), lst.begin() + n);
};


_list ListFilter_Skip::getValue()
{
   const _nint n = number->getValue().toInt();
   const _list lst = list->getValue();

   if (n <= NINT_ZERO) {
      return lst;
   }

   return n >= static_cast<_nint>(lst.size())
      ? _list()
      : _list(lst.begin() + n, lst.end());
};


_list ListFilter_Every::getValue()
{
   const _nint n = number->getValue().toInt();
   const _list lst = list->getValue();

   if (n <= NINT_ONE) {
      return lst;
   }

   const _size baseSize = lst.size();
   const _size newSize = (baseSize / n) + ((baseSize % n == 0) ? 0 : 1);
   _list result(newSize);

   for (_size i = 0; i < newSize; i++) {
      result[i] = lst[i * n];
   }

   return result;
};


_list ListFilter_Final::getValue()
{
   const _nint n = number->getValue().toInt();

   if (n <= NINT_ZERO) {
      return _list();
   }

   const _list lst = list->getValue();

   return n >= static_cast<_nint>(lst.size())
      ? lst
      : _list(lst.end() - n, lst.end());
};


}
