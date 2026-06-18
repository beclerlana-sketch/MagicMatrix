#ifndef CELLBUTTON_H
#define CELLBUTTON_H

#pragma once

#include <QPushButton>

class CellButton : public QPushButton
{
    Q_OBJECT

public:
    explicit CellButton(int cellIndex, QWidget *parent = nullptr);

    int cellIndex() const;
    int digit() const;

    void setDigit(int digit);
    void setHighlighted(bool highlighted);

signals:
    void cellClicked(int cellIndex);

private:
    int matrix_cellIndex;
    int matrix_digit = 0;
    bool matrix_highlighted = false;

    void updateAppearance();
};

#endif // CELLBUTTON_H
