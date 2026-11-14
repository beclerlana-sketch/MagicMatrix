#ifndef GAMESESSION_H
#define GAMESESSION_H

#include <QObject>
#include <QList>
#include <QTimer>
#include <QElapsedTimer>
#include "../core/MagicSquareBoard.h"

class GameSession : public QObject
{
    Q_OBJECT

public:
    enum class State { NotStarted, Playing, Rescue, Won, Lost };
    Q_ENUM(State)

    explicit GameSession(QObject *parent = nullptr);
    BoardSnapShot boardsnapshot() const;
    bool startLevel(const BoardSnapShot &puzzle, const BoardSnapShot &solution);

    State state() const { return m_state; }
    int hearts() const { return m_hearts; }
    int hintsRemaining() const { return m_hints; }
    bool isFixed(int index) const;
    bool isIncorrect(int index) const;
    QString rescueEquation() const { return m_equation; }
    int rescueSecondsRemaining() const;

public slots:
    void setCellValue(int index, int value);
    void clearCell(int index);
    void requestHint(int index);
    void submitRescueAnswer(int answer);

signals:
    void boardChanged(const BoardSnapShot &snapshot);
    void invalidMove(const QString &message);
    void heartsChanged(int hearts);
    void hintsChanged(int remaining);
    void cellErrorChanged(int index, bool incorrect);
    void hintReady(int index, const QList<int> &candidates);
    void stateChanged(GameSession::State state);
    void rescueStarted(const QString &equation, int seconds);
    void rescueTimeChanged(int seconds);

private:
    bool editable(int index);
    void changeState(State state);
    void beginRescue();
    void updateRescueClock();

    MagicSquareBoard m_board;
    BoardSnapShot m_solution;
    std::array<bool, 81> m_fixed{};

    // Each wrong digit is charged only once per cell during a level.
    std::array<std::array<bool, 10>, 81> m_penalized{};
    State m_state = State::NotStarted;
    int m_hearts = 3;
    int m_hints = 2;
    bool m_rescueUsed = false;
    int m_answer = 0;
    QString m_equation;
    QTimer m_timer;
    QElapsedTimer m_elapsed;
};

#endif
