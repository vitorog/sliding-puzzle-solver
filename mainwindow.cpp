#include "mainwindow.h"
#include "ui_mainwindow.h"

#define WIDTH_SIZE 105
#define HEIGHT_SIZE 115

//Constructor; sets all variables to null
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_board = NULL;
    m_puzzle_widget = NULL;
    m_solution_widget = NULL;
    m_goal = NULL;
    m_solver = NULL;
    m_thread = NULL;

    init();
    connect_slots();
    setup_board();    
}

//Initial settings
void MainWindow::init(){
    int interval = ui->interval_spinBox->value();
    m_timer = new QTimer();
    m_timer->setInterval(interval);
    m_total_time = new QTime();

    ui->num_nodes_label->setVisible(false);
    ui->time_label->setVisible(false);
    ui->result_label->setVisible(false);
    ui->stop_pushButton->setEnabled(false);
}

//Connect the interface slots
void MainWindow::connect_slots(){
    connect(m_timer,SIGNAL(timeout()),this,SLOT(solve_puzzle()));
    connect(ui->interval_spinBox,SIGNAL(valueChanged(int)),this,SLOT(change_interval()));
    connect(ui->solve_pushButton,SIGNAL(clicked()),this,SLOT(start()));
    connect(ui->find_solution_pushButton,SIGNAL(clicked()),this,SLOT(find_solution()));
    connect(ui->stop_pushButton,SIGNAL(clicked()),this,SLOT(stop()));
    connect(ui->scramble_pushButton,SIGNAL(clicked()),this,SLOT(scramble()));
    connect(ui->size_spinBox,SIGNAL(valueChanged(int)),this,SLOT(setup_board()));
    connect(ui->listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(set_log_board(int)));
    connect(ui->reset_pushButton,SIGNAL(clicked()),this,SLOT(reset()));
}

//This will build the puzzle board and the solution board
void MainWindow::setup_board(){

    int width_constant = WIDTH_SIZE;
    int height_constant = HEIGHT_SIZE;
    int size = ui->size_spinBox->value();

    if(m_board != NULL)
        delete m_board;
    if(m_goal != NULL)
        delete m_goal;
    if(m_puzzle_widget != NULL)
        delete m_puzzle_widget;
    if(m_solution_widget != NULL)
        delete m_solution_widget;

    m_board = new PuzzleBoard(size);
    m_goal = new PuzzleBoard(size);
    m_puzzle_widget = new BoardWidget(m_board,false,ui->comboBox->currentIndex(),ui->puzzle_dock_widget);
    m_solution_widget = new BoardWidget(m_goal,true,ui->comboBox->currentIndex(),ui->puzzle_dock_widget);
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),m_puzzle_widget,SLOT(change_image(int)));
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),m_solution_widget,SLOT(change_image(int)));
    connect(ui->lock_checkBox,SIGNAL(clicked(bool)),m_solution_widget,SLOT(lock_board(bool)));

    ui->puzzle_dock_widget->setWidget(m_puzzle_widget);
    ui->solution_dock_widget->setWidget(m_solution_widget);
    ui->puzzle_dock_widget->resize(size*width_constant,size*height_constant);
    ui->puzzle_dock_widget->setMinimumWidth(size*width_constant);
    ui->puzzle_dock_widget->setMinimumHeight(size*height_constant);
    ui->puzzle_dock_widget->setMaximumWidth(size*width_constant);
    ui->puzzle_dock_widget->setMaximumHeight(size*height_constant);

    ui->solution_dock_widget->resize(size*width_constant,size*height_constant);
    ui->solution_dock_widget->setMinimumWidth(size*width_constant);
    ui->solution_dock_widget->setMinimumHeight(size*height_constant);
    ui->solution_dock_widget->setMaximumWidth(size*width_constant);
    ui->solution_dock_widget->setMaximumHeight(size*height_constant);
}

//Changes the update interval
void MainWindow::change_interval(){
    m_timer->setInterval(ui->interval_spinBox->value());
}

//Resets the puzzle board to a initial configuration
void MainWindow::reset(){
    if(m_solver!=NULL){
        delete m_solver;
        m_solver = NULL;
    }
    ui->num_nodes_label->setVisible(false);
    ui->time_label->setVisible(false);
    ui->result_label->setVisible(false);

    ui->size_spinBox->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->scramble_pushButton->setEnabled(true);
    ui->solve_pushButton->setEnabled(true);
    ui->find_solution_pushButton->setEnabled(true);
    ui->listWidget->clear();
    setup_board();
    m_puzzle_widget->lock_board(false);
    m_solution_widget->lock_board(ui->lock_checkBox->isChecked());
}

//Sets the initial configurations for solving a puzzle and starts the timer.
//The timer will call solve_puzzle whenever it times out.
void MainWindow::start(){
    ui->size_spinBox->setEnabled(false);
    ui->comboBox->setEnabled(false);
    ui->scramble_pushButton->setEnabled(false);
    ui->num_nodes_label->setVisible(false);
    ui->time_label->setVisible(false);
    ui->result_label->setVisible(false);
    m_solver = new PuzzleSolver(m_board,m_goal);
    m_total_time->restart();
    ui->solve_pushButton->setEnabled(false);
    ui->find_solution_pushButton->setEnabled(false);
    ui->reset_pushButton->setEnabled(false);
    ui->stop_pushButton->setEnabled(true);
    m_puzzle_widget->lock_board(true);
    m_solution_widget->lock_board(true);
    m_timer->start();
}


void MainWindow::thread_stopped(){
    check_status();
    m_thread->terminate();
    delete m_thread;
    m_thread = NULL;
}

//This finds the solution, but locks the screen. Its a bit faster than using the "solve" button
void MainWindow::find_solution(){
    m_solver = new PuzzleSolver(m_board,m_goal);
    m_total_time->restart();
    ui->solve_pushButton->setEnabled(false);
    ui->find_solution_pushButton->setEnabled(false);
    ui->scramble_pushButton->setEnabled(false);    
    ui->stop_pushButton->setEnabled(true);

    ui->num_nodes_label->setVisible(false);
    ui->time_label->setVisible(false);
    ui->result_label->setVisible(false);

    m_thread = new PuzzleThread(m_solver);
    connect(m_thread,SIGNAL(stopped()),this,SLOT(thread_stopped()));
    m_thread->start();
}

//Stops the current calculation
void MainWindow::stop(){
    m_timer->stop();
    ui->reset_pushButton->setEnabled(true);
    ui->stop_pushButton->setEnabled(false); 
    ui->time_label->setVisible(true);
    QString elapsed_time;
    elapsed_time.append(QString::number(m_total_time->elapsed()));
    elapsed_time.append(" ms");
    ui->time_label->setText(elapsed_time);

    QString num_nodes;
    num_nodes.append("Nodes: ");
    num_nodes.append(QString::number(m_solver->get_num_nodes()));
    ui->num_nodes_label->setVisible(true);
    ui->num_nodes_label->setText(num_nodes);    

    if(m_thread != NULL){
        if(m_thread->isRunning()){
            m_thread->terminate();
        }
    }
}

//Calls the AI agent to solve the puzzle
void MainWindow::solve_puzzle(){    
    m_solver->solve();
    if(ui->preview_checkBox->isChecked()){
        m_puzzle_widget->set_curr_board(this->m_board);
        this->m_board->set_board(m_solver->get_current_board());
    }
    check_status();
}

//Checks if a solution was found or not
int MainWindow::check_status(){   
    switch(m_solver->get_status()){
    case SOLUTION_FOUND:       
        stop();
        build_solution_list();      
        return 1;
        break;
    case NO_SOLUTION_FOUND:
        ui->solve_pushButton->setEnabled(true);
        ui->find_solution_pushButton->setEnabled(true);
        ui->result_label->setVisible(true);
        ui->result_label->setText("No solution found");
        ui->time_label->setVisible(true);
        stop();      
        return 0;
        break;
    default:
        break;
    }
    return 0;
}

//Scrambles the board
void MainWindow::scramble(){
    m_board->scramble_board_with_moves();
    m_puzzle_widget->set_curr_board(this->m_board);
}

MainWindow::~MainWindow()
{
    delete m_timer;
    delete m_total_time;
    delete ui;
}

//When the calculations stop, this method is called to build the solution steps
void MainWindow::build_solution_list(){
    m_solution_nodes = m_solver->get_solution_nodes();
    Node *curr_node;
    QListWidgetItem *item = NULL;
    QString log;
    ui->listWidget->clear();
    int index;
    for(index = (*m_solution_nodes).size()-1; index >= 0; index--){

        curr_node = (*m_solution_nodes)[index];
        log.clear();
        if(curr_node->m_depth == 0){
            log.append("Root ");
            log.append(" Depth: ");
            log.append(QString::number(curr_node->m_depth));
            log.append(" Cost: ");
            log.append(QString::number(curr_node->m_cost));

        }else{
            log.append("Node ");
            log.append(" Depth: ");
            log.append(QString::number(curr_node->m_depth));
            log.append(" Cost: ");
            log.append(QString::number(curr_node->m_cost));
            log.append(" Moved: ");
            log.append(QString::number(curr_node->m_action->m_piece_value));
            switch(curr_node->m_action->m_move){
            case LEFT:
                log.append(" Left");
                break;
            case RIGHT:
                log.append(" Right");
                break;
            case UP:
                log.append(" Up");
                break;
            case DOWN:
                log.append(" Down");
                break;
            }
        }
        item = new QListWidgetItem(log,0,index);
        ui->listWidget->addItem(item);
    }
}

//When the user clicks in one of the solution states, this method sets it on the puzzle widget
void MainWindow::set_log_board(int item){
    unsigned int index = ((*m_solution_nodes).size()-1) - item;
    if(index < (*m_solution_nodes).size()){
        Node *curr_node = ((*m_solution_nodes))[index];
        m_puzzle_widget->set_curr_board((curr_node->m_state->m_board));
    }
}
