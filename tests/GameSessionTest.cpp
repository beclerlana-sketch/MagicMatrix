#include <QtTest>
#include "../session/GameSession.h"

class GameSessionTest : public QObject
{
    Q_OBJECT

    static BoardSnapShot solution()
    {
        BoardSnapShot board;
        for (int row = 0; row < 9; ++row) {
            for (int column = 0; column < 9; ++column) {
                board.values[row * 9 + column] = (row * 3 + row / 3 + column) % 9 + 1;
            }
        }
        return board;
    }

private slots:
    void mistakesAndRescue()
    {
        GameSession game;
        QVERIFY(game.startLevel({}, solution()));
        game.setCellValue(0, 2);
        QCOMPARE(game.hearts(), 2);
        QVERIFY(game.isIncorrect(0));
        game.clearCell(0);
        game.setCellValue(0, 2);
        QCOMPARE(game.hearts(), 2);
        game.setCellValue(0, 3);
        game.setCellValue(0, 4);
        QCOMPARE(game.state(), GameSession::State::Rescue);
        game.setCellValue(1, 2);
        QCOMPARE(game.boardsnapshot().values[1], 0);

        // Recover the answer from the public equation, just as a player would.
        const auto parts = game.rescueEquation().split(' ');
        const int answer = (parts[6].toInt() - parts[0].toInt()) / parts[4].toInt();
        game.submitRescueAnswer(answer);
        QCOMPARE(game.hearts(), 1);
        QCOMPARE(game.state(), GameSession::State::Playing);
        game.setCellValue(0, 5);
        QCOMPARE(game.state(), GameSession::State::Lost);
    }

    void hintsAndClues()
    {
        GameSession game;
        auto puzzle = solution();
        puzzle.values[0] = 0;
        QVERIFY(game.startLevel(puzzle, solution()));
        game.clearCell(1);
        QCOMPARE(game.boardsnapshot().values[1], 2);
        QSignalSpy hints(&game, &GameSession::hintReady);
        game.requestHint(0);
        QCOMPARE(hints.count(), 1);
        QCOMPARE(qvariant_cast<QList<int>>(hints[0][1]), QList<int>{1});
        QCOMPARE(game.boardsnapshot().values[0], 0);
        game.requestHint(0);
        game.requestHint(0);
        QCOMPARE(hints.count(), 2);
        game.setCellValue(0, 1);
        QCOMPARE(game.state(), GameSession::State::Won);
    }

    void invalidInputAndReset()
    {
        GameSession game;
        QVERIFY(game.startLevel({}, solution()));
        for (int index : {-1, 81}) game.setCellValue(index, 1);
        for (int digit : {0, -1, 10}) game.setCellValue(0, digit);
        QCOMPARE(game.hearts(), 3);
        game.setCellValue(0, 2);
        QVERIFY(!game.startLevel({}, {}));
        QCOMPARE(game.hearts(), 2);
        QVERIFY(game.startLevel({}, solution()));
        QCOMPARE(game.hearts(), 3);
        QCOMPARE(game.hintsRemaining(), 2);
        game.setCellValue(0, 2);
        QCOMPARE(game.hearts(), 2);
    }

    void rescueFailureAndTimeout()
    {
        GameSession game;
        QVERIFY(game.startLevel({}, solution()));
        for (int value : {2, 3, 4}) game.setCellValue(0, value);
        game.submitRescueAnswer(-1);
        QCOMPARE(game.state(), GameSession::State::Lost);
        QVERIFY(game.startLevel({}, solution()));
        for (int value : {2, 3, 4}) game.setCellValue(0, value);
        QTRY_VERIFY_WITH_TIMEOUT(game.state() == GameSession::State::Lost, 32000);
    }
};

QTEST_GUILESS_MAIN(GameSessionTest)
#include "GameSessionTest.moc"
