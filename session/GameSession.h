#ifndef GAMESESSION_H
#define GAMESESSION_H
#include <QObject>
#include <QString>
#include "../core/MagicSquareBoard.h"
class GameSession: public QObject // the object will emit signals and have it slots

{
    Q_OBJECT
public:
    explicit GameSession (QObject *parent = nullptr);// the created gamesession will be deleted automatically while preventing accidental conversions
    BoardSnapShot boardsnapshot() const; // provides the copy of the current board
public slots :

      void setCellValue(int index , int value);
        void clearCell(int index);
  signals:
        void boardChanged(const BoardSnapShot &snapshot); // the board will be redrawn by the signal sent to the ui

      void invalidMove(const QString &message);// this will telll th ui and invalid move was made
    private:
      MagicSquareBoard m_board;

};

#endif // GAMESESSION_H
