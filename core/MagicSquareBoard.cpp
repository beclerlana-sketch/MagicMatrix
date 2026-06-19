#include "MagicSquareBoard.h"

int MagicSquareBoard::valueAt(int index) const
{
    if (!isValidIndex(index))
        return EmptyValue;

    return m_snapshot.values[index];
}
bool MagicSquareBoard:: setValue(int index, int value){
    if (!isValidIndex(index) || !isValidValue(value))// if the  value isnt btw the index range it regects it .

    return false;
m_snapshot.values[index] = value;
    return true;
}
bool MagicSquareBoard::clearValue(int index){
    if(!isValidIndex(index))
    return false ;
m_snapshot.values[index] = EmptyValue;
    return true ;
}
BoardSnapShot MagicSquareBoard::snapshot() const
{
    return m_snapshot;
}
bool MagicSquareBoard::isValidIndex(int index) const
{
    return index >= 0 && index < CellCount;
}
bool MagicSquareBoard::isValidValue(int value) const
{
    return value >= 1 && value <= 81;
}


