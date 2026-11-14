#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MagicMatrixBoard.h"
#include "../core/MagicSquareBoard.h"
#include "../session/GameSession.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); //creates the window

    setMinimumSize(500, 500);

    auto *boardWidget = new MagicMatrixBoard(this); //creates the board
    setCentralWidget(boardWidget); //the board sets in the middle of mainwindow
    auto *session = new GameSession(this); //creates the logic manager
    boardWidget->setSession(session);

    connect(session, &GameSession::boardChanged, this, //whenever GameSession changed values, board updates auto
            [boardWidget](const BoardSnapShot &snapshot) {
                for (int index = 0; index < MagicSquareBoard::CellCount; ++index) {
                    boardWidget->setCellValue(index, snapshot.values[index]);
                }
            });

    // //TEMPOPRARY TESTED, basically it just pop up warning for duplicate and etc. but it has bug inside anyway
    // connect(session, &GameSession::invalidMove, this, [this](const QString &message) {
    //     QMessageBox::warning(this, "MagicMatrix: Error", message);
    // });

}

MainWindow::~MainWindow()
{
    delete ui;
}
