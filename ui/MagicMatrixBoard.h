#ifndef MAGICMATRIXBOARD_H
#define MAGICMATRIXBOARD_H

#pragma once

#include "../session/GameSession.h"
#include <QWidget>
#include <QRect>
#include <QSize>
#include <array>

class MagicMatrixBoard : public QWidget //responsible for drawing the big board
{
    Q_OBJECT

public:
    explicit MagicMatrixBoard(QWidget *parent = nullptr);

    void setCellValue(int index, int value); //meaning board -> cell 5 gets value 7
    void setSession(GameSession* session); //attach game session to board
    QSize sizeHint() const override;

signals:
    void cellHighlighted(int index);

protected:
    void paintEvent(QPaintEvent *event) override; //draws everything mannually
    void mousePressEvent(QMouseEvent *event) override; //handles clicks
    void keyPressEvent(QKeyEvent *event) override; //handles keyboard input, fe: pressed 5 -> writes into selected cell

private:
    //static -> shared by all objects, constexpr -> known at compile time
    static constexpr int BoardSize = 9; //meaning 9 rows 9 columns
    static constexpr int CellCount = BoardSize * BoardSize; //calculates 81

    QRect boardRect() const;

    std::array<int, CellCount> matrix_values{}; //stores all 81 values
    //std::array<int, 81> is a modern C++ container, std:: -> belongs to C++ sLibrary
    //the {} initializes every el {0,0,0,0..0}, starts as 0and even 80 starts as 0, without it we get garbage values (memory is loaded)
    int matrix_HighlightedIndex = -1;

    GameSession* m_session = nullptr; //pointer to game logic/session
};

#endif // MAGICMATRIXBOARD_H
