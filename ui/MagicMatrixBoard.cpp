#include "MagicMatrixBoard.h"
#include "CellButton.h"

#include <QGridLayout>

MagicMatrixBoard::MagicMatrixBoard(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QGridLayout(this);
    layout->setSpacing(2);
    layout->setContentsMargins(0, 0, 0, 0);

    matrix_cells.reserve(CellCount);

    for (int index = 0; index < CellCount; ++index) {
        auto* cell = new CellButton(index, this);
        matrix_cells.append(cell);

        const int row = index / BoardSize;
        const int column = index % BoardSize;
        layout->addWidget(cell, row, column);

        connect(cell, &CellButton::cellClicked, this, [this](int clickedIndex) {
            setHighlightedCell(clickedIndex);
            emit cellHighlighted(clickedIndex);
        });

    }
};

void MagicMatrixBoard::setCellValue(int index, int value)
{
    if (index < 0 || index >= matrix_cells.size())
        return;

    matrix_cells[index]->setDigit(value);
}

void MagicMatrixBoard::setHighlightedCell(int index)
{
    if (index < 0 || index >= matrix_cells.size())
        return;

    if (matrix_HighlightedIndex >= 0 && matrix_HighlightedIndex < matrix_cells.size())
        matrix_cells[matrix_HighlightedIndex]->setHighlighted(false);

    matrix_HighlightedIndex = index;
    matrix_cells[matrix_HighlightedIndex]->setHighlighted(true);
}