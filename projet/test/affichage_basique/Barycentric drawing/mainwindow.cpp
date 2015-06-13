#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent)
     : QMainWindow(parent)
{
     QWidget *centralWidget = new QWidget(this);
     QGridLayout *centralLayout = new QGridLayout(centralWidget);
     setCentralWidget(centralWidget);

     SceneGL *myGLWidget = new SceneGL(centralWidget);

     QSurfaceFormat myGLFormat;
     myGLFormat.setSamples(16);
     myGLFormat.setVersion(3,3);
     myGLFormat.setProfile(QSurfaceFormat::CoreProfile);
#ifndef NDEBUG
     myGLFormat.setOption(QSurfaceFormat::DebugContext);
#endif
     myGLWidget->setFormat(myGLFormat);

     myGLWidget->setMinimumSize(640, 480);
     centralLayout->addWidget(myGLWidget, 0, 0);

     QWidget *sidebarWidget = new QWidget(centralWidget);
     centralLayout->addWidget(sidebarWidget, 0, 1);
     QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebarWidget);

     QPushButton *noTintButton = new QPushButton(tr("Disable tint"), sidebarWidget);
     connect(noTintButton, &QPushButton::pressed, myGLWidget, &SceneGL::disableTint);
     sidebarLayout->addWidget(noTintButton);

     QPushButton *warmTintButton = new QPushButton(tr("Use warm tint"), sidebarWidget);
     connect(warmTintButton, &QPushButton::pressed, myGLWidget, &SceneGL::setWarmTint);
     sidebarLayout->addWidget(warmTintButton);

     QPushButton *coldTintButton = new QPushButton(tr("Use cold tint"), sidebarWidget);
     connect(coldTintButton, &QPushButton::pressed, myGLWidget, &SceneGL::setColdTint);
     sidebarLayout->addWidget(coldTintButton);

     QTimer *myGLUpdateTimer = new QTimer(this);
     connect(myGLUpdateTimer, &QTimer::timeout, myGLWidget, [=]{ myGLWidget->repaint(); });
     myGLUpdateTimer->start(1000/60);

     setWindowTitle("QtGLTest");
}
