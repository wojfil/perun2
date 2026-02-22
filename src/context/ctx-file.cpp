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

#include "ctx-file.h"
#include "../perun2.h"
#include "../os/os.h"


namespace perun2
{

   FileContext::FileContext(Perun2Process& p2)
      : IndexContext(p2), 
      perun2(p2), attribute(std::make_unique<Attribute>()),
      this_(std::make_unique<Variable<p_str>>(VarType::vt_Special)),
      locContext(p2.contexts.getLocationContext())
   {
      this->initVars();
   };

   FileContext::FileContext(p_attrptr& attr, Perun2Process& p2)
      : IndexContext(p2), 
      perun2(p2), attribute(std::move(attr)),
      this_(std::make_unique<Variable<p_str>>(VarType::vt_Special)),
      locContext(p2.contexts.getLocationContext())
   {
      this->initVars();
   };


   void FileContext::initVars()
   {
      this->v_archive = this->insertVar<p_bool>(STRING_ARCHIVE);
      this->v_compressed = this->insertVar<p_bool>(STRING_COMPRESSED);
      this->v_empty = this->insertVar<p_bool>(STRING_EMPTY);
      this->v_encrypted = this->insertVar<p_bool>(STRING_ENCRYPTED);
      this->v_exists = this->insertVar<p_bool>(STRING_EXISTS);
      this->v_hidden = this->insertVar<p_bool>(STRING_HIDDEN);
      this->v_isdirectory = this->insertVar<p_bool>(STRING_ISDIRECTORY);
      this->v_isfile = this->insertVar<p_bool>(STRING_ISFILE);
      this->v_isimage = this->insertVar<p_bool>(STRING_ISIMAGE);
      this->v_isvideo = this->insertVar<p_bool>(STRING_ISVIDEO);
      this->v_readonly = this->insertVar<p_bool>(STRING_READONLY);
      this->v_access = this->insertVar<p_tim>(STRING_ACCESS);
      this->v_change = this->insertVar<p_tim>(STRING_CHANGE);
      this->v_creation = this->insertVar<p_tim>(STRING_CREATION);
      this->v_modification = this->insertVar<p_tim>(STRING_MODIFICATION);
      this->v_lifetime = this->insertVar<p_per>(STRING_LIFETIME);
      this->v_size = this->insertVar<p_num>(STRING_SIZE);
      this->v_depth = this->insertVar<p_num>(STRING_DEPTH);
      this->v_width = this->insertVar<p_num>(STRING_WIDTH);
      this->v_height = this->insertVar<p_num>(STRING_HEIGHT);
      this->v_drive = this->insertVar<p_str>(STRING_DRIVE);
      this->v_extension = this->insertVar<p_str>(STRING_EXTENSION);
      this->v_fullname = this->insertVar<p_str>(STRING_FULLNAME);
      this->v_name = this->insertVar<p_str>(STRING_NAME);
      this->v_parent = this->insertVar<p_str>(STRING_PARENT);
      this->v_path = this->insertVar<p_str>(STRING_PATH);
      this->v_duration = this->insertVar<p_per>(STRING_DURATION);
   }


   void FileContext::loadData(const p_str& newThis)
   {
      this->this_->value = newThis;
      this->loadAttributes();
   }
   
   void FileContext::loadData(const p_str& newThis, const p_fdata& data)
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

   void FileContext::loadAttributes(const p_fdata& data)
   {
      if (this->attribute->hasAny()) {
         os_loadDataAttributes(*this, data);
      }
   }

}
