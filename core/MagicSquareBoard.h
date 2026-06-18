#ifndef MAGICSQUAREBOARD_H
#define MAGICSQUAREBOARD_H

#include "GameTypes.h"

class MagicSquareBoard
{
public :
    static constexpr int CellCount = 81 ; // bieng knouwn to the compiler at  compilation time
    static constexpr int EmptyValue = 0;
    int valueAt( int index)  const ; // it will resturn the value  stored in the cell
    bool setValue ( int index, int value ); // this will check the number placed in the cell and check if accepted it w will return true or else a false
    bool claerValue ( int index );
    BoardSnapShot snapshot() const ;

private:
    bool isValidIndex (int index) const ; // check if the cell index is in the valid range
    bool isValidValue(int value) const;
    BoardSnapShot m_snapshot; // helps  stores the data inside the class




};


#endif // MAGICSQUAREBOARD_H
