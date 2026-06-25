#include "CellButton.h"
#include <QSizePolicy>

CellButton::CellButton(int cellIndex, QWidget *parent)
    : QPushButton(parent), matrix_cellIndex(cellIndex)
{
    setMinimumSize(48, 48);
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

    QString borders = property("style").toString();

    QString baseStyle =
        "font-size: 16px;"
        "color: #111827;"
        "text-align: center;"
        + borders;

    if (matrix_highlighted) {
        setStyleSheet(
            "QPushButton {"
            "background-color: #dbeafe;"
            "font-weight: bold;"
            "border-color: #2563eb;"   // only color
            "border-width: 2px;"       // only thickness
            + baseStyle +
            "}"
            );
    } else {
        setStyleSheet(
            "QPushButton {"
            "background-color: #ffffff;"
            "border-color: #9ca3af;"   // only color
            "border-width: 1px;"       // only thickness
            + baseStyle +
            "}"
            );
    }
}



