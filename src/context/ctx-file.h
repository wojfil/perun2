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

#ifndef CTX_FILE_H_INCLUDED
#define CTX_FILE_H_INCLUDED

#include "ctx-index.h"
#include "ctx-location.h"
#include "../attribute.h"


namespace perun2
{

   struct FileContext : IndexContext
   {

   public:
      FileContext() = delete;
      FileContext(p_perun2& p2);
      FileContext(p_attrptr& attr, p_perun2& p2);

      void loadData(const p_str& newThis);
      void loadData(const p_str& newThis, const p_fdata& data);
      void reloadData();
      void loadAttributes();
      void loadAttributes(const p_fdata& data);

      p_bool attributeScope = true;
      p_varptr<p_str> this_;
      p_attrptr attribute;
      VarsContext fileVars;
      LocationContext* const locContext;
      p_str trimmed; // the value of this_, but with trimmed spaces and with proper path separators
      p_bool invalid; // the value of this_ is invalid (has not allowed characters, is empty string etc.)
      p_bool isInside = false; // this is context of the Inside structure

      Variable<p_bool>* v_archive;
      Variable<p_bool>* v_compressed;
      Variable<p_bool>* v_empty;
      Variable<p_bool>* v_encrypted;
      Variable<p_bool>* v_exists;
      Variable<p_bool>* v_hidden;
      Variable<p_bool>* v_isdirectory;
      Variable<p_bool>* v_isfile;
      Variable<p_bool>* v_readonly;
      Variable<p_tim>* v_access;
      Variable<p_tim>* v_change;
      Variable<p_tim>* v_creation;
      Variable<p_tim>* v_modification;
      Variable<p_per>* v_lifetime;
      Variable<p_num>* v_size;
      Variable<p_num>* v_depth;
      Variable<p_str>* v_drive;
      Variable<p_str>* v_extension;
      Variable<p_str>* v_fullname;
      Variable<p_str>* v_name;
      Variable<p_str>* v_parent;
      Variable<p_str>* v_path;

   private:
      void initVars(p_perun2& p2);

      template <typename T>
      Variable<T>* insertVar(const p_str& name)
      {
         return this->fileVars.insertVar<T>(name, VarType::vt_Attribute);
      }
   };


   typedef std::unique_ptr<FileContext>            p_fcptr;
}

#endif // CTX_FILE_H_INCLUDED
