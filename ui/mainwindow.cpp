#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "MagicMatrixBoard.h"
#include "../core/MagicSquareBoard.h"
#include "../session/GameSession.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setMinimumSize(500, 500);

    auto *boardWidget = new MagicMatrixBoard(this);
    setCentralWidget(boardWidget);
    auto *session = new GameSession(this);

    connect(session, &GameSession::boardChanged, this,
            [boardWidget](const BoardSnapShot &snapshot) {
                for (int index = 0; index < MagicSquareBoard::CellCount; ++index) {
                    boardWidget->setCellValue(index, snapshot.values[index]);
                }
            });

    session->setCellValue(0, 1);
    session->setCellValue(10, 55);
    session->setCellValue(80, 81);




}

MainWindow::~MainWindow()
{
    delete ui;
}
