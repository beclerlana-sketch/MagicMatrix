#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GameSession;
class MagicMatrixBoard;
class QLabel;
class QWidget;
class QLineEdit;
class QComboBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void startLevel();
    void enterDigit(int digit);

    GameSession *m_session;
    MagicMatrixBoard *m_board;
    QLabel *m_hearts;
    QLabel *m_hints;
    QLabel *m_message;
    QLabel *m_equation;
    QLabel *m_countdown;
    QWidget *m_controls;
    QWidget *m_rescue;
    QLineEdit *m_answer;
    QComboBox *m_difficulty;
    int m_selected = -1;
};

#endif
