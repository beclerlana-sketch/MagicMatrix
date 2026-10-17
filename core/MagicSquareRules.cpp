#include "MagicSquareRules.h"

#include <array>

bool MagicSquareRules::isValueInRange(int value) const
{
    return value >= 1 && value <= 9;
}

bool MagicSquareRules::isValidRowColumn(int index) const
{
    return index >= 0 && index < BoardSize;
}

bool MagicSquareRules::rowHasDuplicate(const BoardSnapShot &board, int row) const
{
    if (!isValidRowColumn(row))
        return false;

    // seen[digit] records whether the digit has already appeared in this row.
    std::array<bool, 10> seen{};

    for (int column = 0; column < BoardSize; ++column) {
        const int index = row * BoardSize + column;
        const int value = board.values[index];

        // Empty or out-of-range values are not valid digits to check for repeats.
        if (value == EmptyValue || !isValueInRange(value))
            continue;

        if (seen[value])
            return true;

        seen[value] = true;
    }

    return false;
}

bool MagicSquareRules::columnHasDuplicate(const BoardSnapShot &board, int column) const
{
    if (!isValidRowColumn(column))
        return false;

    // A fresh checklist is needed for each column.
    std::array<bool, 10> seen{};

    for (int row = 0; row < BoardSize; ++row) {
        const int index = row * BoardSize + column;
        const int value = board.values[index];

        if (value == EmptyValue || !isValueInRange(value))
            continue;

        if (seen[value])
            return true;

        seen[value] = true;
    }

    return false;
}

bool MagicSquareRules::blockHasDuplicate(const BoardSnapShot &board, int blockRow, int blockColumn) const
{
    // Block coordinates are 0, 1, or 2 (there are three blocks per direction).
    if (blockRow < 0 || blockRow >= BlockSize || blockColumn < 0 || blockColumn >= BlockSize)
        return false;

    // This checklist tracks digits already found in this 3x3 block.
    std::array<bool, 10> seen{};

    // Convert the block coordinates to the first cell's row and column.
    const int startRow = blockRow * BlockSize;
    const int startColumn = blockColumn * BlockSize;

    // Visit the nine cells inside this block.
    for (int rowOffset = 0; rowOffset < BlockSize; ++rowOffset) {
        for (int columnOffset = 0; columnOffset < BlockSize; ++columnOffset) {
            const int row = startRow + rowOffset;
            const int column = startColumn + columnOffset;
            const int index = row * BoardSize + column;
            const int value = board.values[index];

            if (value == EmptyValue || !isValueInRange(value))
                continue;

            if (seen[value])
                return true;

            seen[value] = true;
        }
    }

    return false;
}

bool MagicSquareRules::hasAnyDuplicate(const BoardSnapShot &board) const
{
    // Check every row and column; stop as soon as one contains a repeat.
    for (int index = 0; index < BoardSize; ++index) {
        if (rowHasDuplicate(board, index) || columnHasDuplicate(board, index))
            return true;
    }

    // Check all nine 3x3 blocks (three block rows by three block columns).
    for (int blockRow = 0; blockRow < BlockSize; ++blockRow) {
        for (int blockColumn = 0; blockColumn < BlockSize; ++blockColumn) {
            if (blockHasDuplicate(board, blockRow, blockColumn))
                return true;
        }
    }

    return false;
}

bool MagicSquareRules::isComplete(const BoardSnapShot &board) const
{
    // A cell is complete only when it contains a valid Sudoku digit, 1 through 9.
    for (int value : board.values) {
        if (!isValueInRange(value))
            return false;
    }

    return true;
}

bool MagicSquareRules::isSolved(const BoardSnapShot &board) const
{
    // A solved board must be filled and obey every duplicate rule.
    return isComplete(board) && !hasAnyDuplicate(board);
}
