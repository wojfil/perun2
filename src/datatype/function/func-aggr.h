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
typedef std::vector<p_genptr<p_num>>     p_single;
typedef std::vector<p_genptr<p_nlist>>   p_multi;


struct FuncAggr : Generator<p_num>
{
public:
   FuncAggr(p_single& single, p_multi& multi)
      : countSingle(single.size()), countMulti(multi.size())
   {
      langutil::transferUniquePtrs(single, this->singleValues);
      langutil::transferUniquePtrs(multi, this->multiValues);
   };

protected:
   p_single singleValues;
   p_multi multiValues;
   const p_size countSingle;
   const p_size countMulti;
};


struct F_Average : FuncAggr
{
   F_Average(p_single& single, p_multi& multi) : FuncAggr(single, multi) { };
   p_num getValue() override;
};


struct F_Max : FuncAggr
{
   F_Max(p_single& single, p_multi& multi) : FuncAggr(single, multi) { };
   p_num getValue() override;
};


struct F_Median : FuncAggr
{
   F_Median(p_single& single, p_multi& multi) : FuncAggr(single, multi) { };
   p_num getValue() override;
};


struct F_Min : FuncAggr
{
   F_Min(p_single& single, p_multi& multi) : FuncAggr(single, multi) { };
   p_num getValue() override;
};


struct F_Sum : FuncAggr
{
   F_Sum(p_single& single, p_multi& multi) : FuncAggr(single, multi) { };
   p_num getValue() override;
};

}

#endif // FUNC_AGGR_H_INCLUDED
