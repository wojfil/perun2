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

#ifndef FUNC_AGGR_H_INCLUDED
#define FUNC_AGGR_H_INCLUDED

#include "../datatype.h"
#include "../../util.h"


namespace perun2::func
{

// nice abbreviations:
typedef std::vector<_genptr<_num>>     _single;
typedef std::vector<_genptr<_nlist>>   _multi;


struct FuncAggr : Generator<_num>
{
public:
   FuncAggr(_single& single, _multi& multi)
      : countSingle(single.size()), countMulti(multi.size())
   {
      langutil::transferUniquePtrs(single, this->singleValues);
      langutil::transferUniquePtrs(multi, this->multiValues);
   };

protected:
   _single singleValues;
   _multi multiValues;
   const p_size countSingle;
   const p_size countMulti;
};


struct F_Average : FuncAggr
{
   F_Average(_single& single, _multi& multi) : FuncAggr(single, multi) { };
   _num getValue() override;
};


struct F_Max : FuncAggr
{
   F_Max(_single& single, _multi& multi) : FuncAggr(single, multi) { };
   _num getValue() override;
};


struct F_Median : FuncAggr
{
   F_Median(_single& single, _multi& multi) : FuncAggr(single, multi) { };
   _num getValue() override;
};


struct F_Min : FuncAggr
{
   F_Min(_single& single, _multi& multi) : FuncAggr(single, multi) { };
   _num getValue() override;
};


struct F_Sum : FuncAggr
{
   F_Sum(_single& single, _multi& multi) : FuncAggr(single, multi) { };
   _num getValue() override;
};

}

#endif // FUNC_AGGR_H_INCLUDED
