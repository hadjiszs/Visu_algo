/**
 * \file textbox.hpp
 * \brief Gère la text box
 */
#ifndef _TEXT_BOX
#define _TEXT_BOX

#include <iostream>

#include <QFont>
#include <QTextEdit>

/**
 * \class TextBox
 * \brief classe gerant l'affichage de texte durant l'execution
 */
class TextBox : public QTextEdit {
private:
     std::vector<std::string> _buffer;
     unsigned int             _cpt_ligne;
     unsigned int             _last_ligne;
     
public:
     /**
      * \brief Constructeur de la boite de texte
      * \details il s'agit de text box, on y affichera des informations durant l'execution
      * \param[in] parent le widget parent
      */
     TextBox(QWidget* parent = 0);

     /**
      * \brief Met à jour le contenu texte
      * \details est appelé de manière reguliere par le main thread
      */
     void afficher();

     /**
      * \brief Ajoute du texte
      * \details Ajoute du texte à la suite de la ligne
      * \param[in] str texte à ajouter
      * \param[in] num_ligne numero de la ligne
      */
     void ajouter(std::string str, int num_ligne);
     
     /**
      * \brief Remplace le texte
      * \details Remplace le texte passé en parametre
      * \param[in] str texte à remplacer
      * \param[in] num_ligne numero de la ligne
      * \param[in] npos position du premier caractere à remplacer
      */
     void remplacer(std::string str, int num_ligne, int npos);

     /**
      * \brief Retourne un numero d'une nouvelle ligne
      * \details Permet d'avoir un checkpoint vers une nouvelle ligne pour pouvoir modifier cette ligne grace au numero renvoyé
      * \param[out] numero de ligne
      */
     int newline();
     
     /**
      * \brief efface le buffer actuel
      */
     void clear();
};

#endif
