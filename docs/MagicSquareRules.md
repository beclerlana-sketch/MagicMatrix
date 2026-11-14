# MagicMatrix Rules Walkthrough

This guide explains what the `MagicSquareRules` functions check and how they fit together. The class name says “Magic Square,” but these checks implement **Sudoku-style rules**: digits 1–9 may appear only once in each row, column, and 3×3 block.

## The board snapshot

`BoardSnapShot` is a copy of the board values. It stores 81 integers in one flat `std::array`, rather than as nine separate rows. The rules inspect this snapshot and return `bool` results; they do not change the board.

`MagicSquareRules` defines these constants:

| Constant | Meaning |
| --- | --- |
| `BoardSize` | 9 rows and 9 columns |
| `BlockSize` | 3 cells per side of a 3×3 block; there are also 3 blocks per direction |
| `CellCount` | 81 cells (`BoardSize * BoardSize`) |
| `EmptyValue` | 0, meaning the cell is blank |

Rows, columns, and block coordinates start at **0**. To find a cell in the flat array, use:

```cpp
index = row * BoardSize + column;
```

For example, row 4, column 7 is array index `4 * 9 + 7`, or `43`. The first cell (row 0, column 0) is index 0; the last (row 8, column 8) is index 80.

## Checking digits and coordinates

### `isValueInRange(int value)`

Returns `true` only for digits 1 through 9. Zero is deliberately not in range because it means “empty.” The duplicate checks use this to avoid treating invalid values as digits (and to keep them from being used as indexes into their `seen` arrays). `isComplete` uses it to reject both empty and invalid cells.

### `isValidRowColumn(int index)`

This is a private helper. It returns `true` when a row or column index is from 0 through 8. The row and column duplicate checks use it before looking at the board.

If a row, column, or block coordinate is out of range, its duplicate-check function returns `false`. That means “no duplicate was reported”; it does **not** mean that the coordinate was valid. Callers should pass valid coordinates.

## Checking one row or column

### `rowHasDuplicate(const BoardSnapShot &board, int row)`

Checks the nine cells in one row. It skips empty cells and values outside 1–9. A `seen` array remembers each valid digit already encountered. If the same digit appears again, the function returns `true`; otherwise it returns `false` after scanning the row.

### `columnHasDuplicate(const BoardSnapShot &board, int column)`

Does the same check down one column. The flat index for each cell is `row * BoardSize + column`: the row changes as the loop moves downward, while the column stays fixed.

For both functions, `true` means a repeated valid digit was found. `false` means no duplicate was found in the requested line (or the requested index was invalid).

## Checking a 3×3 block

### `blockHasDuplicate(const BoardSnapShot &board, int blockRow, int blockColumn)`

The block coordinates are each 0, 1, or 2. For example, `(0, 2)` selects the top-right block. Multiplying each block coordinate by `BlockSize` finds the first row and column of that block:

```cpp
const int startRow = blockRow * BlockSize;
const int startColumn = blockColumn * BlockSize;
```

For `(0, 2)`, the starting cell is row 0, column 6. The nested loops add offsets 0, 1, and 2, so they visit rows 0–2 and columns 6–8: all nine cells in that block.

```cpp
std::array<bool, 10> seen{}; // All entries start false; digit 0 is unused.

for (int rowOffset = 0; rowOffset < BlockSize; ++rowOffset) {
    for (int columnOffset = 0; columnOffset < BlockSize; ++columnOffset) {
        const int row = startRow + rowOffset;
        const int column = startColumn + columnOffset;
        const int index = row * BoardSize + column;
        const int value = board.values[index];

        if (value == EmptyValue || !isValueInRange(value))
            continue; // Blank and invalid values are not checked as digits.

        if (seen[value])
            return true; // This digit already appeared in this block.

        seen[value] = true; // Remember the digit for the next cells.
    }
}

return false; // All nine cells were checked without finding a repeat.
```

The real function also checks that the block coordinates are in range before calculating the cells.

## Checking the whole board

### `hasAnyDuplicate(const BoardSnapShot &board)`

This combines the smaller checks. First it checks all nine rows and columns. If either check finds a duplicate, `||` makes the condition true and the function returns immediately. If those pass, nested loops check all nine blocks: block rows 0–2 and block columns 0–2.

```cpp
for (int index = 0; index < BoardSize; ++index) {
    if (rowHasDuplicate(board, index) || columnHasDuplicate(board, index))
        return true; // A row or column contains a repeated digit.
}

for (int blockRow = 0; blockRow < BlockSize; ++blockRow) {
    for (int blockColumn = 0; blockColumn < BlockSize; ++blockColumn) {
        if (blockHasDuplicate(board, blockRow, blockColumn))
            return true; // This 3x3 block contains a repeated digit.
    }
}

return false; // No duplicate was found anywhere.
```

In short: `true` means **at least one duplicate exists**; `false` means **none was found**. Invalid cell values are skipped by duplicate checks, so use `isComplete` as well when deciding whether the board is valid and filled.

## Complete versus solved

### `isComplete(const BoardSnapShot &board)`

Looks at every value in the 81-cell array. It returns `true` only if every cell contains a valid digit from 1 to 9. An empty cell (`0`) or any other out-of-range value makes it return `false`. It does not check for duplicates.

### `isSolved(const BoardSnapShot &board)`

Returns:

```cpp
isComplete(board) && !hasAnyDuplicate(board)
```

So a solved board must be both **complete** and **free of duplicates**. The `&&` means that if the board is incomplete, the duplicate check is not needed.

## How the checks fit together

```text
rowHasDuplicate ─┐
columnHasDuplicate ─┼─> hasAnyDuplicate ─┐
blockHasDuplicate ──┘                    ├─> isSolved
isComplete ─────────────────────────────┘
```

`isValueInRange` supports the digit checks and completeness check. `isValidRowColumn` supports row and column bounds checks. The line and block checks inspect one part of the snapshot; the whole-board checks combine their results.
