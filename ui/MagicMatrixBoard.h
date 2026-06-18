#ifndef MAGICMATRIXBOARD_H
#define MAGICMATRIXBOARD_H

#pragma once

#include <QWidget>
#include <QVector>

class CellButton;

class MagicMatrixBoard : public QWidget
{
    Q_OBJECT

public:
    explicit MagicMatrixBoard(QWidget *parent = nullptr);

    void setCellValue(int index, int value);
    void setHighlightedCell(int index);

signals:
    void cellHighlighted(int index);

private:
    static constexpr int BoardSize = 9;
    static constexpr int CellCount = BoardSize * BoardSize;

    QVector<CellButton *> matrix_cells;
    int matrix_HighlightedIndex = -1;
};

#endif // MAGICMATRIXBOARD_H
