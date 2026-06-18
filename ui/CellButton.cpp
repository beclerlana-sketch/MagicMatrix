#include "CellButton.h"

CellButton::CellButton(int cellIndex, QWidget *parent)
    : QPushButton(parent), matrix_digit(cellIndex)
{
    setMinimumSize(44, 44);
    setCheckable(false);

    connect(this, &QPushButton::clicked, this, [this]() {
        emit cellClicked(matrix_cellIndex);
    });

    updateAppearance();
}

int CellButton::cellIndex() const
{
    return matrix_cellIndex;
}

void CellButton::setDigit(int digit)
{
    matrix_digit = digit;
    updateAppearance();
}

void CellButton::setHighlighted(bool highlighted)
{
    matrix_highlighted = highlighted;
    updateAppearance();
}

void CellButton::updateAppearance()
{
    setText(matrix_digit == 0 ? "" : QString::number(matrix_digit));

    if (matrix_highlighted) {
        setStyleSheet("QPushButton { border: 2px solid #2f80ed; background: #dbeafe; font-size: 16px; }");
    } else {
        setStyleSheet("QPushButton { border: 1px solid #888; background: white; font-size: 16px; }");
    }
}