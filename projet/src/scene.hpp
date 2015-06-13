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
#include <QPainter>
#include <QPixmap>
#include <QImage>
#include <QVector4D>
#include <QVector3D>
#include <QMouseEvent>
#include <QPoint>
#include <iostream>
#include <string>

#include "shader.hpp"
#include "structure.hpp"

struct DataNode
{
     // 3d vector representant la couleur de la lumiere diffuse associé à ce type de noeud
     QVector3D  _lumiere_diffuse;

     // Vecteur de paire <VAO|Geometry>
     std::vector< std::pair<GLuint,ColGeom> > _list_vao_mesh;

     // Font associé
     QFont::StyleHint   _style_font;
     QColor             _color_font;
};

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
     PFNGLBUFFERSUBDATAPROC glBufferSubData;

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
     PFNGLUNIFORM1IPROC glUniform1i;
     PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
     PFNGLLINEWIDTHPROC glLineWidth;

     PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
     PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB;
     PFNGLDEBUGMESSAGECALLBACKAMDPROC glDebugMessageCallbackAMD;

     PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
     PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;
     PFNGLBINDBUFFERRANGEPROC glBindBufferRange;

     GLuint     _arete_VAO;
     GLuint     _arete_VBO;
     GLuint     _arete_EBO;

     GLuint     _texte_ID;
     GLuint     _texte_VBO;
     GLuint     _texte_VAO;

     QImage*    _texture_img;

     GLuint *vaos, *vbos;              // OpenGL vertex objects
     GLuint ubo;                       // OpenGL uniform buffer object

     // Shaders
     Shader*    _shader_normal;
     Shader*    _shader_texture;

     // Structure à representer (graphe/arbre/...)
     Structure* _structure;

     GLsizei    _num_scene_indice;

     QVector4D  _teinture;

     float      _coef_rotation;

     bool       _aff_debug_notif;
     bool       _contenu_affichable;

     //Camera     _cam_pos;
     QVector3D  _cam_pos;

     // Etat du noeud associé à plusieurs données (3D, couleur, font, ...)
     std::map<Etat::Noeud, DataNode> _etat_to_data;

public:
     /**
      * \brief Constructeur de la scene GL
      * \details il s'agit de la fenetre principale, elle n'a pas de parent, mais on laisse le parametre par commodité
      * \param[in] parent               le widget parent où sera contenu le contexte OpenGL
      * \param[in] str                  la structure
      * \param[in] flags                flags de widget
      */
     SceneGL(QWidget* parent = 0, Structure* str = nullptr, Qt::WindowFlags f = 0);

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
      * \brief Fonction qui est appelé quand le widget a besoin d'etre repaint
      * \details Cette fonction est appelé une fois avant le premier appel à paintGL ou à resizeGL
      */
     void paintGL();  //override;

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
      * \brief Getters de la structure
      * \details Getters de la structure chargé dans la scene OpenGL courante
      */
     Structure* get_structure();

     /**
      * \brief Charger contenu graphique
      * \details Charge le contenu graphique à afficher dans le contexte OpenGL
      */
     void charger_contenu_graphique();

     // Gestion camera
     
     /**
      * \brief handler evenement clique souris
      * \details handler appelé lors d'un clique sur le widget courant
      */
     void mousePressEvent(QMouseEvent * event) override;

     /**
      * \brief handler evenement mouvement souris
      * \details handler appelé lors d'un mouvement de la souris sur le widget courant
      */
     void mouseMoveEvent(QMouseEvent * event) override;

     /**
      * \brief handler evenement scroll souris
      * \details handler appelé lors d'un scroll de la souris sur le widget courant
      */     
     void wheelEvent(QWheelEvent * event) override;
     
private:
     QPainter* painter;
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
