/**
 * \file mainwindow.cpp
 * \brief Implementation de mainwindow.hpp
 */
#include "mainwindow.hpp"

#include <thread>
MainWindow::MainWindow(QWidget *parent)
     : QMainWindow(parent)
{
     setWindowTitle("Visualiseur");
     charger_menu();
     charger_opengl();

     _sidebar_widget = new QWidget(_central_widget);
     _central_layout->addWidget(_sidebar_widget, 0, 1);
     _sidebar_layout = new QVBoxLayout(_sidebar_widget);

     _text_box = new TextBox(_sidebar_widget);
     _sidebar_layout->addWidget(_text_box);

     _gl_update_timer = new QTimer(this);
     connect(_gl_update_timer, &QTimer::timeout, _gl_scene_context, [&]{ _gl_scene_context->update(); });
     _gl_update_timer->start(1000/60);

     _text_update_timer = new QTimer(this);
     connect(_text_update_timer, &QTimer::timeout, _text_box, [&]{ _text_box->afficher(); });
     _text_update_timer->start(1000/60);
}

void MainWindow::selection_graphe()
{
     QString path_fichier = QFileDialog::getOpenFileName(this,
                                                         "Sélectionner un graphe",
                                                         QString(),
                                                         QString(),
                                                         0,
                                                         QFileDialog::DontUseNativeDialog);

     std::cout << "Fin select [" << path_fichier.toStdString() << "]" << std::endl;

     if(_gl_scene_context != nullptr)
          delete _gl_scene_context;

     _text_box->clear();
     Graphe* structure = new Graphe(path_fichier.toStdString(), _text_box);
     structure->charger();

     _gl_scene_context = new SceneGL(_central_widget, structure);
     _central_layout->addWidget(_gl_scene_context, 0, 0);

     delete _gl_update_timer;
     _gl_update_timer = new QTimer(this);
     connect(_gl_update_timer, &QTimer::timeout, _gl_scene_context, [=]{ _gl_scene_context->update(); });
     _gl_update_timer->start(1000/60);

     std::thread algo(
          [=]
          {
               std::cout << "\ndebut thread lambda function -------------------" << std::endl;
               structure->parcours_profondeur();
               std::cout << "\nfin thread lambda function -------------------" << std::endl;
          });

     algo.detach();
}

inline void MainWindow::charger_menu()
{
     resize(1280, 720);
     _action_graphe = new QAction(this);
     _action_graphe->setObjectName(QString::fromUtf8("_action_graphe"));

     connect(_action_graphe, &QAction::triggered, this, &MainWindow::selection_graphe);
     _action_arbre = new QAction(this);
     _action_arbre->setObjectName(QString::fromUtf8("_action_arbre"));
     _action_options = new QAction(this);
     _action_options->setObjectName(QString::fromUtf8("_action_options"));
     _action_rotation = new QAction(this);
     _action_rotation->setObjectName(QString::fromUtf8("_action_rotation"));
     _action_rotation->setCheckable(true);
     _central_widget = new QWidget(this);
     _central_widget->setObjectName(QString::fromUtf8("_central_widget"));
     setCentralWidget(_central_widget);
     _menu_bar = new QMenuBar(this);
     _menu_bar->setObjectName(QString::fromUtf8("_menu_bar"));
     _menu_bar->setGeometry(QRect(0, 0, 800, 19));
     _menu_fichier = new QMenu(_menu_bar);
     _menu_fichier->setObjectName(QString::fromUtf8("_menu_fichier"));
     _menu_ouvrir = new QMenu(_menu_fichier);
     _menu_ouvrir->setObjectName(QString::fromUtf8("_menu_ouvrir"));
     _menu_outils = new QMenu(_menu_bar);
     _menu_outils->setObjectName(QString::fromUtf8("_menu_outils"));
     setMenuBar(_menu_bar);
     _status_bar = new QStatusBar(this);
     _status_bar->setObjectName(QString::fromUtf8("_status_bar"));
     setStatusBar(_status_bar);

     _menu_bar->addAction(_menu_fichier->menuAction());
     _menu_bar->addAction(_menu_outils->menuAction());
     _menu_fichier->addAction(_menu_ouvrir->menuAction());
     _menu_ouvrir->addAction(_action_graphe);
     _menu_ouvrir->addAction(_action_arbre);
     _menu_outils->addAction(_action_options);
     _menu_outils->addAction(_action_rotation);

     _action_graphe->setText("Graphe");
     _action_arbre->setText("Arbre");
     _action_options->setText("Options");
     _action_rotation->setText("Rotation");
     _menu_fichier->setTitle("Fichier");
     _menu_ouvrir->setTitle("Ouvrir");
     _menu_outils->setTitle("Outils");

     QMetaObject::connectSlotsByName(this);
}

inline void MainWindow::charger_opengl()
{
     _central_layout = new QGridLayout(_central_widget);
     // On cree le widget opengl qui va contenir la scene
     _gl_scene_context = new SceneGL(_central_widget);

     // On met le widget opengl en haut à gauche de la fenetre principale
     _central_layout->addWidget(_gl_scene_context, 0, 0);
}
