#ifndef _MAINWINDOW_
#define _MAINWINDOW_

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>

#include "scene.hpp"

class MainWindow : public QMainWindow {
public:
     MainWindow(QWidget *parent = 0);
};

#endif
