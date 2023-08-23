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

#ifndef GEN_BOOL_CONSTR_H
#define GEN_BOOL_CONSTR_H

#include "../incr-constr.h"


namespace perun2
{
   struct FileContext;
   struct p_perun2;
}

namespace perun2::gen
{

struct SizeConstraint : Generator<p_bool>
{
public:
   SizeConstraint(p_conptr& constr, FileContext& ctx, p_perun2& p2);
   p_bool getValue() override;

private:
   p_conptr constraint;
   FileContext& context;
   p_perun2& perun2;
};


}

#endif /* GEN_BOOL_CONSTR_H */
