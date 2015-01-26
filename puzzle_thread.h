#ifndef PUZZLE_THREAD_H
#define PUZZLE_THREAD_H

#include <QThread>

#include "puzzle_solver.h"

class PuzzleThread : public QThread
{
    Q_OBJECT
public:
    explicit PuzzleThread(PuzzleSolver *puzzle_solver, QObject *parent = 0);
    void run();
signals:
    void stopped();
public slots:    
private:
    PuzzleSolver *m_solver;
};

#endif // SOLVER_THREAD_H
