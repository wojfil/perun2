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

#ifndef FUNC_AGGR_H_INCLUDED
#define FUNC_AGGR_H_INCLUDED

#include "../generator.h"
#include "../datatype.h"

// nice abbreviations:
#define _single std::vector<Generator<_num>*>*
#define _multi std::vector<Generator<_nlist>*>*


struct FuncAggr : Generator<_num>
{
public:
   FuncAggr(_single single, _multi multi)
      : singleValues(single), multiValues(multi),
        countSingle(single->size()), countMulti(multi->size()) { };

   ~FuncAggr();

protected:
   _single singleValues;
   _multi multiValues;
   const _size countSingle;
   const _size countMulti;
};


struct F_Average : FuncAggr
{
   F_Average(_single single, _multi multi) : FuncAggr(single, multi) { };

   _num getValue() override;
};


struct F_Max : FuncAggr
{
   F_Max(_single single, _multi multi) : FuncAggr(single, multi) { };

   _num getValue() override;
};


struct F_Median : FuncAggr
{
   F_Median(_single single, _multi multi) : FuncAggr(single, multi) { };

   _num getValue() override;
};


struct F_Min : FuncAggr
{
   F_Min(_single single, _multi multi) : FuncAggr(single, multi) { };

   _num getValue() override;
};


struct F_Sum : FuncAggr
{
   F_Sum(_single single, _multi multi) : FuncAggr(single, multi) { };

   _num getValue() override;
};





#endif // FUNC_AGGR_H_INCLUDED

