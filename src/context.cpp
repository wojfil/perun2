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

#include "context.h"
#include "perun2.h"
#include "datatype/generator/gen-time.h"
#include "os.h"

namespace perun2
{

   AggregateContext::AggregateContext(p_perun2& p2)
      : aggregate(p2) { };

   LocationContext::LocationContext()
      : location(std::make_unique<Variable<_str>>(VarType::vt_Special)),
        prevLocation(nullptr) { };

   LocationContext::LocationContext(LocationContext* prev)
      : location(std::make_unique<Variable<_str>>(VarType::vt_Special)),
        prevLocation(prev) { };

   void LocationContext::loadData(const _str& trimmedValue)
   {
      this->location->value = this->prevLocation == nullptr
         ? trimmedValue
         : os_leftJoin(this->prevLocation->location->value, trimmedValue);
   }

   IndexContext::IndexContext(p_perun2& p2)
      : AggregateContext(p2),
        index(std::make_unique<Variable<_num>>(VarType::vt_Special)) { };

   FileContext::FileContext(p_perun2& p2)
      : IndexContext(p2), attribute(std::make_unique<Attribute>(p2)),
      this_(std::make_unique<Variable<_str>>(VarType::vt_Special)),
      locContext(p2.contexts.getLocationContext())
   {
      this->initVars(p2);
   };

   FileContext::FileContext(_attrptr& attr, p_perun2& p2)
      : IndexContext(p2), attribute(std::move(attr)),
      this_(std::make_unique<Variable<_str>>(VarType::vt_Special)),
      locContext(p2.contexts.getLocationContext())
   {
      this->initVars(p2);
   };

   void IndexContext::resetIndex()
   {
      this->index->value.value.i = NINT_ZERO;
   }

   void IndexContext::incrementIndex()
   {
      this->index->value.value.i++;
   }

   void FileContext::initVars(p_perun2& p2)
   {
      this->v_archive = this->insertVar<_bool>(STRING_ARCHIVE);
      this->v_compressed = this->insertVar<_bool>(STRING_COMPRESSED);
      this->v_empty = this->insertVar<_bool>(STRING_EMPTY);
      this->v_encrypted = this->insertVar<_bool>(STRING_ENCRYPTED);
      this->v_exists = this->insertVar<_bool>(STRING_EXISTS);
      this->v_hidden = this->insertVar<_bool>(STRING_HIDDEN);
      this->v_isdirectory = this->insertVar<_bool>(STRING_ISDIRECTORY);
      this->v_isfile = this->insertVar<_bool>(STRING_ISFILE);
      this->v_readonly = this->insertVar<_bool>(STRING_READONLY);
      this->v_access = this->insertVar<_tim>(STRING_ACCESS);
      this->v_change = this->insertVar<_tim>(STRING_CHANGE);
      this->v_creation = this->insertVar<_tim>(STRING_CREATION);
      this->v_modification = this->insertVar<_tim>(STRING_MODIFICATION);
      this->v_lifetime = this->insertVar<_per>(STRING_LIFETIME);
      this->v_size = this->insertVar<_num>(STRING_SIZE);
      this->v_depth = this->insertVar<_num>(STRING_DEPTH);
      this->v_drive = this->insertVar<_str>(STRING_DRIVE);
      this->v_extension = this->insertVar<_str>(STRING_EXTENSION);
      this->v_fullname = this->insertVar<_str>(STRING_FULLNAME);
      this->v_name = this->insertVar<_str>(STRING_NAME);
      this->v_parent = this->insertVar<_str>(STRING_PARENT);
      this->v_path = this->insertVar<_str>(STRING_PATH);
   }


   void FileContext::loadData(const _str& newThis)
   {
      this->this_->value = newThis;
      this->loadAttributes();
   }
   
   void FileContext::loadData(const _str& newThis, const _fdata& data)
   {
      this->this_->value = newThis;
      this->loadAttributes(data);
   }

   void FileContext::reloadData()
   {
      this->loadAttributes();
   }

   void FileContext::loadAttributes()
   {
      if (this->attribute->hasAny()) {
         os_loadAttributes(*this);
      }
   }

   void FileContext::loadAttributes(const _fdata& data)
   {
      if (this->attribute->hasAny()) {
         os_loadDataAttributes(*this, data);
      }
   }

   GlobalContext::GlobalContext(p_perun2& p2)
   {
      this->globalVars.times.insert(std::make_pair(STRING_NOW, std::make_unique<gen::v_Now>()));
      this->globalVars.times.insert(std::make_pair(STRING_TODAY, std::make_unique<gen::v_Today>()));
      this->globalVars.times.insert(std::make_pair(STRING_YESTERDAY, std::make_unique<gen::v_Yesterday>()));
      this->globalVars.times.insert(std::make_pair(STRING_TOMORROW, std::make_unique<gen::v_Tomorrow>()));

      this->insertConstant<_str>(STRING_DESKTOP);
      this->insertConstant<_str>(STRING_PERUN2);
      this->insertConstant<_str>(STRING_ORIGIN);
      this->insertConstant<_list>(STRING_ALPHABET);
      this->insertConstant<_list>(STRING_ASCII);
      this->insertConstant<_list>(STRING_ARGUMENTS);
      this->insertConstant<_str>(STRING_MSPAINT);
      this->insertConstant<_str>(STRING_NOTEPAD);
   };

   Contexts::Contexts(p_perun2& p2)
      : GlobalContext(p2), success(std::make_unique<Variable<_bool>>(VarType::vt_Special, false))
   {
      this->locationContexts.push_back(&this->rootLocation);
      this->locationContexts.back()->location->value = p2.arguments.getLocation();

      this->addOsGen(STRING_FILES, gen::OsElement::oe_Files, p2);
      this->addOsGen(STRING_RECURSIVEFILES, gen::OsElement::oe_RecursiveFiles, p2);
      this->addOsGen(STRING_DIRECTORIES, gen::OsElement::oe_Directories, p2);
      this->addOsGen(STRING_RECURSIVEDIRECTORIES, gen::OsElement::oe_RecursiveDirectories, p2);
   };

   _bool Contexts::getVar(const Token& tk, Variable<_bool>*& result, p_perun2& p2)
   {
      if (tk.isWord(STRING_SUCCESS, p2)) {
         result = this->success.get();
         return true;
      }

      return findVar(tk, result, p2);
   }

   _bool Contexts::getVar(const Token& tk, Variable<_num>*& result, p_perun2& p2)
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

   _bool Contexts::getVar(const Token& tk, Variable<_str>*& result, p_perun2& p2)
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
      else if (tk.isWord(STRING_PAINT, p2)) {
         if (this->globalVars.takeVar(STRING_MSPAINT, result)) {
            p2.cache.actualize(tk);
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

   _bool Contexts::hasAggregate()
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

   _bool Contexts::hasIterationContext() const
   {
      return !this->fileContexts.empty();
   }

   _bool Contexts::hasFileContext() const
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

   _bool Contexts::hasIndexContext() const
   {
      return !this->indexContexts.empty();
   }

   void Contexts::addOsGen(const _str& name, const gen::OsElement element, p_perun2& p2)
   {
      osGenerators.insert(std::make_pair(name, gen::DefinitionGenerator(element, p2)));
   }

   void Contexts::makeLocationContext(_lcptr& result)
   {
      result = std::make_unique<LocationContext>(this->locationContexts.back());
   }

   UserVarsContext* Contexts::getUserVarsContext()
   {
      return this->userVarsContexts.back();
   }

   _bool Contexts::varExists(const Token& tk, p_perun2& p2)
   {
      const _str word = tk.toLowerString(p2);
      Variable<_bool>* b;
      Variable<_tim>* t;
      Variable<_per>* p;
      Variable<_str>* s;
      Variable<_num>* n;
      Variable<_tlist>* tl;
      Variable<_nlist>* nl;
      Variable<_list>* l;

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
      const _size limit = this->fileContexts.size() - 1;
      for (_size i = 0; i < limit; i++) {
         this->fileContexts[i]->attributeScope = false;
      }
   }
}
