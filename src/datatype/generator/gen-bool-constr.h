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
#include "../../perun2.h"


namespace perun2::gen
{


struct ContextConstraint : Generator<p_bool>
{
public:
   ContextConstraint(p_genptr<p_num>& limit, const CompType cmptype, FileContext& ctx, p_perun2& p2);

protected:
   IncrementalConstraint constraint;
   FileContext& context;
   p_perun2& perun2;
};


struct SizeConstraint : ContextConstraint
{
public:
   SizeConstraint(p_genptr<p_num>& limit, const CompType cmptype, FileContext& ctx, p_perun2& p2);
   p_bool getValue() override;
};


struct CountConstraint : Generator<p_bool>
{
public:
   CountConstraint(p_genptr<p_num>& limit, const CompType cmptype, p_defptr& def, p_perun2& p2);
   p_bool getValue() override;

private:
   IncrementalConstraint constraint;
   p_defptr definition;
   p_perun2& perun2;
};


struct CountInsideConstraint : ContextConstraint
{
public:
   CountInsideConstraint(p_genptr<p_num>& limit, const CompType cmptype, p_defptr& def, 
      p_lcptr& lctx, FileContext& ctx, p_perun2& p2);
   p_bool getValue() override;

private:
   p_lcptr locContext;
   p_defptr definition;
};


struct SizeConstraint_Def : Generator<p_bool>
{
public:
   SizeConstraint_Def(p_genptr<p_num>& limit, p_defptr& def, const CompType cmptype, p_perun2& p2);
   p_bool getValue() override;

private:
   p_defptr definition;
   IncrementalConstraint constraint;
   LocationContext& context;
   p_perun2& perun2;
};


struct SizeConstraint_List : Generator<p_bool>
{
public:
   SizeConstraint_List(p_genptr<p_num>& limit, p_genptr<p_list>& lst, const CompType cmptype, p_perun2& p2);
   p_bool getValue() override;

private:
   p_genptr<p_list> list;
   IncrementalConstraint constraint;
   LocationContext& context;
   p_perun2& perun2;
};


}

#endif /* GEN_BOOL_CONSTR_H */
