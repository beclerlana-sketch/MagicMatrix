#include "CellButton.h"
#include <QSizePolicy>

CellButton::CellButton(int cellIndex, QWidget *parent)
    : QPushButton(parent), matrix_cellIndex(cellIndex)
{
    setMinimumSize(44, 44);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(this, &QPushButton::clicked, this, [this]() {
        emit cellClicked(matrix_cellIndex);
    });

    updateAppearance();
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
        setStyleSheet(
            "QPushButton {"
            "background-color: #dbeafe;"
            "border: 2px solid #2563eb;"
            "font-size: 16px;"
            "font-weight: bold;"
            "color: #111827;"
            "}"
            );
    } else {
        setStyleSheet(
            "QPushButton {"
            "background-color: #ffffff;"
            "border: 1px solid #9ca3af;"
            "font-size: 16px;"
            "color: #111827;"
            "}"
            );
    }
}