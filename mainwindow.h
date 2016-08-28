#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtOpenGL>
#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QSlider>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QMenuBar>
#include <QTimer>
#include <QFileDialog>
#include <QApplication>
#include "openglwidget.h"
#include "grid.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void about();
    void previous_state();
    void next_state();
    void confirm_size();
    void confirm_command();
    void continue_pause();
    void load_texture();
    void confirm_elasticity();
    void confirm_range();


private:

    void createMenus();
    void createActions();
    void setStabled(int,int);
    void setMode(int);
    void reDraw();

    QWidget *centralWidget;

    QScrollArea *openGLWidgetArea;
    QScrollArea *controlArea;
    QPushButton *button_previous_state;
    QPushButton *button_next_state;
    QPushButton *button_confirm_size;
    QPushButton *button_continue_pause;
    QPushButton *button_load_texture;
    QPushButton *button_confirm_command;
    QPushButton *button_confirm_elasticity;
    QPushButton *button_confirm_range;



    QLineEdit *edit_width;
    QLineEdit *edit_height;
    QLineEdit *edit_command;
    QLineEdit *edit_elasticity;
    QLineEdit *edit_range;
    QLabel *lable_elasticity;
    QLabel *lable_range;
    QLabel *lable_sysInfo;

    QFileDialog *fileDialog_texture;


    openGLWidget *glWidget;
    QWidget *controlWidget;
    //grid作为Model，承担了底层代码的实现
    Grid *grid;

    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *previousStateAction;
    QAction *nextStateAction;
    QAction *exitAction;
    QAction *aboutAction;

    QTimer *timer;

    int width, height;
    int modeFlag = 0;//存储当前显示模式
    double timestep;
    float itr = 0.0f;
    QString address_texture = QString("/Users/WenYang/QtTest6/texture4.jpg");

};

#endif // MAINWINDOW_H
