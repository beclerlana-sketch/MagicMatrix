#include "MagicMatrixBoard.h"

#include <QMouseEvent>
#include <QPainter>
#include <QSizePolicy>

MagicMatrixBoard::MagicMatrixBoard(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(540, 540);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

QSize MagicMatrixBoard::sizeHint() const
{
    return QSize(600, 600);
}

void MagicMatrixBoard::setCellValue(int index, int value)
{
    if (index < 0 || index > CellCount)
        return;

    matrix_values[index] = value;
    update();
}

QRect MagicMatrixBoard::boardRect() const
{
    constexpr int margin = 16;

    int side = qMin(width(), height()) - margin * 2;
    side = qMax(0, side);
    side = (side / BoardSize) * BoardSize;

    const int x = (width() - side) / 2;
    const int y = (height() - side) / 2;

    return QRect(x, y, side, side);
}

void MagicMatrixBoard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    const QRect rect = boardRect();
    const int cellSize = rect.width() / BoardSize;

    painter.fillRect(rect, QColor("#fbfbfb"));

    if(matrix_HighlightedIndex > 0) {
        const int row = matrix_HighlightedIndex / BoardSize;
        const int column = matrix_HighlightedIndex %  BoardSize;

        QRect selectedRect(
            rect.left() + column * cellSize,
            rect.top() + row * cellSize,
            cellSize,
            cellSize
        );

        painter.fillRect(selectedRect, QColor("#dbeafe"));
    }

    QFont font = painter.font();
    font.setPointSize(qMax(18, cellSize / 3));
    painter.setFont(font);
    painter.setPen(QColor("#111827"));

    for(int index = 0; index < CellCount; ++index){
        if(matrix_values[index] == 0)
            continue;

        const int row = index / BoardSize;
        const int column = index % BoardSize;

        QRect cellRect(
            rect.left() + column * cellSize,
            rect.top() + row * cellSize,
            cellSize,
            cellSize
        );

        painter.drawText(cellRect, Qt::AlignCenter, QString::number(matrix_values[index]));
    }

    for(int line = 0; line <= BoardSize; ++line){
        const bool thickLine = line % 3 == 0;

        QPen pen(thickLine ? QColor("#000000") : QColor("#a8a8a8"));
        pen.setWidth(thickLine ? 3 : 1);
        painter.setPen(pen);

        const int position = line * cellSize;

        painter.drawLine(
            rect.left() + position,
            rect.top(),
            rect.left() + position,
            rect.bottom()
        );

        painter.drawLine(
            rect.left(),
            rect.top() + position,
            rect.right(),
            rect.top() + position
        );
    }
}

void MagicMatrixBoard::mousePressEvent(QMouseEvent *event)
{
    const QRect rect = boardRect();
    const int  cellSize = rect.width() / BoardSize;

    if(!rect.contains(event->pos()) || cellSize <= 0)
        return;

    const int column = (event->pos().x() - rect.left()) / cellSize;
    const int row = (event->pos().y() - rect.top()) / cellSize;

    if(row < 0 || row >= BoardSize || column < 0 || column >= BoardSize)
        return;

    matrix_HighlightedIndex = row * BoardSize + column;
    emit cellHighlighted(matrix_HighlightedIndex);
    update();
}

