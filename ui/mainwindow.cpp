#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "MagicMatrixBoard.h"
#include "../core/MagicSquareBoard.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setMinimumSize(500, 500);

    auto *boardWidget = new MagicMatrixBoard(this);
    setCentralWidget(boardWidget);

    MagicSquareBoard board;
    board.setValue(0, 1);
    board.setValue(10, 55);
    board.setValue(80, 81);

    BoardSnapShot snapshot = board.snapshot();

    for (int index = 0; index < MagicSquareBoard::CellCount; ++index) {
        boardWidget->setCellValue(index, snapshot.values[index]);
    }



}

MainWindow::~MainWindow()
{
    delete ui;
}
