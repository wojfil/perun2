#ifndef GEN_MEMORY_H_INCLUDED
#define GEN_MEMORY_H_INCLUDED

#include "definition.h"
#include <memory>
#include <vector>


namespace uro
{

template <typename T>
using _genptr = std::unique_ptr<Generator<T>>;

typedef std::unique_ptr<Definition> _defptr;


template <typename T>
void transferGenPtrs(std::vector<_genptr<T>>& source, std::vector<_genptr<T>>& destination)
{
   const _size len = source.size();
   for (_size i = 0; i < len; i++) {
      _genptr<T>& s = source[i];
      destination.push_back(std::move(source[i]));
   }
};


}




#endif // GEN_MEMORY_H_INCLUDED
