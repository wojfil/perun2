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

#include "com-condition.h"
#include "../uroboros.h"


void CS_If::setMain(Command* com)
{
   mainCommand = com;
   hasMain = true;
}


void CS_If::setElse(Command* com)
{
   elseCommand = com;
   hasElse = true;
   hasAlternatives = true;
}


void CS_If::addElseIf(Generator<_boo>* cond, Command* com)
{
   elseIfConditions.push_back(cond);
   elseIfCommands.push_back(com);
   elseIfCount++;
   hasAlternatives = true;
}


void CS_If::run()
{
   if (this->uroboros->running) {
      if (condition->getValue()) {
         if (hasMain) {
            mainCommand->run();
         }
      }
      else if (hasAlternatives) {
         for (_size i = 0; i < elseIfCount && this->uroboros->running; i++) {
            if ((elseIfConditions[i])->getValue()) {
               (elseIfCommands[i])->run();
               return;
            }
         }
         if (this->uroboros->running && hasElse) {
            elseCommand->run();
         }
      }
   }
}
