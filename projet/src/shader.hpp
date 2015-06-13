/**
 * \file shader.hpp
 * \brief Gère les shaders
 */
#ifndef _SHADER_
#define _SHADER_

#ifdef WIN32
        #include <GL/glew.h>
#else
        #define GL3_PROTOTYPES 1
        #include <GL3/gl3.h>
#endif

#include <string>
#include <QOpenGLFunctions>

/**
 * \class Shader
 * \brief Classe gerant la compilation et verrouillage du vertex et fragment shaders
 */
class Shader {
private:
     const GLchar*	_vertex_source;
     const GLchar*	_fragme_source;

     GLint		_id_position;
     GLint		_id_texture;
     GLint		_id_normal;
     GLint		_id_vertex_shader;
     GLint		_id_fragme_shader;
     GLuint		_id_shader_program;

     GLint		_vertex_source_length;
     GLint		_fragme_source_length;

     QOpenGLFunctions*  _resolve_fonctions;

     bool               _gestion_text;

public:
     /**
      * \brief Constructeur shader
      * \details Constructeur shaders
      * \param[in] ver_shad     le vertex shaders
      * \param[in] fra_shad     le fragment shaders
      * \param[in] fra_shad     gestion du texte ? Si oui alors on gere les textures sinon on gere les normals pour la lumiere/ombre
      */
     Shader(std::string ver_shad, std::string fra_shad, bool text);
     
     /**
      * \brief Destructeur shader
      * \details Destructeur shader
      */
     ~Shader();

     /**
      * \brief Fonction de formattage
      * \details Fonction de formattage
      */
     void del();

     /**
      * \brief Fonction de chargement
      * \details Compile le vertex et fragment shaders, puis fait l'edition des liens des deux shaders
      * \param[in] function_contexte    resolver de fonctions
      */
     void charger(QOpenGLFunctions* function_contexte);

     /**
      * \brief Fonction de verification de la compilation
      * \details Verifie les flag de status de compilation de chacun des shaders
      * \param[in] id_shader    identifiant du shader dont on verifie sa compilation
      * \param[in] nom_shader   nom du shader dont on verifie sa compilation
      */
     bool verif_compil_shader(GLint id_shader, std::string nom_shader);

     /**
      * \brief Fonction de verification de l'edition des liens
      * \details Verifie les flag de status d'edition des liens de chacun des shaders
      * \param[in] id_program    identifiant du programme resultant de l'edition des liens du vertex et fragment shaders
      * \param[in] nom           nom du programme resultant de l'edition des liens du vertex et fragment shaders
      */
     bool verif_link_shader(GLint id_program, std::string nom);

     //
     // Getters
     //
     GLint get_id_position();
     GLint get_id_texture();
     GLint get_id_normal();
     GLint get_id_shader_program();
     GLint get_id_vertex_shader();
     GLint get_id_fragme_shader();

     GLint get_vertex_source_length();
     GLint get_fragme_source_length();

     const GLchar* get_vertex_source();
     const GLchar* get_fragme_source();

     //
     // Setters
     //
     void set_id_position(GLint);
     void set_id_texture(GLint);
     void set_id_normal(GLint);
     void set_id_shader_program(GLint);
     void set_id_vertex_shader(GLint);
     void set_id_fragme_shader(GLint);
};

#endif
