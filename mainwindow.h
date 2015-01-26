#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "puzzle_board.h"
#include "board_widget.h"
#include "puzzle_solver.h"
#include <QTimer>
#include <QTime>
#include <QListWidgetItem>
#include <puzzle_thread.h>

/*This is the main class, it is responsible for controlling the program*/

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:        
    void start();
    void find_solution();
    void stop();
    void scramble();
    void change_interval();
    void solve_puzzle();
    void setup_board();
    void set_log_board(int);
    void reset();
    void thread_stopped();


private:
    void init();    
    int check_status();
    void connect_slots();
    void build_solution_list();    
    std::vector<Node*>* m_solution_nodes;
    Ui::MainWindow *ui;
    PuzzleSolver *m_solver;
    BoardWidget *m_puzzle_widget;
    BoardWidget *m_solution_widget;
    PuzzleBoard *m_board;
    PuzzleBoard *m_goal;
    QTimer *m_timer;
    QTime *m_total_time;
    PuzzleThread *m_thread;
};

#endif // MAINWINDOW_H
