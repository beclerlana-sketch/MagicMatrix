#include "MagicMatrixBoard.h"
#include <QMouseEvent>
#include <QPainter>
#include <QSizePolicy>

MagicMatrixBoard::MagicMatrixBoard(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(540, 540);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); //grow if window grows
}

QSize MagicMatrixBoard::sizeHint() const
{
    return QSize(600, 600); //board can be big till this size
}

void MagicMatrixBoard::setCellValue(int index, int value)
{
    if (index < 0 || index >= CellCount)
        return;

    matrix_values[index] = value; //stores number
    update(); //paint event is called, by updating the value
}

QRect MagicMatrixBoard::boardRect() const //caculates board position
{ //and finds where the board should be drawn
    constexpr int margin = 16; //leave 16 pixels around board

    int side = qMin(width(), height()) - margin * 2; //fe: window 800x700, 700-32=668
    side = qMax(0, side);//protection, if somebody makes it tiny //(qMax-qMin?) Qt provides its own max & min, qMin auto compares and gets small value same rule goes to qMax
    side = (side / BoardSize) * BoardSize; //fe: 668-> 668 / 9 = 74 * 9 = 666

    const int x = (width() - side) / 2; //67
    const int y = (height() - side) / 2; //17

    return QRect(x, y, side, side);
}

void MagicMatrixBoard::paintEvent(QPaintEvent *event) //is called whenever board must be redrawn
{
    Q_UNUSED(event); //avoid compiler warning, think of void one, works behjind the scene, without it wont run

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false); //starts drawig the event board

    const QRect rect = boardRect(); //calls the above fucntion (67,17,666,666)
    const int cellSize = rect.width() / BoardSize; //each square 74x74

    painter.fillRect(rect, QColor("#fbfbfb")); //draw background -> ligth gray board area, before num and lines

    if(matrix_HighlightedIndex >= 0) { //suppose user clicked r1 and c1, index is 10, cuz 1*9+1=10
        const int row = matrix_HighlightedIndex / BoardSize; //10/9=r1
        const int column = matrix_HighlightedIndex %  BoardSize; //10%9=c1

        QRect selectedRect( //creates rectangle
            rect.left() + column * cellSize, //67+1*74
            rect.top() + row * cellSize, //17+1*74
            cellSize, //74
            cellSize //74
        ); //QRect -> 141,91,74,74

        painter.fillRect(selectedRect, QColor("#dbeafe")); //blue highlight, when the exactly square selected
    }

    QFont font = painter.font(); //gets current font from painter
    font.setPointSize(qMax(18, cellSize / 3)); //cellSize is 74, so 74/3=24 -> compare 18, 24
    painter.setFont(font); //we apply 24
    painter.setPen(QColor("#111827"));

    for(int index = 0; index < CellCount; ++index) { //this loop runs every square 0, 1, 2 ... 80
        if(matrix_values[index] == 0) //if not 0 continue drawing
            continue;

        const int row = index / BoardSize;
        const int column = index % BoardSize;
        //both of them we get 0

        QRect cellRect(
            rect.left() + column * cellSize,
            rect.top() + row * cellSize,
            cellSize,
            cellSize
        );

        painter.drawText(cellRect, Qt::AlignCenter, QString::number(matrix_values[index])); //draw, shows 1, fe: from session in mainwindow
        //session->setCellValue(10, 55); r:10/9=1, c:10%9=1, draws 55 at r1 and c1, and so on with every called session
    }

    for(int line = 0; line <= BoardSize; ++line) { //grid lines, we draw the board itself 0-9
        const bool thickLine = line % 3 == 0; //always true for 1 and 3, it draws lines like | | |

        QPen pen(thickLine ? QColor("#000000") : QColor("#a8a8a8"));
        pen.setWidth(thickLine ? 3 : 1);
        painter.setPen(pen);

        const int position = line * cellSize; //3*74=222, draws line at pixel 222

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

void MagicMatrixBoard::mousePressEvent(QMouseEvent *event) //this one is called every time the user clicks the board
{
    const QRect rect = boardRect();
    const int  cellSize = rect.width() / BoardSize;

    if (!rect.contains(event->pos()) || cellSize <= 0) //if outside board is clicked, return (nothing happens)
        return;

    //if you cliked gets the row and column
    const int column = (event->pos().x() - rect.left()) / cellSize;
    const int row = (event->pos().y() - rect.top()) / cellSize;

    if (row < 0 || row >= BoardSize || column < 0 || column >= BoardSize)
        return;

    matrix_HighlightedIndex = row * BoardSize + column; //coverts to arrayIndex
    emit cellHighlighted(matrix_HighlightedIndex); //checks the signal, if clicked sets the value at cell pos, every cellhas one pos 0-30
    update(); //calls the above paint function, and updates
}

