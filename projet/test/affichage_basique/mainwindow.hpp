/**
 * \file mainwindow.hpp
 * \brief Gère la fenetre principale
 */
#ifndef _MAINWINDOW_
#define _MAINWINDOW_

#include <QMainWindow>
#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QFileDialog>

#include <QtCore/QVariant>
#include <QAction>
#include <QDebug>
#include <QApplication>
#include <QButtonGroup>
#include <QHeaderView>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QWidget>

#include <QStringList>
#include <QFileDialog>

#include "scene.hpp"
#include "graphe.hpp"

/**
 * \class MainWindow
 * \brief classe gerant le placement et interactions des principaux widgets dans la fenetre principale
 */
class MainWindow : public QMainWindow {
private:
     QAction*           _action_graphe;         /*!< Action associé à la selection d'un graphe dans le menu */
     QAction*           _action_arbre;          /*!< Action associé à la selection d'un arbre dans le menu */
     QAction*           _action_options;        /*!< Action associé à la selection d'un options dans le menu */
     QAction*           _action_rotation;       /*!< Action associé à la selection d'un rotation dans le menu */

     QMenuBar*          _menu_bar;              /*!< Barre de menu */
     QStatusBar*        _status_bar;            /*!< Barre de status */
     QMenu*             _menu_fichier;          /*!< Menu fichier */
     QMenu*             _menu_ouvrir;           /*!< Menu ouvrir */
     QMenu*             _menu_outils;           /*!< Menu outils */

     QTimer*            _gl_update_timer;       /*!< Timer permettant de rafraichir la SceneGL à intervalle régulière */

     QWidget*           _central_widget;        /*!< Widget central */
     QGridLayout*       _central_layout;        /*!< Grille permettant d'agencer les principaux widget de la fenetre  */

     QPushButton*       _zoom_positiv;          /*!< Bouton pour zoomer */
     QPushButton*       _zoom_negativ;          /*!< Bouton pour dezoomer */

     QWidget*           _sidebar_widget;        /*!< Barre à gauche */
     QVBoxLayout*       _sidebar_layout;        /*!< Barre de widget agencé de manière ordonné verticalement */

     SceneGL*           _gl_scene_context;      /*!< Widget du contexte OpenGL */

public:
     /**
      * \brief Constructeur de la fenetre principale
      * \details il s'agit de la fenetre principale, elle n'a pas de parent, mais on laisse le parametre par commodité
      * \param[in] parent le widget parent
      */
     MainWindow(QWidget* parent = 0);

     /**
      * \brief Selection d'un graphe
      * \details Appelé lors de la reception d'un signal emis par l'utilisateur lorsqu'il demande
      * d'ouvrir un nouveau graphe dans le menu. Cette fonction ouvre une boite de dialogue
      * permettant de selectionner le graphe, puis va créer une structure de graphe à partir
      * du fichier sélectionné.
      */
     void selection_graphe();

     /**
      * \brief Chargeur du menu
      * \details Appelé lors de la construction de la fenetre principale, construit
      * la barre de menu, et associe les signaux aux actions
      */
     void charger_menu();

     /**
      * \brief Chargeur OpenGL
      * \details Appelé lors de la construction de la fenetre principale, cette fonction
      * appelle le constructeur de SceneGL créant le widget OpenGL qui
      * sera integré dans la fenetre principale
      */
     void charger_opengl();
};

#endif
