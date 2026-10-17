#include "CellButton.h"
#include <QSizePolicy>

CellButton::CellButton(int cellIndex, QWidget *parent) //in the process yet
    : QPushButton(parent), matrix_cellIndex(cellIndex)
{
    //minimum size of cell
    setMinimumSize(48, 48);
    //allow button to grow when window grows
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //when QPushButton is clicked, emit our custom signal with cell index
    connect(this, &QPushButton::clicked, this, [this]() {
        emit cellClicked(matrix_cellIndex);
    });

    //draws initial appearance
    updateAppearance();
}

void CellButton::setDigit(int digit)
{
    matrix_digit = digit; //store new member
    updateAppearance(); //redraw button
}

void CellButton::setHighlighted(bool highlighted)
{
    matrix_highlighted = highlighted; //remember highlighted state
    updateAppearance(); //refresh
}

void CellButton::updateAppearance()
{
    setText(matrix_digit == 0 ? "" : QString::number(matrix_digit));

    QString borders = property("style").toString(); //future feature, unused yet

    QString baseStyle = //created stylesheet
        "font-size: 16px;"
        "color: #111827;"
        "text-align: center;"
        + borders;

    if (matrix_highlighted) { //result: blue back when clicked, bold text,a nd thick border
        setStyleSheet(
            "QPushButton {"
            "background-color: #dbeafe;"
            "font-weight: bold;"
            "border-color: #2563eb;"   // only color
            "border-width: 2px;"       // only thickness
            + baseStyle +
            "}"
            );
    } else { //normal state
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



