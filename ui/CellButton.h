#ifndef CELLBUTTON_H
#define CELLBUTTON_H

#pragma once

#include <QPushButton>

class CellButton : public QPushButton //represents an individual cell that can act like a button
{
    Q_OBJECT

public:
    explicit CellButton(int cellIndex, QWidget *parent = nullptr); //creates a button
//use explicit to prevent accidental conversions fe: CellButton button(5) OK, button = 5; ERROR (illegal)
    int cellIndex() const;
    //const meants read only function, cuz the function promised not to change the object
    int digit() const;

    void setDigit(int digit); //changes the value
    void setHighlighted(bool highlighted);

signals:
    void cellClicked(int cellIndex); //when user clicks, signal knows by which idx was clicked

private:
    int matrix_cellIndex;
    int matrix_digit = 0;
    bool matrix_highlighted = false;

    void updateAppearance();
};

#endif // CELLBUTTON_H
