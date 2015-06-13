/**
 * \file scene.cpp
 * \brief Implementation de scene.hpp
 */
#define GLM_FORCE_RADIANS

// Si on est sur windows, on lie statiquement
#ifdef WIN32
#   include "glm/glm.hpp"
#   include "glm/gtx/transform.hpp"
#   include "glm/gtc/type_ptr.hpp"
#else
#   include <glm/glm.hpp>
#   include <glm/gtx/transform.hpp>
#   include <glm/gtc/type_ptr.hpp>
#endif

#include "scene.hpp"

SceneGL::SceneGL(std::string vertex_shad, std::string fragment_shad, QWidget* parent, Structure* str, Qt::WindowFlags f)
     : QOpenGLWidget(parent, f)
{
     _sceneVAO = 0;
     _sceneVBO = 0;
     _sceneEBO = 0;

     // QSurfaceFormat va permettre de spécifier les parametres
     // du context opengl
     QSurfaceFormat format_gl;
     
     format_gl.setSamples(16);
     format_gl.setVersion(3, 3);
     format_gl.setProfile(QSurfaceFormat::CoreProfile);
     format_gl.setOption(QSurfaceFormat::DebugContext);

     setFormat(format_gl);
     setMinimumSize(1280, 720);

     _contenu_affichable = false;

     _cam_pos = QVector3D(1.0, 2.0, 3.0);

     _shader    = new Shader(vertex_shad, fragment_shad);
     _structure = str;

     _aff_debug_notif = true;
     _teinture = QVector4D(0.0f, 0.0f, 0.0f, 0.0f);

     _coef_rotation  = 0;
}

void SceneGL::initializeGL()
{
     QPair<int,int> myGLVersion = context()->format().version();
     QSet<QByteArray> myGLExtensions = context()->extensions();
     std::cout << "OpenGL Version: " << myGLVersion.first << "." << myGLVersion.second << std::endl;

     getProc(glClear, "glClear");
     getProc(glViewport, "glViewport");
     getProc(glEnable, "glEnable");
     getProc(glDrawElements, "glDrawElements");
     getProc(glIsBuffer, "glIsBuffer");

     getProc(glGenBuffers, "glGenBuffers");
     getProc(glDeleteBuffers, "glDeleteBuffers");
     getProc(glBindBuffer, "glBindBuffer");
     getProc(glBufferData, "glBufferData");

     getProc(glGenVertexArrays, "glGenVertexArrays");
     getProc(glDeleteVertexArrays, "glDeleteVertexArrays");
     getProc(glBindVertexArray, "glBindVertexArray");
     getProc(glVertexAttribPointer, "glVertexAttribPointer");
     getProc(glEnableVertexAttribArray, "glEnableVertexAttribArray");
     getProc(glDisableVertexAttribArray, "glDisableVertexAttribArray");

     getProc(glCreateShader, "glCreateShader");
     getProc(glDeleteShader, "glDeleteShader");
     getProc(glShaderSource, "glShaderSource");
     getProc(glCompileShader, "glCompileShader");
     getProc(glGetShaderiv, "glGetShaderiv");
     getProc(glGetShaderInfoLog, "glGetShaderInfoLog");
     getProc(glLineWidth, "glLineWidth");

     getProc(glCreateProgram, "glCreateProgram");
     getProc(glDeleteProgram, "glDeleteProgram");
     getProc(glUseProgram, "glUseProgram");
     getProc(glAttachShader, "glAttachShader");
     getProc(glLinkProgram, "glLinkProgram");
     getProc(glGetProgramiv, "glGetProgramiv");
     getProc(glGetProgramInfoLog, "glGetProgramInfoLog");
     getProc(glGetAttribLocation, "glGetAttribLocation");
     getProc(glGetUniformLocation, "glGetUniformLocation");
     getProc(glUniform4fv, "glUniform4fv");
     getProc(glUniformMatrix4fv, "glUniformMatrix4fv");

     if(context()->format().options() & QSurfaceFormat::DebugContext)
     {
          if ((myGLVersion.first > 4 || (myGLVersion.first == 4 && myGLVersion.second >= 3)) ||
              myGLExtensions.contains("GL_KHR_debug"))
          {
               getProc(glDebugMessageCallback, "glDebugMessageCallback");
               glDebugMessageCallback(&SceneGL::myGLDebugCallback, this);
          }
          else if (myGLExtensions.contains("GL_ARB_debug_output"))
          {
               getProc(glDebugMessageCallbackARB, "glDebugMessageCallbackARB");
               glDebugMessageCallbackARB(&SceneGL::myGLDebugCallback, this);
          }
          else if (myGLExtensions.contains("GL_AMD_debug_output"))
          {
               getProc(glDebugMessageCallbackAMD, "glDebugMessageCallbackAMD");
               glDebugMessageCallbackAMD(&SceneGL::myGLDebugCallbackAMD, this);
          }
     }

     if(_shader)
          // On charge les shaders en envoyant le contexte opengl actuel
          _shader->charger(context()->functions());
     else
          std::cout << "aucun shader chargé en mémoire" << std::endl;

     charger_contenu_graphique();

     connect(context(), &QOpenGLContext::destroyed, this, &SceneGL::cleanupGL);
}

// charge le contenu graphique de la structure courante
void SceneGL::charger_contenu_graphique()
{
     if(_structure == nullptr)
          return;

     std::cout << "\nChargement contenu graphique" << std::endl;

     glGenVertexArrays(1, &_sceneVAO);

     glGenBuffers(1, &_sceneVBO);
     glBindBuffer(GL_ARRAY_BUFFER, _sceneVBO);
     glBufferData(GL_ARRAY_BUFFER, _structure->get_vertices()->size() * sizeof(_structure->get_vertices()[0]), _structure->get_vertices()->data(), GL_STATIC_DRAW);

     glGenBuffers(1, &_sceneEBO);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _sceneEBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, _structure->get_indices()->size() * sizeof(_structure->get_indices()[0]), _structure->get_indices()->data(), GL_STATIC_DRAW);

     _num_scene_indice = _structure->get_indices()->size();

     // ------------
     // load shaders
     _position_loc = _shader->get_id_position();
     _color_loc    = _shader->get_id_color();

     glBindVertexArray(_sceneVAO);

     glBindBuffer(GL_ARRAY_BUFFER, _sceneVBO);
     glVertexAttribPointer(_position_loc, SceneVertex::PositionSize, GL_FLOAT, GL_FALSE, sizeof(SceneVertex), (GLvoid*) offsetof(SceneVertex, Position));
     glVertexAttribPointer(_color_loc, SceneVertex::ColorSize, GL_FLOAT, GL_FALSE, sizeof(SceneVertex), (GLvoid*) offsetof(SceneVertex, Color));

     glEnableVertexAttribArray(_position_loc);
     glEnableVertexAttribArray(_color_loc);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _sceneEBO);

     glBindVertexArray(_sceneVAO);
     // fin load shaders
     // ----------------

     // -----------
     // message log
     std::cout << "\nFin chargement contenu graphique"
               << " _sceneVAO[" << _sceneVAO
               << "] ; Shad[" << _shader->get_id_shader_program()
               << "] ; _sceneVBO[" << _sceneVBO
               << "] ; _sceneEBO[" << _sceneEBO
               << "] ; Pos[" << _position_loc << "]" << "|Col[" << _color_loc
               << "]" << std::endl;

     if(glIsBuffer(_sceneVBO) == GL_TRUE)
          std::cout << "\nVBO[" << _sceneVBO << "] [OOK]";
     else
          std::cout << "\nVBO[" << _sceneVBO << "] [KKO]";

     if(glIsBuffer(_sceneEBO) == GL_TRUE)
          std::cout << "\nEBO[" << _sceneEBO << "] [OOK]\n";
     else
          std::cout << "\nEBO[" << _sceneEBO << "] [KKO]\n";
     // message log
     // -----------

     glLineWidth(1.0f); // ratio dans l'intervalle [0;1]

     _contenu_affichable = true;
}

void SceneGL::cleanupGL()
{
     glBindVertexArray(0);
     std::cout << "Suppression de _sceneEBO[" << _sceneEBO << "]" << std::endl;
     glDeleteBuffers(1, &_sceneEBO);

     std::cout << "Suppression de _sceneVBO[" << _sceneVBO << "]" << std::endl;
     glDeleteBuffers(1, &_sceneVBO);

     std::cout << "Suppression de _sceneVAO[" << _sceneVAO << "]" << std::endl;
     glDeleteVertexArrays(1, &_sceneVAO);

     glDeleteShader(_shader->get_id_fragme_shader());
     glDeleteShader(_shader->get_id_vertex_shader());
     glDeleteProgram(_shader->get_id_shader_program());

     _sceneEBO = 0;
     _sceneVBO = 0;
     _sceneVAO = 0;

     _shader->set_id_fragme_shader(0);
     _shader->set_id_vertex_shader(0);
     _shader->set_id_shader_program(0);
}

void SceneGL::resizeGL(int w, int h)
{
     glViewport(0, 0, w, h);
}

void SceneGL::paintGL()
{
     if( ! _contenu_affichable)
          return;

     // Gestion camera
     glm::mat4 projection = glm::perspective(70.0f, float(640/480), 0.1f, 100.0f);
     glm::mat4 modelview  = glm::mat4(1.0);
     glm::mat4 view       = glm::lookAt(glm::vec3(_cam_pos.x(), _cam_pos.y(), _cam_pos.z()),
                                        glm::vec3(0.0, 0.0, 0.0),
                                        glm::vec3(0.0, 1.0, 0.0));

     //_coef_rotation += 0.5;
     modelview = glm::rotate(modelview, float(_coef_rotation*3.1415926535/180.0), glm::vec3(0.0, 1.0, 0.0));

     glm::mat4 mvp = projection * view * modelview;

     glEnable(GL_DEPTH_TEST);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     glUseProgram(_shader->get_id_shader_program());

     // Vecteur modifiant la teinture des couleurs des pixels
     glUniform4fv(glGetUniformLocation(_shader->get_id_shader_program(), "Tint"), 1, &_teinture[0]);

     // Matrice de Modelview/View/Projection
     glUniformMatrix4fv(glGetUniformLocation(_shader->get_id_shader_program(), "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

     glBindVertexArray(_sceneVAO);
     
     // On dessine les elements
#ifdef AFFDEBUG
       std::cout << "affichage _sceneVAO[" << _sceneVAO
       << "] ; Shad[" << _shader->get_id_shader_program()
       << "] ; _sceneVBO[" << _sceneVBO
       << "] ; _sceneEBO[" << _sceneEBO
       << "] ; Pos[" << _position_loc << "]" << "|Col[" << _color_loc
       << "]" << std::endl;
#endif
       
     glDrawElements(GL_LINES, _num_scene_indice, GL_UNSIGNED_INT, NULL);
}

Structure* SceneGL::get_structure()
{
     return _structure;
}

void SceneGL::Zoompos()
{
     _cam_pos *= 0.9;
     std::cout << "X: " << _cam_pos.x() << " Y: " << _cam_pos.y() << " Z: " << _cam_pos.z() << std::endl;
}

void SceneGL::Zoomneg()
{
     _cam_pos *= 1.1;
     std::cout << "X: " << _cam_pos.x() << " Y: " << _cam_pos.y() << " Z: " << _cam_pos.z() << std::endl;
}

SceneGL::~SceneGL()
{
     if(_shader)
          delete _shader;

     if(_structure)
          delete _structure;
}
