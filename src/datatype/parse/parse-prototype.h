#ifndef PARSE_PROTOTYPE_H_INCLUDED
#define PARSE_PROTOTYPE_H_INCLUDED

#include "../../uroboros.h"
#include "../generator/gen-generic.h"


enum FilterType
{
   ft_Where,
   ft_Numeric
};

template <typename T>
struct FilterPrototype
{
public:
   virtual ~FilterPrototype() { };
   virtual FilterType getFilterType() = 0;
   virtual T build(T base, Attribute* attr, const _boo& hasMem, Uroboros* uro) = 0;
};

template <typename T>
void makeWhereFilter(Generator<_boo>* boo, Attribute* attr,
   const _boo& hasMemory, Generator<std::vector<T>>*& result, Uroboros* uro)
{
   result = new Filter_Where<T>(result, boo, attr, uro);
}

template <typename T>
void makeWhereFilter(Generator<_boo>* boo, Generator<std::vector<T>>*& result, Uroboros* uro)
{
   result = new Filter_Where<T>(result, boo, nullptr, uro);
}

void makeWhereFilter(Generator<_boo>* boo, Attribute* attr, const _boo& hasMemory, _def*& result, Uroboros* uro);
void makeWhereFilter(Generator<_boo>* boo, _def*& result, Uroboros* uro);

template <typename T>
void makeNumericFilter(const Keyword& kw, Generator<_num>* num, Generator<std::vector<T>>*& result, Uroboros* uro)
{
   switch (kw) {
      case Keyword::kw_Every: {
         result = new Filter_Every<T>(result, num);
         break;
      }
      case Keyword::kw_Limit: {
         result = new Filter_Limit<T>(result, num);
         break;
      }
      case Keyword::kw_Skip: {
         result = new Filter_Skip<T>(result, num);
         break;
      }
   }
}

void makeNumericFilter(const Keyword& kw, Generator<_num>* num, _def*& result, Uroboros* uro);

template <typename T>
struct FP_Where : FilterPrototype<T>
{
public:
   FP_Where(Generator<_boo>* cond)
      : condition(cond) { };

   ~FP_Where()
   {
      if (condition != nullptr) {
         delete condition;
      }
   };

   FilterType getFilterType() override
   {
      return FilterType::ft_Where;
   };

   T build(T base, Attribute* attr, const _boo& hasMem, Uroboros* uro) override
   {
      T result = base;

      if (attr == nullptr) {
         makeWhereFilter(condition, result, uro);
      }
      else {
         makeWhereFilter(condition, attr, hasMem, result, uro);
      }

      condition = nullptr;
      return result;
   };

private:
   Generator<_boo>* condition;

};

template <typename T>
struct FP_Numeric : FilterPrototype<T>
{
public:
   FP_Numeric(Generator<_num>* val, const Keyword& kw)
      : value(val), keyword(kw) { };

   ~FP_Numeric()
   {
      if (value != nullptr) {
         delete value;
      }
   };

   FilterType getFilterType() override
   {
      return FilterType::ft_Numeric;
   };

   T build(T base, Attribute* attr, const _boo& hasMem, Uroboros* uro) override
   {
      T result = base;
      makeNumericFilter(keyword, value, result, uro);
      value = nullptr;
      return result;
   };

private:
   Generator<_num>* value;
   const Keyword keyword;
};


#endif // PARSE_PROTOTYPE_H_INCLUDED
