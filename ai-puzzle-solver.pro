#-------------------------------------------------
#
# Project created by QtCreator 2011-09-01T23:20:38
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = ai-puzzle-solver
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    puzzle_board.cpp \
    board_widget.cpp \
    puzzle_solver.cpp \
    heuristics.cpp \
    node_container.cpp \
    node.cpp \
    puzzle_thread.cpp

HEADERS  += mainwindow.h \
    puzzle_board.h \
    board_widget.h \
    puzzle_solver.h \
    heuristics.h \
    node_container.h \
    node.h \
    puzzle_thread.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc
