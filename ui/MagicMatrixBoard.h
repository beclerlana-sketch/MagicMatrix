#ifndef MAGICMATRIXBOARD_H
#define MAGICMATRIXBOARD_H

#pragma once

#include <QWidget>
#include <QRect>
#include <QSize>
#include <array>

class MagicMatrixBoard : public QWidget
{
    Q_OBJECT

public:
    explicit MagicMatrixBoard(QWidget *parent = nullptr);

    void setCellValue(int index, int value);
    QSize sizeHint() const override;

signals:
    void cellHighlighted(int index);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    static constexpr int BoardSize = 9;
    static constexpr int CellCount = BoardSize * BoardSize;

    QRect boardRect() const;

    std::array<int, CellCount> matrix_values{};
    int matrix_HighlightedIndex = -1;
};

#endif // MAGICMATRIXBOARD_H
