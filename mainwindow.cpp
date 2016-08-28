#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    //初始化Model
    timestep = 0.001;
    width = 30;
    height = 30;
    Force Gravity =  Force(vec3{0,-9.8,0});
    //grid = new Grid(width,height,1,timestep,Gravity);
    grid = new Grid(width,10,timestep,Gravity);


    //初始化部件
    timer = new QTimer(this);
    timer->start(1);
    connect(timer,SIGNAL(timeout()),this,SLOT(next_state()));


    setMode(3);

/*顶部对角双悬挂
    grid->arr_mass[0][0]->m_external_force = new Force(vec3{0,0,0});
    grid->arr_mass[width-1][0]->m_external_force = new Force(vec3{0,0,0});
    grid->arr_mass[0][0]->stabled = true;
    grid->arr_mass_TimeStepPrediction[0][0]->stabled = true;
    grid->arr_mass[width-1][0]->stabled = true;
    grid->arr_mass_TimeStepPrediction[width-1][0]->stabled = true;

    grid->arr_mass[width-2][height-4]->m_external_force = new Force(vec3{0,0,-20});
*/

/*顶部对角双悬挂有风
    grid->arr_mass[0][0]->m_external_force = new Force(vec3{0,0,0});
    grid->arr_mass[width-1][0]->m_external_force = new Force(vec3{0,0,0});
    grid->arr_mass[width/2][0]->m_external_force = new Force(vec3{0,0,0});
    grid->arr_mass[0][0]->stabled = true;
    grid->arr_mass_TimeStepPrediction[0][0]->stabled = true;
    grid->arr_mass[width/2][0]->stabled = true;
    grid->arr_mass_TimeStepPrediction[width/2][0]->stabled = true;
    grid->arr_mass[width-1][0]->stabled = true;
    grid->arr_mass_TimeStepPrediction[width-1][0]->stabled = true;

    for(int i=0;i<=width-1;i++)
    {
        for(int j=0;j<=height-1;j++)
        {
            if(i!=0||i!=width/2||i!=width-1)
            {
                grid->arr_mass[i][j]->m_external_force = new Force(vec3{0,0,5});
            }
        }
    }
*/

/*左部对角双悬挂
    grid->arr_mass[0][0]->m_external_force = new Force(vec3{0,0,0});
    grid->arr_mass[0][height-1]->m_external_force = new Force(vec3{0,0,0});
    grid->arr_mass[0][0]->stabled = true;
    grid->arr_mass_TimeStepPrediction[0][0]->stabled = true;
    grid->arr_mass[0][height-1]->stabled = true;
    grid->arr_mass_TimeStepPrediction[0][height-1]->stabled = true;

    grid->arr_mass[width-1][0]->m_external_force = new Force(vec3{20,0,-20});
    grid->arr_mass[width-1][height-1]->m_external_force = new Force(vec3{20,0,-20});
*/

/*中部单悬挂
    grid->arr_mass[width/2][0]->m_external_force = new Force(vec3{0,0,0});
    grid->arr_mass[width/2][0]->stabled = true;
    grid->arr_mass_TimeStepPrediction[width/2][0]->stabled = true;
    grid->arr_mass[width-1][0]->m_external_force = new Force(vec3{20,0,-20});
    grid->arr_mass[0][0]->m_external_force = new Force(vec3{20,0,-20});
*/

/*船帆
    for(int i=0;i<=width-1;i++)
    {
        grid->arr_mass[i][0]->m_external_force = new Force(vec3{0,0,0});

        grid->arr_mass[i][0]->stabled = true;
        grid->arr_mass_TimeStepPrediction[i][0]->stabled = true;
    }

    grid->arr_mass[0][height-1]->m_external_force = new Force(vec3{0,0,0});
    grid->arr_mass[width-1][height-1]->m_external_force = new Force(vec3{0,0,0});

    grid->arr_mass[0][height-1]->stabled = true;
    grid->arr_mass_TimeStepPrediction[0][height-1]->stabled = true;
    grid->arr_mass[width-1][height-1]->stabled = true;
    grid->arr_mass_TimeStepPrediction[width-1][height-1]->stabled = true;

    for(int i=0;i<=width-1;i++)
    {
        for(int j=0;j<=height-1;j++)
        {
            if(j!=0)
            {
                grid->arr_mass[i][j]->m_external_force = new Force(vec3{0,0,20});
            }
        }
    }
*/

/*桌布Circle
    int c_x = width/2,c_y = height/2;
    for(int i=0;i<=width-1;i++)
    {
        for(int j=0;j<=height-1;j++)
        {
            if(sqrt((i-c_x)*(i-c_x)+(j-c_y)*(j-c_y))<=10)
            {
                setStabled(i,j)
            }else
            {
                grid->arr_mass[i][j]->m_external_force = new Force(vec3{0,0,-9.8});
            }
        }
    }
    */

 /*桌布Square
        double c_x = width/2,c_y = height/2;
        double o,p;
        double factor = 0.6;
        for(int i=0;i<=width-1;i++)
        {
            for(int j=0;j<=height-1;j++)
            {
                o = (fabs(i-c_x)/c_x);
                p = (fabs(j-c_y)/c_y);
                if((o<factor)&&(p<factor))
                {
                    setStabled(i,j);
                }else
                {
                    grid->arr_mass[i][j]->m_external_force = new Force(vec3{0,0,-9.8});
                }
            }
        }
*/

/*帐篷
    double c_x = width/2, c_y = height/2;
    double q_x = width/4, q_y = height/4;

    for(int i=0;i<=width-1;i++)
    {
        for(int j=0;j<=height-1;j++)
        {
            grid->arr_mass[i][j]->m_external_force = new Force(vec3{0,0,-9.8});
        }
    }

    setStabled(c_x-q_x,c_y-q_x);
    setStabled(c_x-q_x,c_y+q_x);
    setStabled(c_x+q_x,c_y-q_x);
    setStabled(c_x+q_x,c_y+q_x);
*/

/*底部全固定
    setStabled(0,height-1);
    setStabled(width-1,0);
    setStabled(width/2,height-1);
    setStabled(width-1,height-1);
    grid->arr_mass[0][0]->m_external_force = new Force(vec3{0,0,-2});
    grid->arr_mass[width/2][0]->m_external_force = new Force(vec3{0,0,-2});
*/

/*对角拉扯
    for(int i=0;i<=width-1;i++)
    {
        for(int j=0;j<=height-1;j++)
        {
            grid->arr_mass[i][j]->m_external_force = new Force(vec3{0,0,0});
        }
    }
    double f = 1000;
    //grid->arr_mass[0][0]->m_external_force = new Force(vec3{f,-f,10});
    grid->arr_mass[width/2][0]->m_external_force = new Force(vec3{0,f,1});
    grid->arr_mass[width/2][height-1]->m_external_force = new Force(vec3{0,-f,1});
    //grid->arr_mass[width-1][height-1]->m_external_force = new Force(vec3{-f,f,10});
*/

    //初始化 主Widget
    centralWidget = new QWidget;
    setGeometry(100,100,1200,768);
    setWindowTitle("Fabric Simulator");
    setCentralWidget(centralWidget);

    //初始化openGL Widget
    glWidget = new openGLWidget(width,height);//glWidget = new openGLWidget;
    glWidget->set_arr_position(grid->get_position_array());

    openGLWidgetArea = new QScrollArea;
    openGLWidgetArea->setWidget(glWidget);
    openGLWidgetArea->setWidgetResizable(true);
    openGLWidgetArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    openGLWidgetArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    openGLWidgetArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    openGLWidgetArea->setMinimumSize(50, 50);

    //初始化控制台Widget
    controlWidget = new QWidget;
    button_previous_state = new QPushButton("previous");
    connect(button_previous_state, SIGNAL(clicked()), this, SLOT(previous_state()));
    button_next_state = new QPushButton("Next");
    connect(button_next_state, SIGNAL(clicked()), this, SLOT(next_state()));
    button_continue_pause = new QPushButton("Pause");
    connect(button_continue_pause, SIGNAL(clicked()), this, SLOT(continue_pause()));
    button_confirm_size = new QPushButton("Confirm");
    connect(button_confirm_size, SIGNAL(clicked()), this, SLOT(confirm_size()));
    button_confirm_command = new QPushButton("Confirm");
    connect(button_confirm_command, SIGNAL(clicked()), this, SLOT(confirm_command()));
    button_load_texture = new QPushButton("Load Texture");
    connect(button_load_texture, SIGNAL(clicked()), this, SLOT(load_texture()));
    button_confirm_elasticity = new QPushButton("Confirm");
    connect(button_confirm_elasticity, SIGNAL(clicked()), this, SLOT(confirm_elasticity()));
    button_confirm_range = new QPushButton("Confirm");
    connect(button_confirm_range, SIGNAL(clicked()), this, SLOT(confirm_range()));
    edit_width = new QLineEdit("");
    edit_width->setPlaceholderText("width");
    edit_width->setText(QString::number(width,10));
    edit_height = new QLineEdit("");
    edit_height->setPlaceholderText("height");
    edit_height->setText(QString::number(height,10));
    edit_command = new QLineEdit("");
    edit_command->setPlaceholderText("Input your command");
    edit_elasticity = new QLineEdit("");
    edit_elasticity->setText(QString::number(grid->arr_mass[0][0]->arr_spring[0]->m_elasticity,'f',3));
    edit_range = new QLineEdit("");
    edit_range->setText(QString::number(grid->arr_mass[0][0]->arr_spring[0]->m_range,'f',3));
    lable_elasticity = new QLabel("Elasticity:");
    lable_range = new QLabel("Range:");
    lable_sysInfo = new QLabel("");
    lable_sysInfo->setText(QString(""));

//    QGridLayout *controlLayout = new QGridLayout;
//    controlLayout->addWidget(edit_width,0,0,1,1);
//    controlLayout->addWidget(edit_height,0,1,1,1);
//    controlLayout->addWidget(button_confirm_size,0,2,1,1);
//    controlLayout->addWidget(button_continue_pause,1,0,1,3);
//    controlLayout->addWidget(button_previous_state,2,0,1,1);
//    controlLayout->addWidget(button_next_state,2,1,1,2);
//    controlLayout->addWidget(edit_command,3,0,1,3);
//    controlLayout->addWidget(button_confirm_command,3,3,1,1);
//    controlLayout->addWidget(lable_sysInfo,4,0,1,4);
//    controlWidget->setLayout(controlLayout);

    QGridLayout *controlLayout = new QGridLayout;
    controlLayout->addWidget(edit_width,0,0,1,2);
    controlLayout->addWidget(edit_height,0,2,1,2);
    controlLayout->addWidget(button_confirm_size,0,4,1,1);
    controlLayout->addWidget(button_continue_pause,1,0,1,5);
    controlLayout->addWidget(button_next_state,2,0,1,5);
    controlLayout->addWidget(button_load_texture,3,0,1,5);
    controlLayout->addWidget(edit_command,4,0,1,4);
    controlLayout->addWidget(button_confirm_command,4,4,1,1);
    controlLayout->addWidget(lable_elasticity,5,0,1,1);
    controlLayout->addWidget(edit_elasticity,5,1,1,3);
    controlLayout->addWidget(button_confirm_elasticity,5,4,1,1);
    controlLayout->addWidget(lable_range,6,0,1,1);
    controlLayout->addWidget(edit_range,6,1,1,3);
    controlLayout->addWidget(button_confirm_range,6,4,1,1);
    controlLayout->addWidget(lable_sysInfo,7,0,1,5);
    controlWidget->setLayout(controlLayout);

    controlArea = new QScrollArea;
    controlArea->setWidget(controlWidget);
    controlArea->setWidgetResizable(true);
    controlArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    controlArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    controlArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    controlArea->setMinimumSize(50, 50);

    //将控件加入到主Widget里去
    QGridLayout *centralLayout = new QGridLayout;
    centralLayout->addWidget(openGLWidgetArea, 0, 0, 1, 3);
    centralLayout->addWidget(controlArea, 0, 3, 1, 1);

    centralWidget->setLayout(centralLayout);

    createActions();;
    createMenus();
}


MainWindow::~MainWindow()
{

}

//SLOT Implementation
void MainWindow::about()
{
    QMessageBox::about(this, tr("About Grabber"),
             tr("The <b>Grabber</b> example demonstrates two approaches for "
                "rendering OpenGL into a Qt pixmap."));
}
void MainWindow::previous_state()
{
    static bool flag_wind = true;
    if(flag_wind)
    {
        grid->arr_mass[width-1][0]->m_external_force = new Force(vec3{0,-9.8,0});
        grid->arr_mass[width-2][0]->m_external_force = new Force(vec3{0,-9.8,0});
        flag_wind = false;
    }else
    {
        grid->arr_mass[width-1][0]->m_external_force = new Force(vec3{700,1500,0});
        grid->arr_mass[width-2][0]->m_external_force = new Force(vec3{700,1500,0});
        flag_wind = true;
    }

}
void MainWindow::next_state()
{
    grid->next_state();
    glWidget->set_arr_position(grid->get_position_array());
    glWidget->set_timeStep(grid->m_timeStep);
    glWidget->paint();

    //static float itr = 0.0f;
    Mass* m_mass1 = grid->arr_mass[0][0]->arr_spring[1]->m_mass1;
    Mass* m_mass2 = grid->arr_mass[0][0]->arr_spring[1]->m_mass2;
    double length = m_mass1->m_position.distanceFrom(m_mass2->m_position);
    ++itr;
    std::cout<<itr<<": "<<length<<" step:"<<grid->m_timeStep<<std::endl;

}
void MainWindow::confirm_size()
{
    int t_width = edit_width->text().toInt();
    int t_height= edit_height->text().toInt();
    width = t_width;
    height = t_height;

    //初始化Model
    double timestep = 0.001;
    Force Gravity =  Force(vec3{0,-9.8,0});
    //grid = new Grid(width,height,1,timestep,Gravity);
    delete grid;
    grid = new Grid(t_width,10,timestep,Gravity);

    setMode(modeFlag);

    //初始化openGL Widget
    delete glWidget;
    glWidget = new openGLWidget(t_width,t_height);//glWidget = new openGLWidget;
    glWidget->set_arr_position(grid->get_position_array());

    glWidget->iteration = 0;
    glWidget->time = 0;


    openGLWidgetArea->setWidget(glWidget);

    timer->start(1);
    connect(timer,SIGNAL(timeout()),this,SLOT(next_state()));
    button_continue_pause->setText("Pause");
}

void MainWindow::load_texture()
{
    QString t_add = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("Allfile(*.*);;mp3file(*.mp3)"));
    address_texture = t_add==QString("") ? address_texture : t_add;

    glWidget->loadGLTextures(address_texture);

    std::cout<<"previous state"<<std::endl;
}

void MainWindow::confirm_command()
{
    QString command = edit_command->text();
    if(command == QString("Cease All Winds"))
    {
        for(int i=0;i<=width-1;i++)
        {
            for(int j=0;j<=height-1;j++)
            {
                if(!grid->arr_mass[i][j]->stabled)
                {
                    delete grid->arr_mass[i][j]->m_external_force;
                    grid->arr_mass[i][j]->m_external_force = new Force(vec3{0,-9.8,0});
                }
            }
        }
    }
    else if(command == QString("Flag"))
    {
        //初始化Model
        double timestep = 0.001;
        Force Gravity =  Force(vec3{0,-9.8,0});
        delete grid;
        grid = new Grid(width,10,timestep,Gravity);

        grid->arr_mass[0][0]->m_external_force = new Force(vec3{0,0,0});
        grid->arr_mass[0][height-1]->m_external_force = new Force(vec3{0,0,0});
        grid->arr_mass[0][0]->stabled = true;
        grid->arr_mass_TimeStepPrediction[0][0]->stabled = true;
        grid->arr_mass[0][height-1]->stabled = true;
        grid->arr_mass_TimeStepPrediction[0][height-1]->stabled = true;

        grid->arr_mass[width-1][0]->m_external_force = new Force(vec3{20,0,-20});
        grid->arr_mass[width-1][height-1]->m_external_force = new Force(vec3{20,0,-20});

        //初始化openGL Widget
        delete glWidget;
        glWidget = new openGLWidget(width,height);//glWidget = new openGLWidget;
        glWidget->set_arr_position(grid->get_position_array());

        glWidget->iteration = 0;
        glWidget->time = 0;


        openGLWidgetArea->setWidget(glWidget);

        timer->start(1);
        connect(timer,SIGNAL(timeout()),this,SLOT(next_state()));
        button_continue_pause->setText("Pause");
    }
    else if(command == QString("Wind"))
    {
        static bool flag_wind = true;
        if(flag_wind)
        {
            grid->arr_mass[width-1][0]->m_external_force = new Force(vec3{0,-9.8,0});
            grid->arr_mass[width-2][0]->m_external_force = new Force(vec3{0,-9.8,0});
            flag_wind = false;
        }else
        {
            grid->arr_mass[width-1][0]->m_external_force = new Force(vec3{700,1500,0});
            grid->arr_mass[width-2][0]->m_external_force = new Force(vec3{700,1500,0});
            flag_wind = true;
        }
    }
    else
    {
        lable_sysInfo->setText(QString("No Command Found"));
    }
}

void MainWindow::continue_pause()
{

    std::cout<<button_continue_pause->text().toStdString()<<std::endl;
    if(button_continue_pause->text() == tr("Continue"))
    {
        timer->start(1);
        connect(timer,SIGNAL(timeout()),this,SLOT(next_state()));
        button_continue_pause->setText("Pause");
    }
    else if(button_continue_pause->text() == tr("Pause"))
    {
        timer->stop();
        button_continue_pause->setText("Continue");
    }
}

void MainWindow::createActions()
{
    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(tr("&File"));
    fileMenu->addAction(exitAction);
    //fileMenu->addSeparator();

    helpMenu = new QMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);

    /* 将菜单添加到菜单栏上 */
    QMenuBar *menuBar = this->menuBar();
    menuBar->addMenu(fileMenu);
    menuBar->addMenu(helpMenu);
    std::cout<<"Height: "<<menuBar->height()<<std::endl;
}

void MainWindow::setStabled(int t_x,int t_y)
{
    grid->arr_mass[t_x][t_y]->m_external_force = new Force(vec3{0,0,0});
    grid->arr_mass[t_x][t_y]->stabled = true;
    grid->arr_mass_TimeStepPrediction[t_x][t_y]->stabled = true;
}

void MainWindow::setMode(int flag)
{
    modeFlag = flag;
    switch (flag) {
    case 0:{
        //顶部对角双悬挂
        setStabled(0,0);
        setStabled(width-1,0);
        grid->arr_mass[width-2][height-4]->m_external_force = new Force(vec3{0,0,-20});
        break;
    }
    case 1:{
        //左部对角双悬挂
        setStabled(0,0);
        setStabled(0,height-1);
        grid->arr_mass[width-1][0]->m_external_force = new Force(vec3{20,0,-20});
        grid->arr_mass[width-1][height-1]->m_external_force = new Force(vec3{20,0,-20});
        break;
    }
    case 2:{
        //桌布Circle
        int c_x = width/2,c_y = height/2;
        for(int i=0;i<=width-1;i++)
        {
            for(int j=0;j<=height-1;j++)
            {
                if(sqrt((i-c_x)*(i-c_x)+(j-c_y)*(j-c_y))<=10)
                {
                    setStabled(i,j);
                }else
                {
                    grid->arr_mass[i][j]->m_external_force = new Force(vec3{0,0,-9.8});
                }
            }
        }
        break;
    }
    case 3:{
        //桌布Square
        double c_x = width/2,c_y = height/2;
        double o,p;
        double factor = 0.6;
        for(int i=0;i<=width-1;i++)
        {
            for(int j=0;j<=height-1;j++)
            {
                o = (fabs(i-c_x)/c_x);
                p = (fabs(j-c_y)/c_y);
                if((o<factor)&&(p<factor))
                {
                    setStabled(i,j);
                }else
                {
                    grid->arr_mass[i][j]->m_external_force = new Force(vec3{0,0,-9.8});
                }
            }
        }
        break;
    }
    case 4:{
        //对角拉扯
        for(int i=0;i<=width-1;i++)
        {
            for(int j=0;j<=height-1;j++)
            {
                grid->arr_mass[i][j]->m_force = new Force(vec3{0,0,0});
                grid->arr_mass[i][j]->m_external_force = new Force(vec3{0,0,0});
            }
        }
        double f = 1000;
        grid->arr_mass[width-1][0]->m_external_force = new Force(vec3{f,f,10});
        grid->arr_mass[0][height-1]->m_external_force = new Force(vec3{-f,-f,10});
        break;
    }
    case 5:{
        //顶部对角双悬挂有风
        setStabled(0,0);
        setStabled(width-1,0);
        setStabled(width/2,0);
        for(int i=0;i<=width-1;i++)
        {
            for(int j=0;j<=height-1;j++)
            {
                if(i!=0||i!=width/2||i!=width-1)
                {
                    grid->arr_mass[i][j]->m_external_force = new Force(vec3{0,0,5});
                }
            }
        }
        break;
    }
    default:
        break;
    }

}


void MainWindow::confirm_elasticity()
{
    reDraw();
    grid->setElasticity(edit_elasticity->text().toDouble());
    std::cout<<grid->arr_mass[0][0]->arr_spring[0]->m_elasticity<<std::endl;
}

void MainWindow::confirm_range()
{
    reDraw();
    grid->setRange(edit_range->text().toDouble());
}

void MainWindow::reDraw()
{
    //初始化Model
    double timestep = 0.001;
    Force Gravity =  Force(vec3{0,-9.8,0});
    //grid = new Grid(width,height,1,timestep,Gravity);
    delete grid;
    grid = new Grid(width,10,timestep,Gravity);

    setMode(modeFlag);

    //初始化openGL Widget
    delete glWidget;
    glWidget = new openGLWidget(width,height);//glWidget = new openGLWidget;
    glWidget->set_arr_position(grid->get_position_array());

    glWidget->iteration = 0;
    glWidget->time = 0;

    openGLWidgetArea->setWidget(glWidget);

    timer->start(1);
    connect(timer,SIGNAL(timeout()),this,SLOT(next_state()));
    button_continue_pause->setText("Pause");
}
