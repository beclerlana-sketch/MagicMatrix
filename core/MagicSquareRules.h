#ifndef MAGICSQUARERULES_H
#define MAGICSQUARERULES_H

#include "GameTypes.h"

class MagicSquareRules
{
public:
    static constexpr int BoardSize = 9;
     static constexpr int BlockSize  = 3;
      static constexpr int CellCount  = BoardSize * BoardSize;
       static constexpr int EmptyValue = 0;

      bool isValueInRange(int value) const; // will check if the  value 1<= &&<=9 . ie if it falls  in the required range

       bool rowHasDuplicate(const BoardSnapShot &board, int row) const;// checks if the row repeats num.
       bool columnHasDuplicate(const BoardSnapShot &board, int column) const;
       bool blockHasDuplicate(const BoardSnapShot &board, int blockRow, int blockColumn) const;

       bool hasAnyDuplicate(const BoardSnapShot &board) const;
       bool isComplete(const BoardSnapShot &board) const;
       bool isSolved(const BoardSnapShot &board) const;
   private:
       bool isValidRowColumn(int index) const;
};


#endif // MAGICSQUARERULES_H
