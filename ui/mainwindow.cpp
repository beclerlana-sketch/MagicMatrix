#include "mainwindow.h"
#include "MagicMatrixBoard.h"
#include "../session/GameSession.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QIntValidator>
#include <QShortcut>
#include <QGridLayout>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("ANASLO - Level 1");
    auto *page = new QWidget(this);
    auto *layout = new QVBoxLayout(page);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(16);
    setCentralWidget(page);

    m_session = new GameSession(this);
    m_board = new MagicMatrixBoard(page);
    m_board->setSession(m_session);
    m_hearts = new QLabel(page);
    m_hints = new QLabel(page);
    m_message = new QLabel("Select an empty cell, then enter a digit.", page);
    m_message->setWordWrap(true);
    m_message->setFixedHeight(64);
    m_message->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    setStyleSheet(
        "QMainWindow { background: #f3f6fa; }"
        "QLabel { color: #24344b; font-size: 14px; }"
        "QPushButton { background: white; color: #24558c; border: 1px solid #cbd5e1;"
        " border-radius: 6px; padding: 8px; font-size: 16px; }"
        "QPushButton:hover { background: #e8f0fc; }"
        "QPushButton:disabled { color: #94a3b8; }"
        "QLineEdit, QComboBox { padding: 7px; background: white; color: #24344b; }"
    );

    auto *header = new QHBoxLayout;
    auto *title = new QLabel("ANASLO", page);
    title->setStyleSheet("font-size: 24px; font-weight: bold;");
    header->addWidget(title);
    header->addStretch();
    m_difficulty = new QComboBox(page);
    m_difficulty->addItems({"Medium", "Hard", "Expert"});
    header->addWidget(m_difficulty);
    header->addWidget(m_hearts);
    header->addStretch();
    header->addWidget(m_hints);
    auto *restart = new QPushButton("Restart level", page);
    header->addWidget(restart);
    layout->addLayout(header);
    // Fixed square board and a consistent side panel prevent layout jumps.
    auto *body = new QHBoxLayout;
    body->setSpacing(24);
    m_board->setFixedSize(540, 540);
    body->addWidget(m_board, 0, Qt::AlignTop);
    auto *panel = new QWidget(page);
    panel->setFixedWidth(264);
    auto *panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(0, 0, 0, 0);
    panelLayout->setSpacing(16);
    panelLayout->addWidget(m_message);
    body->addWidget(panel, 0, Qt::AlignTop);
    layout->addLayout(body);
    layout->addStretch();

    // Buttons and keyboard shortcuts share the same session entry point.
    m_controls = new QWidget(page);
    auto *digits = new QGridLayout(m_controls);
    digits->setContentsMargins(0, 0, 0, 0);
    digits->setSpacing(8);
    for (int digit = 1; digit <= 9; ++digit) {
        auto *button = new QPushButton(QString::number(digit), m_controls);
        button->setFixedHeight(48);
        digits->addWidget(button, (digit - 1) / 3, (digit - 1) % 3);
        connect(button, &QPushButton::clicked, this, [this, digit]() { enterDigit(digit); });
        auto *shortcut = new QShortcut(QKeySequence(QString::number(digit)), m_controls);
        connect(shortcut, &QShortcut::activated, this, [this, digit]() { enterDigit(digit); });
    }
    auto *clear = new QPushButton("Clear", m_controls);
    auto *hint = new QPushButton("Hint", m_controls);
    digits->addWidget(clear, 3, 0, 1, 2);
    digits->addWidget(hint, 3, 2);
    panelLayout->addWidget(m_controls);
    connect(clear, &QPushButton::clicked, this, [this]() { m_session->clearCell(m_selected); });
    connect(hint, &QPushButton::clicked, this, [this]() { m_session->requestHint(m_selected); });

    // The rescue stays in the window so closing a dialog cannot bypass it.
    m_rescue = new QWidget(page);
    // Hidden rescue controls retain their space, keeping the grid stationary.
    auto rescuePolicy = m_rescue->sizePolicy();
    rescuePolicy.setRetainSizeWhenHidden(true);
    m_rescue->setSizePolicy(rescuePolicy);
    auto *rescueLayout = new QVBoxLayout(m_rescue);
    rescueLayout->setContentsMargins(0, 0, 0, 0);
    m_equation = new QLabel(m_rescue);
    m_countdown = new QLabel(m_rescue);
    m_answer = new QLineEdit(m_rescue);
    m_answer->setPlaceholderText("Missing number");
    m_answer->setValidator(new QIntValidator(0, 999, m_answer));
    auto *submit = new QPushButton("Submit answer", m_rescue);
    rescueLayout->addWidget(m_equation);
    rescueLayout->addWidget(m_countdown);
    rescueLayout->addWidget(m_answer);
    rescueLayout->addWidget(submit);
    panelLayout->addWidget(m_rescue);
    panelLayout->addStretch();
    m_rescue->hide();

    auto submitAnswer = [this]() {
        if (!m_answer->hasAcceptableInput()) {
            m_message->setText("Enter a number before submitting.");
            return;
        }
        m_session->submitRescueAnswer(m_answer->text().toInt());
    };
    connect(submit, &QPushButton::clicked, this, submitAnswer);
    connect(m_answer, &QLineEdit::returnPressed, this, submitAnswer);
    connect(restart, &QPushButton::clicked, this, &MainWindow::startLevel);

    connect(m_board, &MagicMatrixBoard::cellHighlighted, this, [this](int index) {
        m_selected = index;
        m_message->setText(m_session->isFixed(index)
            ? "This is a fixed starting clue. Select an empty cell."
            : "Enter a digit, clear this cell, or request a hint.");
    });
    connect(m_session, &GameSession::boardChanged, this, [this](const BoardSnapShot &board) {
        for (int index = 0; index < 81; ++index) {
            m_board->setCellValue(index, board.values[index]);
            m_board->setCellState(index, m_session->isFixed(index), m_session->isIncorrect(index));
        }
    });
    connect(m_session, &GameSession::heartsChanged, this, [this](int count) {
        m_hearts->setText(QString("Lives: %1%2").arg(QString(count, QChar(0x2665)), QString(3 - count, QChar(0x2661))));
        m_hearts->setStyleSheet("color: #dc2626; font-size: 22px;");
    });
    connect(m_session, &GameSession::hintsChanged, this, [this](int count) {
        m_hints->setText(QString("Hints left: %1").arg(count));
    });
    connect(m_session, &GameSession::invalidMove, m_message, &QLabel::setText);
    connect(m_session, &GameSession::cellErrorChanged, this, [this](int index, bool wrong) {
        m_board->setCellState(index, m_session->isFixed(index), wrong);
        if (wrong) m_message->setText("Incorrect digit. This cell stays red until corrected or cleared.");
    });
    connect(m_session, &GameSession::hintReady, this, [this](int, const QList<int> &values) {
        QStringList candidates;
        for (int value : values) candidates.append(QString::number(value));
        m_message->setText(candidates.isEmpty() ? "No candidates: check nearby mistakes."
            : "Possible digits: " + candidates.join(", "));
    });
    connect(m_session, &GameSession::rescueStarted, this, [this](const QString &equation, int) {
        m_equation->setText(equation);
        m_answer->clear();
        m_answer->setFocus();
    });
    connect(m_session, &GameSession::rescueTimeChanged, this, [this](int seconds) {
        m_countdown->setText(QString("%1 seconds").arg(seconds));
    });
    connect(m_session, &GameSession::stateChanged, this, [this](GameSession::State state) {
        m_controls->setEnabled(state == GameSession::State::Playing);
        m_board->setEnabled(state == GameSession::State::Playing);
        m_rescue->setVisible(state == GameSession::State::Rescue);
        if (state == GameSession::State::Rescue) m_message->setText("Last chance! Solve the equation. Multiply before adding.");
        if (state == GameSession::State::Lost) m_message->setText("Level failed. Press Restart level to try again.");
        if (state == GameSession::State::Won) m_message->setText("Level complete! Well done.");
        if (state == GameSession::State::Playing) m_message->setText("Select a cell and enter a digit.");
    });

    connect(m_difficulty, &QComboBox::currentIndexChanged, this, [this](int) { startLevel(); });

    startLevel();
    setMinimumSize(884, 670);
    resize(920, 720);
}

void MainWindow::enterDigit(int digit)
{
    m_session->setCellValue(m_selected, digit);
}

void MainWindow::startLevel()
{
    // Each starter puzzle has one verified solution. Difficulty labels are provisional.
    const QString puzzles[] = {
        "530070000600195000098000060800060003400803001700020006060000280000419005000080079",
        "000000010400000000020000000000050407008000300001090000300400200050100000000806000",
        "100007090030020008009600500005300900010080002600004000300000010040000007007000300"
    };
    const QString answers[] = {
        "534678912672195348198342567859761423426853791713924856961537284287419635345286179",
        "693784512487512936125963874932651487568247391741398625319475268856129743274836159",
        "162857493534129678789643521475312986913586742628794135356478219241935867897261354"
    };
    const int difficulty = m_difficulty->currentIndex();
    const QString clues = puzzles[difficulty];
    const QString answer = answers[difficulty];
    setWindowTitle("ANASLO - " + m_difficulty->currentText());
    BoardSnapShot puzzle;
    BoardSnapShot solution;
    for (int index = 0; index < 81; ++index) {
        puzzle.values[index] = clues[index].digitValue();
        solution.values[index] = answer[index].digitValue();
    }
    m_selected = -1;
    m_board->resetSelection();
    m_session->startLevel(puzzle, solution);
}
