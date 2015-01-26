#include "puzzle_thread.h"



PuzzleThread::PuzzleThread(PuzzleSolver *puzzle_solver, QObject *parent) :
    QThread(parent)
{
    m_solver = puzzle_solver;
}

void PuzzleThread::run(){
    while(m_solver->get_status()==RUNNING){
        m_solver->solve();
    }
    emit stopped();
}
