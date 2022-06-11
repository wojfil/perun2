/*
    This file is part of Uroboros.
    Uroboros is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    Uroboros is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with Uroboros. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GENERATOR_H
#define GENERATOR_H


// generator is a structure
// that generates a new instance of a certain data type
// when is called
template <typename T>
struct Generator
{
   virtual T getValue() = 0;
};


// reference to a generator
//   why not use a normal pointer ?
//   because this struct deliberately doesn't free its memory
//   when "delete" is called upon it
template <typename T>
struct GeneratorRef : Generator<T>
{
public:
   GeneratorRef(Generator<T>* val) : value(val) {};

   T getValue() override {
      return value->getValue();
   }

   Generator<T>* value;
};


#endif /* GENERATOR_H */
