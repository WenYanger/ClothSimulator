#include "openglwidget.h"


openGLWidget::openGLWidget(QWidget *parent) : QGLWidget(parent)
{
    xRot = yRot = zRot = 0;
    m_width = 2;
    m_height = 2;
    //给坐标数组分配内存
    arr_position_3f = new GLfloat*[m_width*m_height];
    int i=0;
    for(i=0;i<m_width*m_height;i++)
        arr_position_3f[i] = new GLfloat[3];

}

openGLWidget::openGLWidget(int width, int height, QWidget *parent) : QGLWidget(parent)
{
    xRot = yRot = zRot = 0;
    m_width = width;
    m_height = height;
    iteration = 0;
    //给坐标数组分配内存
    arr_position = new GLfloat[m_width*m_height*3];
    arr_position_3f = new GLfloat*[m_width*m_height];
    int i=0,j=0;
    for(i=0;i<m_width*m_height;i++)
        arr_position_3f[i] = new GLfloat[3];

    arr_position_6f = new GLfloat**[m_width];
    for(i=0;i<m_width;i++)
        arr_position_6f[i] = new GLfloat*[m_height];
    for(i=0;i<m_width;i++)
    {
        for(j=0;j<m_height;j++)
        {
            arr_position_6f[i][j] = new GLfloat[3];
        }
    }
}

openGLWidget::~openGLWidget()
{}

void openGLWidget::set_arr_position(GLfloat * arr)
{
    arr_position = arr;
}

void openGLWidget::set_timeStep(double timeS)
{
    timeStep = timeS;
}

void openGLWidget::paint()
{
    updateGL();
}

//以下是私有方法
void openGLWidget::initializeGL()
{
    glShadeModel( GL_SMOOTH );//启用阴影平滑
    glClearColor( 0.0, 0.0, 0.0, 0.0 );// 黑色背景
    glClearDepth( 1.0 ); // 设置深度缓存
    glEnable( GL_DEPTH_TEST );// 启用深度测试
    glDepthFunc( GL_LEQUAL );// 所作深度测试的类型
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );// 告诉系统对透视进行修正

    loadGLTextures();// 启用纹理
    glEnable( GL_TEXTURE_2D );

    glEnable(GL_POLYGON_SMOOTH);//启用抗锯齿
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);


    //启用光照
//    GLfloat lightAmbient[4] = { 0.5, 0.5, 0.5, 1.0 };
//    GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
//    GLfloat lightPosition[4] = { 0.0, 0.0, 12.0, 1.0 };
//    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
//    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
//    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );
//    glEnable( GL_LIGHT1 );
//    glEnable(GL_LIGHTING);


}
void openGLWidget::paintGL()
{
    // 清除屏幕和深度缓存
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    //将当前矩阵设置为单位矩阵（初始化）
    glLoadIdentity();
    //绘制底层标准面
    paintHorizontalLevel();
    //绘制网格
    paintGrid(arr_position);
    //paintGridOnly(arr_position);
    //显示fps
    cal_fps();

}
 // 重置OpenGL窗口大小
void openGLWidget::resizeGL(int width, int height)
{
        // 防止窗口大小变为0
    if ( height == 0 )
    {
        height = 1;
    }
    // 重置当前的视口
    glViewport( 0, 0, (GLint)width, (GLint)height );
    // 选择投影矩阵（指定当前矩阵为）
    glMatrixMode( GL_PROJECTION );
    // 重置投影矩阵
    glLoadIdentity();

    // 设置视口的大小
    gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );

    // 设置相机方向
    //gluLookAt (3.0, 3.0, 5.0, 0.0, 0.0, -8.0, 0.0, 1.0, 0.0);
    gluLookAt (6.0, 9.0, -5.0, 0.0, 0.0, -8.0, 0.0, 0.0, 1.0);

    // 选择模型观察矩阵（指定当前矩阵为模型——视图矩阵）
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

//纹理装载函数
void openGLWidget::loadGLTextures()
{
    QImage tex,image;
    if(image.load("/Users/WenYang/QtTest6/texture4.jpg"))
    {
        qWarning("success!");
    }else{
        //如果不成功，自动生成一个128*128的32位绿色图片
        qWarning("Could not read image file!");
        QImage dummy(128,128,QImage::Format_RGB32);
        dummy.fill(Qt::red);
        image = dummy;
    }
    //转换成纹理类型
    tex = QGLWidget::convertToGLFormat(image);
    //创建2维纹理
     glGenTextures( 1, &texture[0] );
    //使用来自位图数据生成的典型纹理,将纹理名字texture[0]绑定到纹理目标上
     glBindTexture( GL_TEXTURE_2D, texture[0] );
     glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
           GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
     glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
     glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

     //创建3维纹理
      glGenTextures( 1, &texture[1] );
     //使用来自位图数据生成的典型纹理,将纹理名字texture[0]绑定到纹理目标上
      glBindTexture( GL_TEXTURE_3D, texture[1] );
      glTexImage3D( GL_TEXTURE_3D, 0, 3, tex.width(), tex.height(), 1,
                    1, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void openGLWidget::loadGLTextures(QString address_texture)
{
    QImage tex,image;
    if(image.load(address_texture))
    {
        qWarning("success!");
    }else{
        //如果不成功，自动生成一个128*128的32位绿色图片
        qWarning("Could not read image file!");
        QImage dummy(128,128,QImage::Format_RGB32);
        dummy.fill(Qt::red);
        image = dummy;
    }
    //转换成纹理类型
    tex = QGLWidget::convertToGLFormat(image);
    //创建2维纹理
     glGenTextures( 1, &texture[0] );
    //使用来自位图数据生成的典型纹理,将纹理名字texture[0]绑定到纹理目标上
     glBindTexture( GL_TEXTURE_2D, texture[0] );
     glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
           GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
     glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
     glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

     //创建3维纹理
      glGenTextures( 1, &texture[1] );
     //使用来自位图数据生成的典型纹理,将纹理名字texture[0]绑定到纹理目标上
      glBindTexture( GL_TEXTURE_3D, texture[1] );
      glTexImage3D( GL_TEXTURE_3D, 0, 3, tex.width(), tex.height(), 1,
                    1, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
      glTexParameteri( GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void openGLWidget::paintHorizontalLevel()
{
    //将当前矩阵设置为单位矩阵（初始化）
    glLoadIdentity();
    //绘制底层标准面
    GLfloat width = 12.0, height = 12.0;
    //坐标转移
    glTranslatef( -width/2,-4.0f,-6.0f );

    glBegin( GL_QUADS );
        //设置颜色
        glColor3f( 0.0, 0.0, 1.0 );
        glVertex3f( 0.0, 0.0, 0.0 );
        glColor3f( 0.0, 1.0, 0.0 );
        glVertex3f( width, 0.0, 0.0 );
        glColor3f( 1.0, 0.0, 0.0 );
        glVertex3f( width, 0.0 ,-height);
        glVertex3f( 0.0, 0.0, -height);
    glEnd();


}

void openGLWidget::paintGrid()
{
    //将当前矩阵设置为单位矩阵（初始化）
    glLoadIdentity();

    int i = 0,j = 0;
    //缩放比例，越小，点的间距越小
    double factor = 0.2;
    //横纵点的个数
    int width = 10, height = 10;

    glTranslatef(-(width/2)*factor,(height/2)*factor,-8.0);
    glLineWidth( 3.0 );
    glPointSize( 5.0 );

    //绘制横线
    glColor3f(1.0,0.0,0.0);
    for(i=0;i<width;i++)
    {
        for(j=0;j<height+1;j++)
        {

            glBegin(GL_LINES);
                glVertex3f(i*factor         ,-j*factor,0.0);
                glVertex3f(i*factor + factor,-j*factor,0.0);
            glEnd();
        }
    }
    //绘制竖线
    for(i=0;i<width+1;i++)
    {
        for(j=0;j<height;j++)
        {
            glBegin(GL_LINES);
                glVertex3f(i*factor,-j*factor,0.0);
                glVertex3f(i*factor,-(j*factor + factor),0.0);
            glEnd();
        }
    }
    //绘制顶点
    glColor3f(1.0,1.0,1.0);
    for(i=0;i<width+1;i++)
    {
        for(j=0;j<height+1;j++)
        {
            glBegin(GL_POINTS);
                glVertex3f(i*factor,-j*factor,0.0);
            glEnd();
        }
    }
}

void openGLWidget::paintGrid(GLfloat* arr_position)
{
    //将当前矩阵设置为单位矩阵（初始化）
    glLoadIdentity();
    
    int i = 0,j = 0;
    GLfloat x,y,z;
    //缩放比例，越小，点的间距越小
    double factor = 0.2;

    //将Grid传过来的坐标转化为3f数组
    for(i=0;i<(m_width*m_height)*3;i++)
    {

        switch (i%3) {
        case 0:
            x = arr_position[i];
            break;
        case 1:
            y = arr_position[i];
            break;
        case 2:
            z = arr_position[i];
            arr_position_3f[i/3][0] = x;
            arr_position_3f[i/3][1] = y;
            arr_position_3f[i/3][2] = z;
            break;
        default:
            break;
        }
    }

    //将3f数组，转化为标准数组
    for(i=0;i<m_width*m_height;i++)
    {
        arr_position_6f[i/m_width][i%m_height][0] = arr_position_3f[i][0];
        arr_position_6f[i/m_width][i%m_height][1] = arr_position_3f[i][1];
        arr_position_6f[i/m_width][i%m_height][2] = arr_position_3f[i][2];
    }


    glTranslatef(-(m_width/2)*factor,(m_height/2)*factor,-8.0);
    glLineWidth( 3.0 );
    glPointSize( 5.0 );


    GLfloat x1,y1,z1,x2,y2,z2;
//    //绘制横线
//    glColor3f(1.0,0.0,0.0);
//    for(i=0; i<(m_width*m_height); i++)
//    {
//        if((i+1)%m_width == 0)
//            continue;
//        x1 = arr_position_3f[i][0];
//        y1 = arr_position_3f[i][1];
//        z1 = arr_position_3f[i][2];
//        x2 = arr_position_3f[i+1][0];
//        y2 = arr_position_3f[i+1][1];
//        z2 = arr_position_3f[i+1][2];

//        glBegin(GL_LINES);
//            glVertex3f(x1*factor, y1*factor, z1*factor);
//            glVertex3f(x2*factor, y2*factor, z2*factor);
//        glEnd();
//    }

//    //绘制竖线
//    for(i=0; i<(m_width*m_height); i++)
//    {
//        if(i+m_width>=(m_width*m_height))
//            break;
//        x1 = arr_position_3f[i][0];
//        y1 = arr_position_3f[i][1];
//        z1 = arr_position_3f[i][2];
//        x2 = arr_position_3f[i+m_width][0];
//        y2 = arr_position_3f[i+m_width][1];
//        z2 = arr_position_3f[i+m_width][2];
//        glBegin(GL_LINES);
//            glVertex3f(x1*factor, y1*factor, z1*factor);
//            glVertex3f(x2*factor, y2*factor, z2*factor);
//        glEnd();
//    }

//    //绘制顶点
//    glColor3f(1.0,1.0,1.0);
//    for(i=0; i<(m_width*m_height); i++)
//    {
//        x1 = arr_position_3f[i][0];
//        y1 = arr_position_3f[i][1];
//        z1 = arr_position_3f[i][2];
//        glBegin(GL_POINTS);
//            glVertex3f(x1*factor, y1*factor, z1*factor);
//        glEnd();
//    }

    glColor3f(1.0,1.0,1.0);
    //绘制纹理
    glBegin(GL_QUADS);
    for(i=0;i<m_width-1;i++)
    {
        for(j=0;j<m_height-1;j++)
        {
//            glVertex3f( arr_position_6f[i][j][0]*factor, arr_position_6f[i][j][1]*factor, arr_position_6f[i][j][2]*factor );
//            glTexCoord2f( (arr_position_6f[i][j][0]*factor+(m_width/2)*factor)/m_width*factor,
//                          (arr_position_6f[i][j][1]*factor+(m_height/2)*factor)/m_height*factor);
//            glVertex3f( arr_position_6f[i][j+1][0]*factor, arr_position_6f[i][j+1][1]*factor, arr_position_6f[i][j+1][2]*factor );
//            glTexCoord2f( (arr_position_6f[i][j+1][0]*factor+(m_width/2)*factor)/m_width*factor,
//                          (arr_position_6f[i][j+1][1]*factor+(m_height/2)*factor)/m_height*factor);
//            glVertex3f( arr_position_6f[i+1][j+1][0]*factor, arr_position_6f[i+1][j+1][1]*factor, arr_position_6f[i+1][j+1][2]*factor );
//            glTexCoord2f( (arr_position_6f[i+1][j+1][0]*factor+(m_width/2)*factor)/m_width*factor,
//                          (arr_position_6f[i+1][j+1][1]*factor+(m_height/2)*factor)/m_height*factor);
//            glVertex3f( arr_position_6f[i+1][j][0]*factor, arr_position_6f[i+1][j][1]*factor, arr_position_6f[i+1][j][2]*factor );
//              ( (arr_position_6f[i+1][j][0]*factor+(m_width/2)*factor)/m_width*factor,
//                          (arr_position_6f[i+1][j][1]*factor+(m_height/2)*factor)/m_height*factor);

//            glVertex3f( arr_position_6f[i][j][0]*factor, arr_position_6f[i][j][1]*factor, arr_position_6f[i][j][2]*factor );
//            glTexCoord2f( arr_position_6f[i][j][0]*factor/m_width*factor,
//                          arr_position_6f[i][j][1]*factor/m_height*factor);
//            glVertex3f( arr_position_6f[i][j+1][0]*factor, arr_position_6f[i][j+1][1]*factor, arr_position_6f[i][j+1][2]*factor );
//            glTexCoord2f( arr_position_6f[i][j+1][0]*factor/m_width*factor,
//                          arr_position_6f[i][j+1][1]*factor/m_height*factor);
//            glVertex3f( arr_position_6f[i+1][j+1][0]*factor, arr_position_6f[i+1][j+1][1]*factor, arr_position_6f[i+1][j+1][2]*factor );
//            glTexCoord2f( arr_position_6f[i+1][j+1][0]*factor/m_width*factor,
//                          arr_position_6f[i+1][j+1][1]*factor/m_height*factor);
//            glVertex3f( arr_position_6f[i+1][j][0]*factor, arr_position_6f[i+1][j][1]*factor, arr_position_6f[i+1][j][2]*factor );
//            glTexCoord2f( arr_position_6f[i+1][j][0]*factor/m_width*factor,
//                          arr_position_6f[i+1][j][1]*factor/m_height*factor);


//            glTexCoord2f( arr_position_6f[i][j][0]/m_width,arr_position_6f[i][j][1]/m_height);
//            glVertex3f( arr_position_6f[i][j][0]*factor, arr_position_6f[i][j][1]*factor, arr_position_6f[i][j][2]*factor );
//            glTexCoord2f( arr_position_6f[i][j+1][0]/m_width,arr_position_6f[i][j+1][1]/m_height);
//            glVertex3f( arr_position_6f[i][j+1][0]*factor, arr_position_6f[i][j+1][1]*factor, arr_position_6f[i][j+1][2]*factor );
//            glTexCoord2f( arr_position_6f[i+1][j+1][0]/m_width,arr_position_6f[i+1][j+1][1]/m_height);
//            glVertex3f( arr_position_6f[i+1][j+1][0]*factor, arr_position_6f[i+1][j+1][1]*factor, arr_position_6f[i+1][j+1][2]*factor );
//            glTexCoord2f( arr_position_6f[i+1][j][0]/m_width,arr_position_6f[i+1][j][1]/m_height);
//            glVertex3f( arr_position_6f[i+1][j][0]*factor, arr_position_6f[i+1][j][1]*factor, arr_position_6f[i+1][j][2]*factor );


            double factor1 = 0.9;
            glTexCoord3f( i*factor1/m_width,j*factor1/m_height,0.0);
            glVertex3f( arr_position_6f[i][j][0]*factor, arr_position_6f[i][j][1]*factor, arr_position_6f[i][j][2]*factor );

            glTexCoord3f( i*factor1/m_width,(j+1)*factor1/m_height,0.0);
            glVertex3f( arr_position_6f[i][j+1][0]*factor, arr_position_6f[i][j+1][1]*factor, arr_position_6f[i][j+1][2]*factor );


            glTexCoord3f( (i+1)*factor1/m_width,(j+1)*factor1/m_height,0.0);
            glVertex3f( arr_position_6f[i+1][j+1][0]*factor, arr_position_6f[i+1][j+1][1]*factor, arr_position_6f[i+1][j+1][2]*factor );


            glTexCoord3f( (i+1)*factor1/m_width,j*factor1/m_height,0.0);
            glVertex3f( arr_position_6f[i+1][j][0]*factor, arr_position_6f[i+1][j][1]*factor, arr_position_6f[i+1][j][2]*factor );

        }
    }
    glEnd();
}

void openGLWidget::paintGridOnly(GLfloat* arr_position)
{
    //将当前矩阵设置为单位矩阵（初始化）
    glLoadIdentity();

    int i = 0,j = 0;
    GLfloat x,y,z;
    //缩放比例，越小，点的间距越小
    double factor = 0.2;

    //将Grid传过来的坐标转化为3f数组
    for(i=0;i<(m_width*m_height)*3;i++)
    {

        switch (i%3) {
        case 0:
            x = arr_position[i];
            break;
        case 1:
            y = arr_position[i];
            break;
        case 2:
            z = arr_position[i];
            arr_position_3f[i/3][0] = x;
            arr_position_3f[i/3][1] = y;
            arr_position_3f[i/3][2] = z;
            break;
        default:
            break;
        }
    }

    //将3f数组，转化为标准数组
    for(i=0;i<m_width*m_height;i++)
    {
        arr_position_6f[i/m_width][i%m_height][0] = arr_position_3f[i][0];
        arr_position_6f[i/m_width][i%m_height][1] = arr_position_3f[i][1];
        arr_position_6f[i/m_width][i%m_height][2] = arr_position_3f[i][2];
    }


    glTranslatef(-(m_width/2)*factor,(m_height/2)*factor,-8.0);
    glLineWidth( 3.0 );
    glPointSize( 5.0 );


    GLfloat x1,y1,z1,x2,y2,z2;
//    //绘制横线
    glColor3f(1.0,0.0,0.0);
    for(i=0; i<(m_width*m_height); i++)
    {
        if((i+1)%m_width == 0)
            continue;
        x1 = arr_position_3f[i][0];
        y1 = arr_position_3f[i][1];
        z1 = arr_position_3f[i][2];
        x2 = arr_position_3f[i+1][0];
        y2 = arr_position_3f[i+1][1];
        z2 = arr_position_3f[i+1][2];

        glBegin(GL_LINES);
            glVertex3f(x1*factor, y1*factor, z1*factor);
            glVertex3f(x2*factor, y2*factor, z2*factor);
        glEnd();
    }

//    //绘制竖线
    for(i=0; i<(m_width*m_height); i++)
    {
        if(i+m_width>=(m_width*m_height))
            break;
        x1 = arr_position_3f[i][0];
        y1 = arr_position_3f[i][1];
        z1 = arr_position_3f[i][2];
        x2 = arr_position_3f[i+m_width][0];
        y2 = arr_position_3f[i+m_width][1];
        z2 = arr_position_3f[i+m_width][2];
        glBegin(GL_LINES);
            glVertex3f(x1*factor, y1*factor, z1*factor);
            glVertex3f(x2*factor, y2*factor, z2*factor);
        glEnd();
    }

//    //绘制顶点
    glColor3f(1.0,1.0,1.0);
    for(i=0; i<(m_width*m_height); i++)
    {
        x1 = arr_position_3f[i][0];
        y1 = arr_position_3f[i][1];
        z1 = arr_position_3f[i][2];
        glBegin(GL_POINTS);
            glVertex3f(x1*factor, y1*factor, z1*factor);
        glEnd();
    }
}

void openGLWidget::cal_fps()
{

    static  QString tmp="";
    static  QString cou="Counting...";
    static  QString itr="";
    static  QString tim="";
    static float framesPerSecond=0.0f;   // fps的数值
    static float frames    = 0.0f;       // 用于存储渲染的帧数
    //static int   iteration = 0;          // 用于存储迭代次数
    static float lastTime   = 0.0f;      // 前一秒的时刻
    float currentTime =  glutGet(GLUT_ELAPSED_TIME)* 0.001f;//程序运行的时间
    ++frames;
    ++iteration;
    time+=timeStep;
    if( currentTime - lastTime > 1.0f || frames >= 60)//，每秒刷新一次
    {
        framesPerSecond=frames;
        tmp.setNum(framesPerSecond);
        lastTime = currentTime;
        frames= 0;
    }
    if(tmp == "")
        renderText(100,100,"fps:  "+cou,QFont(tr("宋体"),20,10,false));
    else
        renderText(100,100,"fps:  "+tmp,QFont(tr("宋体"),20,10,false));

    itr.setNum(iteration);
    renderText(100,60,"its:  "+itr,QFont(tr("宋体"),20,10,false));
    tim.setNum(time);
    renderText(100,140,"time:  "+tim+"s",QFont(tr("宋体"),20,10,false));
}
