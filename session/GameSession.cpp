#include "GameSession.h"
GameSession::GameSession(QObject *parent):QObject(parent)// creates the game session , passes it parameters to the Qobject and makes it default
{

}
BoardSnapShot GameSession::boardsnapshot() const // the board getter
{
    return m_board.snapshot();
}
void GameSession::setCellValue(int index , int value)
{
    if(!m_board.setValue(index ,value)){ // incase of a wrong player move it will emit the signal
        emit invalidMove("Invalid value");
        return;
    }
    emit boardChanged(m_board.snapshot());
}void GameSession::clearCell(int index)
{
    if(!m_board.clearValue(index)){
        emit invalidMove("Invalid cell");
        return;
    }
    emit boardChanged(m_board.snapshot()); // same as the cellvalue
}