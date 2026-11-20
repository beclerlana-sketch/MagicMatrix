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

    // Creates the session and connects the rescue timer.
    explicit GameSession(QObject *parent = nullptr);
    // Returns a copy of the current session board.
    BoardSnapShot boardsnapshot() const;
    // Validates and loads a level, then resets hearts, hints, and rescue state.
    bool startLevel(const BoardSnapShot &puzzle, const BoardSnapShot &solution);

    // Returns the current gameplay state.
    State state() const { return m_state; }
    // Returns the number of remaining hearts.
    int hearts() const { return m_hearts; }
    // Returns the number of unused hints.
    int hintsRemaining() const { return m_hints; }
    // Reports whether a valid cell is a protected starting clue.
    bool isFixed(int index) const;
    // Reports whether a nonempty cell differs from the answer key.
    bool isIncorrect(int index) const;
    // Returns the current missing-number equation.
    QString rescueEquation() const { return m_equation; }
    // Returns the rescue time left, rounded up to whole seconds.
    int rescueSecondsRemaining() const;

public slots:
    // Updates a cell; the session checks gameplay rules and the widget only redraws it.
    void setCellValue(int index, int value);
    // Clears an editable cell and removes its error marker.
    void clearCell(int index);
    // Consumes a hint to return legal candidates for an empty editable cell.
    void requestHint(int index);
    // Checks the answer and deadline, then restores a heart or ends the level.
    void submitRescueAnswer(int answer);

signals:
    // Notifies the UI that the board values have changed.
    void boardChanged(const BoardSnapShot &snapshot);
    // Provides feedback when an action cannot be accepted.
    void invalidMove(const QString &message);
    // Notifies the UI that the remaining heart count changed.
    void heartsChanged(int hearts);
    // Notifies the UI that the remaining hint count changed.
    void hintsChanged(int remaining);
    // Tells the UI whether a cell should show an error marker.
    void cellErrorChanged(int index, bool incorrect);
    // Provides the candidate digits for the requested cell.
    void hintReady(int index, const QList<int> &candidates);
    // Notifies listeners of a gameplay state transition.
    void stateChanged(GameSession::State state);
    // Provides the equation and initial time for the rescue screen.
    void rescueStarted(const QString &equation, int seconds);
    // Provides the remaining whole seconds for the countdown display.
    void rescueTimeChanged(int seconds);

private:
    // Checks whether the current state and cell allow editing.
    bool editable(int index);
    // Stores the new gameplay state and notifies listeners.
    void changeState(State state);
    // Creates the one-time arithmetic challenge and starts its deadline.
    void beginRescue();
    // Updates the countdown and ends the level when time expires.
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
