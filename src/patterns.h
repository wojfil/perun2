#ifndef PATTERNS_H_INCLUDED
#define PATTERNS_H_INCLUDED


#define P_MEMORY_MEMBER _str prevThis; \
   _numi prevIndex; \
   _numi prevDepth;


#define P_MEMORY_LOAD this->prevThis = this->inner->this_s.value; \
   this->prevIndex = this->inner->index.value; \
   this->prevDepth = this->inner->depth.value;


#define P_MEMORY_RESTORE this->inner->this_s.value = this->prevThis; \
   this->inner->index.value = this->prevIndex; \
   this->inner->depth.value = this->prevDepth;


#define P_CHECK_LOOP_BREAK if (this->uroboros->break_) { \
      this->uroboros->running = true; \
      this->uroboros->break_ = false; \
      break; \
   } \
   else if (this->uroboros->continue_) { \
      this->uroboros->running = true; \
      this->uroboros->continue_ = false; \
      continue; \
   }


#define P_CHECK_SOFT_LOOP_BREAK if (this->uroboros->break_) { \
      this->uroboros->running = true; \
      this->uroboros->break_ = false; \
   } \
   else if (this->uroboros->continue_) { \
      this->uroboros->running = true; \
      this->uroboros->continue_ = false; \
   }

#define P_DIVIDE_BY_KEYWORD(kw) std::pair<Tokens, Tokens> pair = tks.divideByKeyword(Keyword::kw); \
   Tokens& left = pair.first; \
   Tokens& right = pair.second; \


#endif // PATTERNS_H_INCLUDED
