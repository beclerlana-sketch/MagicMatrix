#include <QtTest>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include "../ui/mainwindow.h"
#include "../ui/MagicMatrixBoard.h"
#include "../session/GameSession.h"

class PlayableUiTest : public QObject
{
    Q_OBJECT

private slots:
    void keyboardClearing()
    {
        MainWindow window;
        auto *game = window.findChild<GameSession *>();
        auto *board = window.findChild<MagicMatrixBoard *>();

        // Select the third cell using the painted grid's actual mouse path.
        const int cellSize = (540 - 32) / 9;
        QTest::mouseClick(board, Qt::LeftButton, Qt::NoModifier,
            QPoint(18 + cellSize * 2 + cellSize / 2, 18 + cellSize / 2));
        QTest::keyClick(board, Qt::Key_4);
        QCOMPARE(game->boardsnapshot().values[2], 4);
        QTest::keyClick(board, Qt::Key_Backspace);
        QCOMPARE(game->boardsnapshot().values[2], 0);
        QCOMPARE(game->hearts(), 3);
    }

    void difficultySelection()
    {
        MainWindow window;
        auto *selector = window.findChild<QComboBox *>();
        auto *game = window.findChild<GameSession *>();
        QVERIFY(selector);
        QCOMPARE(selector->count(), 3);

        const auto medium = game->boardsnapshot().values;
        selector->setCurrentIndex(1);
        QCOMPARE(game->state(), GameSession::State::Playing);
        QVERIFY(game->boardsnapshot().values != medium);
        QCOMPARE(game->hearts(), 3);
        selector->setCurrentIndex(2);
        QCOMPARE(game->state(), GameSession::State::Playing);
        QCOMPARE(game->boardsnapshot().values[0], 1);
        QCOMPARE(game->hintsRemaining(), 2);
        QCOMPARE(window.windowTitle(), QString("ANASLO - Expert"));
    }

    void playableFlow()
    {
        MainWindow window;
        auto *game = window.findChild<GameSession *>();
        auto *board = window.findChild<MagicMatrixBoard *>();
        QVERIFY(game);
        QVERIFY(board);
        QCOMPARE(game->state(), GameSession::State::Playing);
        QCOMPARE(game->boardsnapshot().values[0], 5);

        // Cell 2 is empty and its correct digit is 4.
        board->cellHighlighted(2);
        auto clickButton = [&window](const QString &text) {
            for (auto *button : window.findChildren<QPushButton *>()) {
                if (button->text() == text) {
                    button->click();
                    return true;
                }
            }
            return false;
        };
        QVERIFY(clickButton("1"));
        QCOMPARE(game->hearts(), 2);
        QVERIFY(game->isIncorrect(2));
        QVERIFY(clickButton("Clear"));
        QVERIFY(clickButton("Hint"));
        QCOMPARE(game->hintsRemaining(), 1);
        QVERIFY(clickButton("2"));
        QVERIFY(clickButton("3"));
        QCOMPARE(game->state(), GameSession::State::Rescue);
        QVERIFY(!window.findChild<QLineEdit *>()->isHidden());
        QVERIFY(clickButton("Restart level"));
        QCOMPARE(game->hearts(), 3);
        QCOMPARE(game->hintsRemaining(), 2);
        QCOMPARE(game->boardsnapshot().values[2], 0);
    }
};

QTEST_MAIN(PlayableUiTest)
#include "PlayableUiTest.moc"

