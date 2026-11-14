#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "MagicMatrixBoard.h"
#include "../core/MagicSquareBoard.h"
#include "../session/GameSession.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); //creates the window

    setMinimumSize(500, 500);

    auto *boardWidget = new MagicMatrixBoard(this); //creates the board
    setCentralWidget(boardWidget); //the board sets in the middle of mainwindow
    auto *session = new GameSession(this); //creates the logic manager

    connect(session, &GameSession::boardChanged, this, //whenever GameSession changed values, board updates auto
            [boardWidget](const BoardSnapShot &snapshot) {
                for (int index = 0; index < MagicSquareBoard::CellCount; ++index) {
                    boardWidget->setCellValue(index, snapshot.values[index]);
                }
            });

    //these are just test values for a moment
    session->setCellValue(0, 1);
    session->setCellValue(10, 55);
    session->setCellValue(80, 81);




}

MainWindow::~MainWindow()
{
    delete ui;
}
