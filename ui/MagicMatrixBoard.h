#ifndef MAGICMATRIXBOARD_H
#define MAGICMATRIXBOARD_H

#pragma once

#include <QWidget>
#include <QRect>
#include <QSize>
#include <array>

class GameSession;

class MagicMatrixBoard : public QWidget
{
    Q_OBJECT

public:
    explicit MagicMatrixBoard(QWidget *parent = nullptr);

    void setCellValue(int index, int value);
    void setCellState(int index, bool fixed, bool incorrect);
    void resetSelection();
    void setSession(GameSession *session);
    QSize sizeHint() const override;

signals:
    void cellHighlighted(int index);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    static constexpr int BoardSize = 9;
    static constexpr int CellCount = BoardSize * BoardSize;

    QRect boardRect() const;

    std::array<int, CellCount> matrix_values{};
    std::array<bool, CellCount> matrix_fixed{};
    std::array<bool, CellCount> matrix_incorrect{};
    int matrix_HighlightedIndex = -1;
    GameSession *m_session = nullptr;
};

#endif // MAGICMATRIXBOARD_H
