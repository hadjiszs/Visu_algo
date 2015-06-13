/**
 * \file scene.hpp
 * \brief Gère le contexte OpenGL
 */
#ifndef _SCENE_
#define _SCENE_

#include <QSet>
#include <QFile>
#include <QFileSystemWatcher>

#include <GL/glcorearb.h>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QVector4D>
#include <QVector3D>
#include <iostream>
#include <string>

#include "shader.hpp"
#include "structure.hpp"

/**
 * \class SceneGL
 * \brief classe gerant le contexte OpenGL de l'application
 */
class SceneGL : public QOpenGLWidget {
private:

     // Pour la resolution de contexte
     template<class ProcT>
     ProcT & getProc(ProcT &proc, const QByteArray &procName)
          {
               return proc = (ProcT) context()->getProcAddress(procName);
          }

     PFNGLCLEARPROC glClear;
     PFNGLVIEWPORTPROC glViewport;
     PFNGLENABLEPROC glEnable;
     PFNGLDRAWELEMENTSPROC glDrawElements;
     PFNGLISBUFFERPROC glIsBuffer;

     PFNGLGENBUFFERSPROC glGenBuffers;
     PFNGLDELETEBUFFERSPROC glDeleteBuffers;
     PFNGLBINDBUFFERPROC glBindBuffer;
     PFNGLBUFFERDATAPROC glBufferData;

     PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
     PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
     PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
     PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
     PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
     PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;

     PFNGLCREATESHADERPROC glCreateShader;
     PFNGLDELETESHADERPROC glDeleteShader;
     PFNGLSHADERSOURCEPROC glShaderSource;
     PFNGLCOMPILESHADERPROC glCompileShader;
     PFNGLGETSHADERIVPROC glGetShaderiv;
     PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;

     PFNGLCREATEPROGRAMPROC glCreateProgram;
     PFNGLDELETEPROGRAMPROC glDeleteProgram;
     PFNGLUSEPROGRAMPROC glUseProgram;
     PFNGLATTACHSHADERPROC glAttachShader;
     PFNGLLINKPROGRAMPROC glLinkProgram;
     PFNGLGETPROGRAMIVPROC glGetProgramiv;
     PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
     PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
     PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
     PFNGLUNIFORM4FVPROC glUniform4fv;
     PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
     PFNGLLINEWIDTHPROC glLineWidth;

     PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
     PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB;
     PFNGLDEBUGMESSAGECALLBACKAMDPROC glDebugMessageCallbackAMD;

     GLuint     _sceneVAO;
     GLuint     _sceneVBO;
     GLuint     _sceneEBO;

     // Shaders
     Shader*    _shader;

     // Structure à representer (graphe/arbre/...)
     Structure* _structure;

     GLsizei    _num_scene_indice;

     QVector4D  _teinture;

     float      _coef_rotation;

     bool       _aff_debug_notif;
     bool       _contenu_affichable;

     GLint      _position_loc;
     GLint      _color_loc;

     QVector3D  _cam_pos;

public:
     /**
      * \brief Constructeur de la scene GL
      * \details il s'agit de la fenetre principale, elle n'a pas de parent, mais on laisse le parametre par commodité
      * \param[in] vertex_shad          le vertex shaders
      * \param[in] fragment_shad        le fragment shaders
      * \param[in] parent               le widget parent où sera contenu le contexte OpenGL
      * \param[in] str                  la structure
      * \param[in] flags                flags de widget
      */
     SceneGL(std::string vertex_shad, std::string fragment_shad, QWidget* parent = 0, Structure* str = nullptr, Qt::WindowFlags f = 0);

     /**
      * \brief Destructeur
      * \details Destructeur
      */
     ~SceneGL();

     /**
      * \brief Initialise le contexte OpenGL
      * \details Cette fonction est appelé une fois avant le premier appel à paintGL ou à resizeGL
      */
     void initializeGL(); //override;

     /**
      * \brief Fonction de formattage
      * \details Cette fonction supprime les identifiants de shaders et vide les buffers
      */
     void cleanupGL();

     /**
      * \brief Fonction de redimensionnement
      * \details Cette fonction est appelé lorsque le widget conteneur du contexte OpenGL est redimensionné
      * \param[in] w          nouvelle dimension de largeur
      * \param[in] h          nouvelle dimension de hauteur
      */
     void resizeGL(int w, int h); //override;

     /**
      * \brief Fonction qui est appelé quand le widget a besoin d'etre repaint
      * \details Cette fonction est appelé une fois avant le premier appel à paintGL ou à resizeGL
      */
     void paintGL(); //override;

     /**
      * \brief Fonction de zoom avant de la camera
      * \details Fonction appelé lors d'un zoom arriere commandé par l'IHM
      */
     void Zoompos();

     /**
      * \brief Fonction de zoom arriere de la camera
      * \details Fonction appelé lors d'un zoom arriere commandé par l'IHM
      */
     void Zoomneg();

     /**
      * \brief Getters de la structure
      * \details Getters de la structure chargé dans la scene OpenGL courante
      */     
     Structure* get_structure();

     /**
      * \brief Charger contenu graphique
      * \details Charge le contenu graphique à afficher dans le contexte OpenGL
      */          
     void charger_contenu_graphique();

private:
     static void APIENTRY myGLDebugCallback(GLenum source, GLenum type, GLuint id,
                                            GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
          {
               // TODO: use these parameters to give a more descriptive message.
               /*
                 (void) source;
                 (void) type;
                 (void) id;
                 (void) length;
                 SceneGL *widget = (SceneGL*) userParam;
                 if (severity == GL_DEBUG_SEVERITY_NOTIFICATION && !widget->mShowDebugNotifications)
                 {
                 // These can get spammy.
                 return;
                 }
               */
               qWarning("GL debug: %s", message);
          }
     static void APIENTRY myGLDebugCallbackAMD(GLuint id, GLenum category,
                                               GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
          {
               /*
               // TODO: use these parameters to give a more descriptive message.
               (void) id;
               (void) category;
               (void) severity;
               (void) length;
               SceneGL *widget = (SceneGL*) userParam;
               */
               qWarning("GL debug: %s", message);
          }
};

#endif
