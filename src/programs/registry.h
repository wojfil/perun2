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

#ifndef REGISTRY_H_INCLUDED
#define REGISTRY_H_INCLUDED

#include "../datatype/generator.h"
#include "pattern.h"
#include <windows.h>


namespace perun2::prog
{


enum RegistryRootType
{
   ClassesRoot,
   CurrentUser,
   LocalMachine
};


struct RegistryIterator : Generator<p_str>
{
public:
   RegistryIterator(const RegistryRootType type, p_genptr<p_str>& rt);
   virtual void reset() = 0;
   virtual p_bool hasNext() = 0;
   p_str getValue() override;

protected:
   HKEY getRootKey() const;

   p_genptr<p_str> root;
   p_str value;

private:
   const RegistryRootType rootType;
};


typedef std::unique_ptr<RegistryIterator> p_riptr;


struct MultiRegistry : RegistryIterator
{
public:
   MultiRegistry(const RegistryRootType type, p_genptr<p_str>& rt, const p_str& pattern);
   void reset() override;
   p_bool hasNext() override;

private:
   p_bool first = true;
   ProgramPatternComparer comparer;

   HKEY key;
   LONG result;
   DWORD index = 0;
   WCHAR subkeyName[MAX_PATH];
   DWORD subkeyNameSize = MAX_PATH;
};



}

#endif // REGISTRY_H_INCLUDED
