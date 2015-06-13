/**
 * \file main.cpp
 * \brief Programme principale
 */
#include <QApplication>

#include <QSet>
#include <QFile>
#include <QFileSystemWatcher>

#include <vector>

#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
     QApplication app(argc, argv);

     MainWindow myMainWindow;
     myMainWindow.show();

     return app.exec();
}
