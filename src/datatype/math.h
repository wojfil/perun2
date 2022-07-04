#ifndef MATH_H_INCLUDED
#define MATH_H_INCLUDED

#include "primitives.h"
#include <random>


struct Math {

public:

   Math();
   void init();
   _ndouble randomDouble();
   _nint randomInt(const _nint& limit);
   void setAnyRandomDouble();

private:

   _boo anyRandomDouble;
   std::mt19937 generator;
   std::uniform_real_distribution<> doubleDist;
};


#endif // MATH_H_INCLUDED
