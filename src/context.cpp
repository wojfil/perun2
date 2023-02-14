/*
    This file is part of Uroboros2.
    Uroboros2 is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros2 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros2. If not, see <http://www.gnu.org/licenses/>.
*/

#include "context.h"
#include "uroboros.h"
#include "datatype/generator/gen-time.h"


namespace uro
{

   AggregateContext::AggregateContext(_uro& uro)
      : aggregate(uro) { };

   LocationContext::LocationContext()
      : location(std::make_unique<vars::Variable<_str>>(vars::VarType::vt_Special)),
        prevLocation(nullptr) { };

   LocationContext::LocationContext(LocationContext* prev)
      : location(std::make_unique<vars::Variable<_str>>(vars::VarType::vt_Special)),
        prevLocation(nullptr) { };

   void LocationContext::loadData(const _str& newThis)
   {
      this->location->value = this->prevLocation == nullptr
         ? newThis
         : os_join(this->prevLocation->location->value, newThis);
   }

   IndexContext::IndexContext(_uro& uro)
      : AggregateContext(uro), 
        index(std::make_unique<vars::Variable<_num>>(vars::VarType::vt_Special)) { };

   FileContext::FileContext(_uro& uro) 
      : IndexContext(uro), attribute(std::make_unique<Attribute>(uro)),
      this_(std::make_unique<vars::Variable<_str>>(vars::VarType::vt_Special))
   {
      this->initVars(uro); 
   };

   FileContext::FileContext(_attrptr& attr, _uro& uro)
      : IndexContext(uro), attribute(std::move(attr)),
      this_(std::make_unique<vars::Variable<_str>>(vars::VarType::vt_Special))
   { 
      this->initVars(uro); 
   };

   void IndexContext::resetIndex()
   {
      this->index->value.value.i = NINT_ZERO;
   }

   void IndexContext::incrementIndex()
   {
      this->index->value.value.i++;
   }
   
   void FileContext::initVars(_uro& uro)
   {
      this->addVar<_bool>(uro.hashes.HASH_VAR_ARCHIVE);
      this->addVar<_bool>(uro.hashes.HASH_VAR_COMPRESSED);
      this->addVar<_bool>(uro.hashes.HASH_VAR_EMPTY);
      this->addVar<_bool>(uro.hashes.HASH_VAR_ENCRYPTED);
      this->addVar<_bool>(uro.hashes.HASH_VAR_EXISTS);
      this->addVar<_bool>(uro.hashes.HASH_VAR_HIDDEN);
      this->addVar<_bool>(uro.hashes.HASH_VAR_ISDIRECTORY);
      this->addVar<_bool>(uro.hashes.HASH_VAR_ISFILE);
      this->addVar<_bool>(uro.hashes.HASH_VAR_READONLY);
      this->addVar<_tim>(uro.hashes.HASH_VAR_ACCESS);
      this->addVar<_tim>(uro.hashes.HASH_VAR_CHANGE);
      this->addVar<_tim>(uro.hashes.HASH_VAR_CREATION);
      this->addVar<_tim>(uro.hashes.HASH_VAR_MODIFICATION);
      this->addVar<_per>(uro.hashes.HASH_VAR_LIFETIME);
      this->addVar<_num>(uro.hashes.HASH_VAR_SIZE);
      this->addVar<_num>(uro.hashes.HASH_VAR_DEPTH);
      this->addVar<_str>(uro.hashes.HASH_VAR_DRIVE);
      this->addVar<_str>(uro.hashes.HASH_VAR_EXTENSION);
      this->addVar<_str>(uro.hashes.HASH_VAR_FULLNAME);
      this->addVar<_str>(uro.hashes.HASH_VAR_NAME);
      this->addVar<_str>(uro.hashes.HASH_VAR_PARENT);
      this->addVar<_str>(uro.hashes.HASH_VAR_PATH);

      this->v_archive = this->fileVars.bools.at(uro.hashes.HASH_VAR_ARCHIVE).get();
      this->v_compressed = this->fileVars.bools.at(uro.hashes.HASH_VAR_COMPRESSED).get();
      this->v_empty = this->fileVars.bools.at(uro.hashes.HASH_VAR_EMPTY).get();
      this->v_encrypted = this->fileVars.bools.at(uro.hashes.HASH_VAR_ENCRYPTED).get();
      this->v_exists = this->fileVars.bools.at(uro.hashes.HASH_VAR_EXISTS).get();
      this->v_hidden = this->fileVars.bools.at(uro.hashes.HASH_VAR_HIDDEN).get();
      this->v_isdirectory = this->fileVars.bools.at(uro.hashes.HASH_VAR_ISDIRECTORY).get();
      this->v_isfile = this->fileVars.bools.at(uro.hashes.HASH_VAR_ISFILE).get();
      this->v_readonly = this->fileVars.bools.at(uro.hashes.HASH_VAR_READONLY).get();
      this->v_access = this->fileVars.times.at(uro.hashes.HASH_VAR_ACCESS).get();
      this->v_change = this->fileVars.times.at(uro.hashes.HASH_VAR_CHANGE).get();
      this->v_creation = this->fileVars.times.at(uro.hashes.HASH_VAR_CREATION).get();
      this->v_modification = this->fileVars.times.at(uro.hashes.HASH_VAR_MODIFICATION).get();
      this->v_lifetime = this->fileVars.periods.at(uro.hashes.HASH_VAR_LIFETIME).get();
      this->v_size = this->fileVars.numbers.at(uro.hashes.HASH_VAR_SIZE).get();
      this->v_depth = this->fileVars.numbers.at(uro.hashes.HASH_VAR_DEPTH).get();
      this->v_drive = this->fileVars.strings.at(uro.hashes.HASH_VAR_DRIVE).get();
      this->v_extension = this->fileVars.strings.at(uro.hashes.HASH_VAR_EXTENSION).get();
      this->v_fullname = this->fileVars.strings.at(uro.hashes.HASH_VAR_FULLNAME).get();
      this->v_name = this->fileVars.strings.at(uro.hashes.HASH_VAR_NAME).get();
      this->v_parent = this->fileVars.strings.at(uro.hashes.HASH_VAR_PARENT).get();
      this->v_path = this->fileVars.strings.at(uro.hashes.HASH_VAR_PATH).get();
   }

   
   void FileContext::loadData(const _str& newThis)
   {
      this->this_->value = newThis;
      this->loadAttributes();
   }

   void FileContext::reloadData()
   {
      this->loadAttributes();
   }

   void FileContext::loadAttributes()
   {
      // todo load attributes
   }

   void FileContext::resetIndexAndDepth()
   {
      this->resetIndex();
      this->v_depth->value.setToZero();
   }

   GlobalContext::GlobalContext(_uro& uro)
   {
      this->globalVars.times.insert(std::make_pair(uro.hashes.HASH_VAR_NOW, std::make_unique<gen::v_Now>()));
      this->globalVars.times.insert(std::make_pair(uro.hashes.HASH_VAR_TODAY, std::make_unique<gen::v_Today>()));
      this->globalVars.times.insert(std::make_pair(uro.hashes.HASH_VAR_YESTERDAY, std::make_unique<gen::v_Yesterday>()));
      this->globalVars.times.insert(std::make_pair(uro.hashes.HASH_VAR_TOMORROW, std::make_unique<gen::v_Tomorrow>()));

      this->globalVars.strings.insert(std::make_pair(uro.hashes.HASH_VAR_DESKTOP, 
         std::make_unique<vars::Variable<_str>>(vars::VarType::vt_Special, os_desktopPath())));
      this->globalVars.strings.insert(std::make_pair(uro.hashes.HASH_VAR_UROBOROS2, 
         std::make_unique<vars::Variable<_str>>(vars::VarType::vt_Special, os_uroborosPath())));
      this->globalVars.strings.insert(std::make_pair(uro.hashes.HASH_VAR_ROOT, 
         std::make_unique<vars::Variable<_str>>(vars::VarType::vt_Special, uro.arguments.getLocation())));

      this->globalVars.lists.insert(std::make_pair(uro.hashes.HASH_VAR_ALPHABET, 
         std::make_unique<vars::Variable<_list>>(vars::VarType::vt_Special, getAlphabet())));
      this->globalVars.lists.insert(std::make_pair(uro.hashes.HASH_VAR_ASCII, 
         std::make_unique<vars::Variable<_list>>(vars::VarType::vt_Special, STRINGS_ASCII)));
      this->globalVars.lists.insert(std::make_pair(uro.hashes.HASH_VAR_ARGUMENTS, 
         std::make_unique<vars::Variable<_list>>(vars::VarType::vt_Special, uro.arguments.getArgs())));
   };
   
   
   Contexts::Contexts(_uro& uro) 
      : GlobalContext(uro), hashes(uro.hashes)
   { 
      this->locationContexts.push_back(&this->rootLocation);
      this->locationContexts.back()->location->value = uro.arguments.getLocation();

      this->addOsGen(hashes.HASH_VAR_FILES, gen::OsElement::oe_Files, uro);
      this->addOsGen(hashes.HASH_VAR_RECURSIVEFILES, gen::OsElement::oe_RecursiveFiles, uro);
      this->addOsGen(hashes.HASH_VAR_DIRECTORIES, gen::OsElement::oe_Directories, uro);
      this->addOsGen(hashes.HASH_VAR_RECURSIVEDIRECTORIES, gen::OsElement::oe_RecursiveDirectories, uro);
   };
   
   _bool Contexts::getVariable(const Token& tk, vars::Variable<_bool>*& result, _uro& uro)
   {
      const _size& var = tk.value.word.h;

      if (var == this->hashes.HASH_VAR_SUCCESS) {
         result = this->success.get();
         return true;
      }

      return findVar(tk, result, uro);
   }

   _bool Contexts::getVariable(const Token& tk, vars::Variable<_num>*& result, _uro& uro)
   {
      const _size& var = tk.value.word.h;

      if (var == this->hashes.HASH_VAR_INDEX) {
         if (this->indexContexts.empty()) {
            throw SyntaxError::undefinedVarValue(tk.getOriginString(uro), tk.line);
            return false;
         }

         IndexContext* last = this->indexContexts.back();
         if (last != nullptr) {
            result = last->index.get();
            return true;
         }
      }

      return findVar(tk, result, uro);
   }

   _bool Contexts::getVariable(const Token& tk, vars::Variable<_str>*& result, _uro& uro)
   {
      const _size& var = tk.value.word.h;
      
      if (var == this->hashes.HASH_VAR_THIS) {
         if (this->fileContexts.empty()) {
            throw SyntaxError::undefinedVarValue(tk.getOriginString(uro), tk.line);
            return false;
         }

         FileContext* last = this->fileContexts.back();
         if (last != nullptr) {
            result = last->this_.get();
            return true;
         }
      }
      else if (var == this->hashes.HASH_VAR_LOCATION) {
         if (this->locationContexts.empty()) {
            throw SyntaxError::undefinedVarValue(tk.getOriginString(uro), tk.line);
            return false;
         }

         LocationContext* last = this->locationContexts.back();
         if (last != nullptr) {
            result = last->location.get();
            return true;
         }
      }

      return findVar(tk, result, uro);
   }

   _bool Contexts::getVariable(const Token& tk, _defptr& result, _uro& uro)
   {
      const _size& var = tk.value.word.h;
      auto v = this->osGenerators.find(var);

      if (v != this->osGenerators.end()) {
         return v->second.generate(result);
      }

      return false;
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



   void Contexts::markAllAttributesToRun()
   {
      for (FileContext*& fc : this->fileContexts) {
         if (fc != nullptr) {
            fc->attribute->markToRun();
         }
      }
   }

   
   _bool Contexts::nextContextWillBeAggregate()
   {
      return this->aggregateContexts.size() == 1;
   }

   _bool Contexts::hasAggregate()
   {
      return this->aggregateContexts.size() > 1;
   }

   comm::Aggregate* Contexts::getAggregate()
   {
      return &this->aggregateContexts[1]->aggregate;
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

   std::vector<FileContext*>& Contexts::getFileContexts() 
   {
      return this->fileContexts;
   }

   _bool Contexts::hasIndexContext() const
   {
      return !this->indexContexts.empty();
   }

   void Contexts::addOsGen(const _size& hash, const gen::OsElement& element, _uro& uro)
   {
      osGenerators.insert(std::make_pair(hash, gen::DefinitionGenerator(element, uro)));   
   }
   
   void Contexts::makeLocationContext(_lcptr& result)
   {  
      result = std::make_unique<LocationContext>(this->locationContexts.back());
   }

   UserVarsContext* Contexts::getUserVarsContext()
   {
      return this->userVarsContexts.back();
   }

}
