#include "GameSession.h"
#include "../core/MagicSquareRules.h"
#include <QRandomGenerator>

GameSession::GameSession(QObject *parent) : QObject(parent)
{
    m_timer.setInterval(100);
    connect(&m_timer, &QTimer::timeout, this, &GameSession::updateRescueClock);
}

BoardSnapShot GameSession::boardsnapshot() const
{
    return m_board.snapshot();
}

bool GameSession::startLevel(const BoardSnapShot &puzzle, const BoardSnapShot &solution)
{
    // Validate everything before replacing the current level.
    if (!MagicSquareRules().isSolved(solution)) {
        emit invalidMove("Invalid solution");
        return false;
    }

    for (int i = 0; i < 81; ++i) {
        if (puzzle.values[i] != 0 && puzzle.values[i] != solution.values[i]) {
            emit invalidMove("Clues must match the solution");
            return false;
        }
    }

    m_timer.stop();
    m_elapsed.invalidate();
    m_board = MagicSquareBoard();
    m_solution = solution;
    m_penalized = {};
    m_hearts = 3;
    m_hints = 2;
    m_rescueUsed = false;
    m_equation.clear();

    for (int i = 0; i < 81; ++i) {
        m_fixed[i] = puzzle.values[i] != 0;
        if (m_fixed[i]) {
            m_board.setValue(i, puzzle.values[i]);
        }
    }

    changeState(puzzle.values == solution.values ? State::Won : State::Playing);
    emit heartsChanged(m_hearts);
    emit hintsChanged(m_hints);
    emit boardChanged(m_board.snapshot());

    for (int i = 0; i < 81; ++i) {
        emit cellErrorChanged(i, false);
    }
    return true;
}

bool GameSession::isFixed(int index) const
{
    return index >= 0 && index < 81 && m_fixed[index];
}

bool GameSession::isIncorrect(int index) const
{
    if (index < 0 || index >= 81 || m_state == State::NotStarted) {
        return false;
    }
    const int value = m_board.valueAt(index);
    return value != 0 && value != m_solution.values[index];
}

bool GameSession::editable(int index)
{
    if (m_state != State::Playing || index < 0 || index >= 81 || isFixed(index)) {
        emit invalidMove("Cell is not editable");
        return false;
    }
    return true;
}

void GameSession::changeState(State state)
{
    m_state = state;
    emit stateChanged(state);
}

void GameSession::setCellValue(int index, int value)
{
    if (!editable(index)) {
        return;
    }
    if (!m_board.setValue(index, value)) {
        emit invalidMove("Enter a digit from 1 to 9");
        return;
    }

    const bool wrong = isIncorrect(index);
    if (wrong && !m_penalized[index][value]) {
        m_penalized[index][value] = true;
        --m_hearts;
        emit heartsChanged(m_hearts);
    }

    emit boardChanged(m_board.snapshot());
    emit cellErrorChanged(index, wrong);

    if (m_hearts == 0) {
        if (m_rescueUsed) {
            changeState(State::Lost);
        } else {
            beginRescue();
        }
    } else if (m_board.snapshot().values == m_solution.values) {
        changeState(State::Won);
    }
}

void GameSession::clearCell(int index)
{
    if (!editable(index)) {
        return;
    }
    m_board.clearValue(index);
    emit boardChanged(m_board.snapshot());
    emit cellErrorChanged(index, false);
}

void GameSession::requestHint(int index)
{
    if (!editable(index)) {
        return;
    }
    if (m_hints == 0 || m_board.valueAt(index) != 0) {
        emit invalidMove("Hints require an empty cell and an available hint");
        return;
    }

    // Candidates follow the visible row, column and box, not the answer key.
    QList<int> candidates;
    auto snapshot = m_board.snapshot();
    const MagicSquareRules rules;
    for (int digit = 1; digit <= 9; ++digit) {
        snapshot.values[index] = digit;
        if (!rules.rowHasDuplicate(snapshot, index / 9)
            && !rules.columnHasDuplicate(snapshot, index % 9)
            && !rules.blockHasDuplicate(snapshot, index / 27, (index % 9) / 3)) {
            candidates.append(digit);
        }
    }

    --m_hints;
    emit hintsChanged(m_hints);
    emit hintReady(index, candidates);
}

void GameSession::beginRescue()
{
    m_rescueUsed = true;
    const int a = QRandomGenerator::global()->bounded(1, 10);
    const int b = QRandomGenerator::global()->bounded(2, 10);
    m_answer = QRandomGenerator::global()->bounded(1, 10);
    m_equation = QString("%1 + ? * %2 = %3").arg(a).arg(b).arg(a + m_answer * b);

    m_elapsed.start();
    changeState(State::Rescue);
    m_timer.start();
    emit rescueStarted(m_equation, 30);
    emit rescueTimeChanged(30);
}

int GameSession::rescueSecondsRemaining() const
{
    if (m_state != State::Rescue || !m_elapsed.isValid()) {
        return 0;
    }
    return int(qMax(qint64(0), (30000 - m_elapsed.elapsed() + 999) / 1000));
}

void GameSession::updateRescueClock()
{
    if (m_state != State::Rescue) {
        return;
    }
    emit rescueTimeChanged(rescueSecondsRemaining());
    if (m_elapsed.elapsed() >= 30000) {
        m_timer.stop();
        changeState(State::Lost);
    }
}

void GameSession::submitRescueAnswer(int answer)
{
    if (m_state != State::Rescue) {
        return;
    }
    m_timer.stop();

    // Check elapsed time directly: a delayed timer must not extend the deadline.
    if (m_elapsed.elapsed() >= 30000 || answer != m_answer) {
        changeState(State::Lost);
        return;
    }
    m_hearts = 1;
    emit heartsChanged(m_hearts);
    changeState(State::Playing);
}
