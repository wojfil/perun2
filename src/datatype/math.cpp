#include "math.h"


Math* g_math;


Math::Math() {
   anyRandomDouble = false;
}

void Math::init() {
   std::random_device rd;
   generator = std::mt19937(rd());

   if (anyRandomDouble) {
      doubleDist = std::uniform_real_distribution<>(0.0L, 1.0L);
   }
}

_ndouble Math::randomDouble() {
   return doubleDist(generator);
}

_nint Math::randomInt(const _nint& limit) {
   std::uniform_int_distribution<_nint> distribution(0LL, limit);
   return distribution(generator);
}

void Math::setAnyRandomDouble() {
   anyRandomDouble = true;
}
