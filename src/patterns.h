#ifndef PATTERNS_H_INCLUDED
#define PATTERNS_H_INCLUDED

// todo add explanation comment
#define P_MEMORY_MEMBER _str prevThis; _numi prevIndex; _numi prevDepth;
#define P_MEMORY_LOAD this->prevThis = this->inner->this_s.value; this->prevIndex = this->inner->index.value; this->prevDepth = this->inner->depth.value;
#define P_MEMORY_RESTORE this->inner->this_s.value = this->prevThis; this->inner->index.value = this->prevIndex; this->inner->depth.value = this->prevDepth;

#endif // PATTERNS_H_INCLUDED
