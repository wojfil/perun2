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

#ifndef VAR_RUNTIME_H
#define VAR_RUNTIME_H

#include <map>
#include "var.h"
#include "var-bundle.h"
#include "var-inner.h"
#include "var-context.h"
#include "../datatype/def-var.h"
#include "../datatype/generator/gen-os.h"
#include "../hash.h"
#include "../token.h"


struct Uroboros;

struct Variables
{
public:

   Variables(Uroboros* uro);

   const _str uroPath;
   InnerVariables inner;

   void varsLevelUp();
   void varsLevelDown();

   // these overloaded methods fit easily into template functions:
   void takeBundlePointer(VarBundle<_boo>*& bundle);
   void takeBundlePointer(VarBundle<_per>*& bundle);
   void takeBundlePointer(VarBundle<_tim>*& bundle);
   void takeBundlePointer(VarBundle<_num>*& bundle);
   void takeBundlePointer(VarBundle<_str>*& bundle);
   void takeBundlePointer(VarBundle<_nlist>*& bundle);
   void takeBundlePointer(VarBundle<_tlist>*& bundle);
   void takeBundlePointer(VarBundle<_list>*& bundle);

   // get a reference to variable or 'this'
   _boo getVarValue(const Token& tk, Generator<_tim>*& result);
   _boo getVarValue(const Token& tk, Generator<_str>*& result);
   _boo getVarValue(const Token& tk, Generator<_num>*& result);

   // generate new definition
   _boo getVarValue(const Token& tk, _def*& result);

   // usual variable value reference
   template <typename T>
   _boo getVarValue(const Token& tk, Generator<T>*& result)
   {
      VarBundle<T>* bundle;
      takeBundlePointer(bundle);
      return bundle->getValue(this->vc, tk, result, this->inner);
   }

   _boo variableExists(const Token& tk);
   void makeAllNotConstant();

   // variable reference for commands, that alter variables
   template <typename T>
   _boo getVarPtr(const Token& tk, ParseVariable<T>*& result)
   {
      VarBundle<T>* bundle;
      takeBundlePointer(bundle);
      return bundle->getVarPtr(tk, result);
   }

private:

   Uroboros* uroboros;
   Hashes* hashes;
   VariablesContext* vc;

   VarBundle<_boo> boo;
   VarBundle<_per> per;
   VarBundle<_tim> tim;
   VarBundle<_num> num;
   VarBundle<_str> str_;
   VarBundle<_nlist> nlist;
   VarBundle<_tlist> tlist;
   VarBundle<_list> list;

   std::map<_size, DefinitionGenerator*> defGenerators;
   std::map<_size, Variable<_numi>*> intVars;

};

#endif /* VAR_RUNTIME_H */
