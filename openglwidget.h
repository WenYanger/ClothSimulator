#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include <QGLWidget>
#include <QtGui>
#include <QtOpenGL>
#include <glu.h>
#include <GLUT/glut.h>



class openGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    openGLWidget(QWidget *parent = 0);
    openGLWidget(int width, int height, QWidget *parent = 0);
    ~openGLWidget();

    void set_arr_position(GLfloat *);
    void set_timeStep(double);
    void paint();
    void loadGLTextures(QString);

    double time;
    int iteration;

private:
    //正方体在三个方向上的旋转
    GLfloat xRot, yRot, zRot;
    //纹理数组
    GLuint texture[2];
    //坐标数组
    GLfloat* arr_position;
    GLfloat** arr_position_3f;
    GLfloat*** arr_position_6f;
    //
    int m_width,m_height;
    double timeStep;//步长


protected:

    //设置渲染环境
    void initializeGL();
    //绘制窗口
    void paintGL();
    //响应窗口大小变化
    void resizeGL(int w, int h);
    //加载纹理函数
    void loadGLTextures();
    //绘制水平面
    void paintHorizontalLevel();
    //绘制网格
    void paintGrid();
    void paintGrid(GLfloat* arr_position);
    void paintGridOnly(GLfloat* arr_position);

    //显示fps
    void cal_fps();
};

#endif // OPENGLWIDGET_H
