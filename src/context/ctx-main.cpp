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

#include "ctx-main.h"
#include "../perun2.h"


namespace perun2
{

   Contexts::Contexts(Perun2Process& p2)
      : GlobalContext(p2), success(std::make_unique<Variable<p_bool>>(VarType::vt_Special, false))
   {
      this->locationContexts.push_back(&this->rootLocation);
      this->locationContexts.back()->location->value = p2.arguments.getLocation();

      this->addOsGen(STRING_FILES, gen::OsElement::oe_Files, p2);
      this->addOsGen(STRING_RECURSIVEFILES, gen::OsElement::oe_RecursiveFiles, p2);
      this->addOsGen(STRING_DIRECTORIES, gen::OsElement::oe_Directories, p2);
      this->addOsGen(STRING_RECURSIVEDIRECTORIES, gen::OsElement::oe_RecursiveDirectories, p2);
      this->addOsGen(STRING_IMAGES, gen::OsElement::oe_Images, p2);
      this->addOsGen(STRING_RECURSIVEIMAGES, gen::OsElement::oe_RecursiveImages, p2);
      this->addOsGen(STRING_VIDEOS, gen::OsElement::oe_Videos, p2);
      this->addOsGen(STRING_RECURSIVEVIDEOS, gen::OsElement::oe_RecursiveVideos, p2);
   };

   p_bool Contexts::getVar(const Token& tk, Variable<p_bool>*& result, Perun2Process& p2)
   {
      if (tk.isWord(STRING_SUCCESS, p2)) {
         result = this->success.get();
         return true;
      }

      return findVar(tk, result, p2);
   }

   p_bool Contexts::getVar(const Token& tk, Variable<p_num>*& result, Perun2Process& p2)
   {
      if (tk.isWord(STRING_INDEX, p2)) {
         if (this->indexContexts.empty()) {
            throw SyntaxError::undefinedVarValue(tk.getOriginString(p2), tk.line);
            return false;
         }

         IndexContext* last = this->indexContexts.back();
         if (last != nullptr) {
            result = last->index.get();
            return true;
         }
      }

      return findVar(tk, result, p2);
   }

   p_bool Contexts::getVar(const Token& tk, Variable<p_str>*& result, Perun2Process& p2)
   {
      if (tk.isWord(STRING_THIS, p2)) {
         if (this->fileContexts.empty()) {
            throw SyntaxError::undefinedVarValue(tk.getOriginString(p2), tk.line);
            return false;
         }

         FileContext* last = this->fileContexts.back();
         if (last != nullptr) {
            result = last->this_.get();
            return true;
         }
      }
      else if (tk.isWord(STRING_LOCATION, p2)) {
         if (this->locationContexts.empty()) {
            throw SyntaxError::undefinedVarValue(tk.getOriginString(p2), tk.line);
            return false;
         }

         LocationContext* last = this->locationContexts.back();
         if (last != nullptr) {
            result = last->location.get();
            return true;
         }
      }

      return findVar(tk, result, p2);
   }

   void Contexts::addUserVarsContext(UserVarsContext* ctx)
   {
      this->userVarsContexts.push_back(ctx);
   }

   void Contexts::retreatUserVarsContext()
   {
      this->userVarsContexts.pop_back();
   }

   void Contexts::addAggregateContext(AggregateContext* ctx)
   {
      this->aggregateContexts.push_back(ctx);
   }

   void Contexts::retreatAggregateContext()
   {
      this->aggregateContexts.pop_back();
   }

   void Contexts::addIndexContext(IndexContext* ctx)
   {
      this->aggregateContexts.push_back(ctx);
      this->indexContexts.push_back(ctx);
   }

   void Contexts::retreatIndexContext()
   {
      this->aggregateContexts.pop_back();
      this->indexContexts.pop_back();
   }

   void Contexts::addFileContext(FileContext* ctx)
   {
      this->aggregateContexts.push_back(ctx);
      this->indexContexts.push_back(ctx);
      this->fileContexts.push_back(ctx);
   }

   void Contexts::retreatFileContext()
   {
      this->aggregateContexts.pop_back();
      this->indexContexts.pop_back();
      this->fileContexts.pop_back();
   }

   void Contexts::addLocationContext(LocationContext* ctx)
   {
      this->locationContexts.push_back(ctx);
   }

   void Contexts::retreatLocationContext()
   {
      this->locationContexts.pop_back();
   }

   p_bool Contexts::hasAggregate()
   {
      return this->aggregateContexts.size() >= 1;
   }

   comm::Aggregate* Contexts::getAggregate()
   {
      return &this->aggregateContexts[0]->aggregate;
   }

   LocationContext* Contexts::getLocationContext()
   {
      return this->locationContexts.back();
   }

   AggregateContext* Contexts::getAggregateContext()
   {
      return this->aggregateContexts.back();
   }

   p_bool Contexts::hasIterationContext() const
   {
      return !this->fileContexts.empty();
   }

   p_bool Contexts::hasFileContext() const
   {
      if (this->fileContexts.empty()) {
         return false;
      }

      return this->fileContexts.back() != nullptr;
   }

   FileContext* Contexts::getFileContext()
   {
      return this->fileContexts.back();
   }

   p_bool Contexts::hasIndexContext() const
   {
      return !this->indexContexts.empty();
   }

   void Contexts::addOsGen(const p_str& name, const gen::OsElement element, Perun2Process& p2)
   {
      osGenerators.insert(std::make_pair(name, gen::DefinitionGenerator(element, p2)));
   }

   void Contexts::makeLocationContext(p_lcptr& result)
   {
      result = std::make_unique<LocationContext>(this->locationContexts.back());
   }

   UserVarsContext* Contexts::getUserVarsContext()
   {
      return this->userVarsContexts.back();
   }

   p_bool Contexts::varExists(const Token& tk, Perun2Process& p2)
   {
      if (p2.constCache.programs.haveName(tk)) {
         return true;
      }

      const p_str word = tk.toLowerString(p2);
      Variable<p_bool>* b;
      Variable<p_tim>* t;
      Variable<p_per>* p;
      Variable<p_str>* s;
      Variable<p_num>* n;
      Variable<p_tlist>* tl;
      Variable<p_nlist>* nl;
      Variable<p_list>* l;

      return osGenerators.find(word) != osGenerators.end()
          || this->getVar(tk, b, p2)
          || this->getVar(tk, t, p2)
          || this->getVar(tk, p, p2)
          || this->getVar(tk, s, p2)
          || this->getVar(tk, n, p2)
          || this->getVar(tk, tl, p2)
          || this->getVar(tk, nl, p2)
          || this->getVar(tk, l, p2);
   }

   void Contexts::closeAttributeScope()
   {
      for (FileContext*& fc : this->fileContexts) {
         fc->attributeScope = false;
      }
   }

   void Contexts::closeDeepAttributeScope()
   {
      const p_size limit = this->fileContexts.size() - 1;
      for (p_size i = 0; i < limit; i++) {
         this->fileContexts[i]->attributeScope = false;
      }
   }

}
