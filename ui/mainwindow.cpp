#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "MagicMatrixBoard.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setMinimumSize(500, 500);

    auto *boardWidget = new MagicMatrixBoard(this);
    setCentralWidget(boardWidget);

    boardWidget->setCellValue(0, 1);
    boardWidget->setCellValue(10, 55);
    boardWidget->setCellValue(80, 81);


}

MainWindow::~MainWindow()
{
    delete ui;
}
