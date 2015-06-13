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

#include <iostream>
#include <string>
#include <functional>
#include "scene.hpp"

#define AFFDEBUG 1

struct param_lux {
     glm::vec4 diffuse_intensity;
     glm::vec4 ambient_intensity;
     glm::vec4 light_direction;
};

#define WARN(MSG) printf("\n\033[41;01m"                                \
                         "[_POSITION_]\t[%s:%d] -> [%s]\n"              \
                         "[_DEBUG_MSG_]\t[%s]"                          \
                         "\033[00m\n", __FILE__, __LINE__, __func__, MSG);

#define ISOK(MSG) printf("\n\033[42;01m"                                \
                         "[_POSITION_]\t[%s:%d] -> [%s]\n"              \
                         "[_DEBUG_MSG_]\t[%s]"                          \
                         "\033[00m\n", __FILE__, __LINE__, __func__, MSG);

double angleY = 0.0;
double angleZ = 0.0;

int old_posx = 0;
int old_posy = 0;

bool _hold = false;

SceneGL::SceneGL(QWidget* parent, Structure* str, Qt::WindowFlags f)
     : QOpenGLWidget(parent, f)
{
     _arete_VAO = 0;
     _arete_VBO = 0;
     _arete_EBO = 0;

     // QSurfaceFormat va permettre de spécifier les parametres
     // du context opengl
     QSurfaceFormat format_gl;

     format_gl.setSamples(16);
     format_gl.setVersion(3, 3);
     format_gl.setOption(QSurfaceFormat::DebugContext);
     //format_gl.setProfile(QSurfaceFormat::CoreProfile);
     format_gl.setRenderableType(QSurfaceFormat::OpenGL);

     setFormat(format_gl);
     setMinimumSize(1280, 720);

     // Prise en charge manuelle de l'arriere plan
     setAttribute(Qt::WA_OpaquePaintEvent);
     setAutoFillBackground(false);

     _contenu_affichable = false;

     _cam_pos = QVector3D(0.0, 0.0, 2.0);

     // Pour le contenu de la scene (noeuds/arete/etc)
     _shader_normal  = new Shader("shaders/normal.vert", "shaders/normal.frag", false);
     // Pour le texte (label/valeur/etc)
     _shader_texture = new Shader("shaders/texture.vert", "shaders/texture.frag", true);
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
     getProc(glBufferSubData, "glBufferSubData");

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
     getProc(glUniform1i, "glUniform1i");
     getProc(glUniformMatrix4fv, "glUniformMatrix4fv");

     getProc(glGetUniformBlockIndex, "glGetUniformBlockIndex");
     getProc(glUniformBlockBinding, "glUniformBlockBinding");
     getProc(glBindBufferRange, "glBindBufferRange");

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

     if(_shader_normal)
          _shader_normal->charger(context()->functions());
     else
          std::cout << "aucun shader pour gestion des normales" << std::endl;

     if(_shader_texture)
          _shader_texture->charger(context()->functions());
     else
          std::cout << "aucun shader pour gestion des textures (pour affichage texte)" << std::endl;

     charger_contenu_graphique();

     connect(context(), &QOpenGLContext::destroyed, this, &SceneGL::cleanupGL);
}

// charge le contenu graphique de la structure courante
void SceneGL::charger_contenu_graphique()
{
     if(_structure == nullptr)
          return;

     std::cout << "\nChargement contenu graphique" << std::endl;

     //
     // Creat/Load/Init VAO pour chaque type 3D
     //
     glUseProgram(_shader_normal->get_id_shader_program());

     // Lambda function permettant d'associer un modele 3D pour un type de noeud
     std::function<void (DataNode, Etat::Noeud, std::string, glm::vec3, QColor, QFont::StyleHint)>
          init_nodetype =
          [&](DataNode datanode, Etat::Noeud typenode, std::string path_model, glm::vec3 diffuse_color, QColor colorfont, QFont::StyleHint stylefont)
          {
               std::vector<ColGeom> geom_vec;
               ColladaInterface::readGeometries(&geom_vec, path_model.c_str());

               // On cree un VAO pour chaque geometry
               vaos = new GLuint[geom_vec.size()];
               glGenVertexArrays(geom_vec.size(), vaos);

               // On cree deux VBO pour chaque geometry (1 pour position; 1 pour normals)
               vbos = new GLuint[2 * geom_vec.size()];
               glGenBuffers(2 * geom_vec.size(), vbos);

               // Traitement pour chaque geometry
               for(unsigned int i = 0; i < geom_vec.size(); i++)
               {
                    // Verrouillage du VAO courant
                    glBindVertexArray(vaos[i]);

                    // -- Position
                    // Verrouillage du VBO
                    glBindBuffer(GL_ARRAY_BUFFER, vbos[2*i]);
                    // Allocation + initialisation des données dans la VRAM
                    glBufferData(GL_ARRAY_BUFFER, geom_vec[i].map["POSITION"].size,
                                 geom_vec[i].map["POSITION"].data, GL_STATIC_DRAW);

                    // Spécifie la position des données de position dans la VRAM
                    glVertexAttribPointer(_shader_normal->get_id_position(), geom_vec[i].map["POSITION"].stride,
                                          geom_vec[i].map["POSITION"].type, GL_FALSE, 0, 0);
                    glEnableVertexAttribArray(_shader_normal->get_id_position());

                    // -- Normals
                    // Verrouillage du VBO
                    glBindBuffer(GL_ARRAY_BUFFER, vbos[2*i+1]);
                    // Allocation + initialisation des données dans la VRAM
                    glBufferData(GL_ARRAY_BUFFER, geom_vec[i].map["NORMAL"].size,
                                 geom_vec[i].map["NORMAL"].data, GL_STATIC_DRAW);

                    // Spécifie la position des données de position dans la VRAM
                    glVertexAttribPointer(_shader_normal->get_id_normal(), geom_vec[i].map["NORMAL"].stride,
                                          geom_vec[i].map["NORMAL"].type, GL_FALSE, 0, 0);
                    glEnableVertexAttribArray(_shader_normal->get_id_normal());

                    // Ajout de la paire <VAO|Geometry> courante
                    datanode._list_vao_mesh.push_back(std::make_pair(vaos[i], geom_vec[i]));
               }

               datanode._lumiere_diffuse = QVector3D(diffuse_color.x,
                                                     diffuse_color.y,
                                                     diffuse_color.z);

               datanode._style_font = stylefont;
               datanode._color_font = colorfont;

               _etat_to_data[typenode] = datanode;
          };

     // [DATANODE] ; [ETAT NOEUD] ; [MODEL 3D] ; [COULEUR] ; [FONT] ; [COLOR]
     DataNode datanode;
     init_nodetype(datanode, Etat::Noeud::SIMPLE, "../data/sphere.dae", glm::vec3(0.45, 0.45, 0.45),
                   Qt::white, QFont::Monospace);

     init_nodetype(datanode, Etat::Noeud::COURANT, "../data/square.dae", glm::vec3(0.8, 0.0, 0.0),
                   Qt::white, QFont::Monospace);

     init_nodetype(datanode, Etat::Noeud::VISITED, "../data/sphere.dae", glm::vec3(0.0, 0.0, 0.8),
                   Qt::green, QFont::Monospace);

     init_nodetype(datanode, Etat::Noeud::SELECT, "../data/sphere.dae", glm::vec3(0.0, 0.0, 0.8),
                   Qt::green, QFont::Monospace);

     init_nodetype(datanode, Etat::Noeud::CHOIX, "../data/sphere.dae", glm::vec3(0.0, 0.0, 0.8),
                   Qt::yellow, QFont::Monospace);

     glBindBuffer(GL_ARRAY_BUFFER, 0);
     glBindVertexArray(0);

     //
     // Aretes
     //
     glUseProgram(_shader_normal->get_id_shader_program());

     // Creation/Allocation/Initialisation VBO pour les arêtes du graphe (static durant l'execution)
     glGenVertexArrays(1, &_arete_VAO);
     glGenBuffers(1, &_arete_VBO);
     glBindBuffer(GL_ARRAY_BUFFER, _arete_VBO);
     glBufferData(GL_ARRAY_BUFFER, _structure->get_vertices()->size() * sizeof(_structure->get_vertices()[0]), _structure->get_vertices()->data(), GL_STATIC_DRAW);
     glGenBuffers(1, &_arete_EBO);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _arete_EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, _structure->get_indices()->size() * sizeof(_structure->get_indices()[0]), _structure->get_indices()->data(), GL_STATIC_DRAW);
     _num_scene_indice = _structure->get_indices()->size();

     glBindVertexArray(_arete_VAO);
     glBindBuffer(GL_ARRAY_BUFFER, _arete_VBO);
     glVertexAttribPointer(_shader_normal->get_id_position(), SceneVertex::PositionSize, GL_FLOAT, GL_FALSE, sizeof(SceneVertex), (GLvoid*) offsetof(SceneVertex, Position));
     glVertexAttribPointer(_shader_normal->get_id_normal(), SceneVertex::NormalSize, GL_FLOAT, GL_FALSE, sizeof(SceneVertex), (GLvoid*) offsetof(SceneVertex, Normal));
     glEnableVertexAttribArray(_shader_normal->get_id_position());
     glEnableVertexAttribArray(_shader_normal->get_id_normal());
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _arete_EBO);
     glBindVertexArray(_arete_VAO);

     //
     // Uniform shaders
     //
     glUseProgram(_shader_normal->get_id_shader_program());

     // Initialisation des données uniform (pour shaders)
     GLuint block_index, ubo_index;
     struct param_lux params;

     // Modification du jeu de lumière (phong)
     glm::vec4 diff_color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
     GLint location = glGetUniformLocation(_shader_normal->get_id_shader_program(), "diffuse_color");
     glUniform4fv(location, 1, &(diff_color[0]));

     // Initialise UBO
     params.diffuse_intensity = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
     params.ambient_intensity = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
     params.light_direction   = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

     // Creation/Allocation/Initialisation de l'UBO
     glGenBuffers(1, &ubo);
     glBindBuffer(GL_UNIFORM_BUFFER, ubo);
     glBufferData(GL_UNIFORM_BUFFER, 3*sizeof(glm::vec4), &params, GL_STREAM_DRAW);
     glBindBuffer(GL_UNIFORM_BUFFER, 0);

     // Connexion entre UBO et GLSL
     ubo_index = 0;
     block_index = glGetUniformBlockIndex(_shader_normal->get_id_fragme_shader(), "param_lux");

     // FIXME: pourquoi ça retourne INVALID_OPERATION(== 4294967295) ? (non compatible avec compute shader ?)
     glUniformBlockBinding(_shader_normal->get_id_shader_program(), block_index, ubo_index);
     std::cout << "\nblock_index[" << block_index << "]" << std::endl;

     glBindBufferRange(GL_UNIFORM_BUFFER, ubo_index, ubo, 0, 3*sizeof(glm::vec4));

     glLineWidth(1.0f); // ratio dans l'intervalle [0;1]

     //
     // Texte
     //
     // Creation de l'image
     _texture_img = new QImage(64, 64, QImage::Format_ARGB32);
     _texture_img->fill(0);

     QFont font;
     font.setStyleHint(QFont::Monospace);
     font.setBold(true);
     font.setPixelSize(20);

     QPainter painter;
     painter.begin(_texture_img);
     painter.setPen(Qt::white);
     painter.setFont(font);
     painter.drawText(0, 0, 60, 60, Qt::AlignCenter, "5");
     painter.end();

     _texture_img->save("_texture_img.png", "PNG");
     *_texture_img = _texture_img->mirrored(false, true);

     // chargement texture à partir de l'image correspondant à du texte
     glUseProgram(_shader_texture->get_id_shader_program());
     glGenTextures(1, &_texte_ID);

     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, _texte_ID);

     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _texture_img->width(), _texture_img->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _texture_img->bits());
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glUniform1i(glGetUniformLocation(_shader_texture->get_id_shader_program(), "tex"), 0);

     // VBO|VAO pour le carré qui va servir de panneau texturé pour les labels/texte à afficher
     glGenBuffers(1, &_texte_VBO);
     glBindBuffer(GL_ARRAY_BUFFER, _texte_VBO);

     // Vertices du carré
     GLfloat vertices[] = {-1.0f, -1.0f, 0.0f,   1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   // Triangle 1
                           -1.0f, -1.0f, 0.0f,   -1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f};  // Triangle 2
     int nb_vertices = 18;
     GLfloat coordTexture[] = {0.0f, 0.0f,   1.0f, 0.0f,   1.0f, 1.0f,     // Triangle 1
                               0.0f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f};    // Triangle 2
     int nb_uv_map = 12;

     glBufferData(GL_ARRAY_BUFFER, nb_vertices*sizeof(GLfloat) + nb_uv_map*sizeof(GLfloat), 0, GL_STATIC_DRAW);

     glBufferSubData(GL_ARRAY_BUFFER, 0, nb_vertices*sizeof(GLfloat), vertices);
     glBufferSubData(GL_ARRAY_BUFFER, nb_vertices*sizeof(GLfloat), nb_uv_map*sizeof(GLfloat), coordTexture);

     glGenVertexArrays(1, &_texte_VAO);
     glBindVertexArray(_texte_VAO);

     glUseProgram(_shader_texture->get_id_shader_program());
     glBindBuffer(GL_ARRAY_BUFFER, _texte_VBO);
     // Spécification et envoie des variables et données
     glVertexAttribPointer(_shader_texture->get_id_position(), 3, GL_FLOAT, GL_FALSE, 0, (char*)NULL + 0);
     glEnableVertexAttribArray(_shader_texture->get_id_position());

     glVertexAttribPointer(_shader_texture->get_id_texture(), 2, GL_FLOAT, GL_FALSE, 0, (char*)NULL + nb_vertices*sizeof(GLfloat));
     glEnableVertexAttribArray(_shader_texture->get_id_texture());

     glBindBuffer(GL_ARRAY_BUFFER, 0);

     glBindVertexArray(0);

     ISOK("Fin chargement contenu");
     _contenu_affichable = true;
}

void SceneGL::cleanupGL()
{
     glBindVertexArray(0);
     std::cout << "Suppression de _arete_EBO[" << _arete_EBO << "]" << std::endl;
     glDeleteBuffers(1, &_arete_EBO);

     std::cout << "Suppression de _arete_VBO[" << _arete_VBO << "]" << std::endl;
     glDeleteBuffers(1, &_arete_VBO);

     std::cout << "Suppression de _arete_VAO[" << _arete_VAO << "]" << std::endl;
     glDeleteVertexArrays(1, &_arete_VAO);

     glDeleteShader(_shader_normal->get_id_fragme_shader());
     glDeleteShader(_shader_normal->get_id_vertex_shader());
     glDeleteProgram(_shader_normal->get_id_shader_program());

     _arete_EBO = 0;
     _arete_VBO = 0;
     _arete_VAO = 0;

     _shader_normal->set_id_fragme_shader(0);
     _shader_normal->set_id_vertex_shader(0);
     _shader_normal->set_id_shader_program(0);
}

void SceneGL::resizeGL(int w, int h)
{
     glViewport(0, 0, w, h);
}

void SceneGL::paintGL()
{
     if( ! _contenu_affichable)
          return;

     glEnable(GL_DEPTH_TEST);
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

     // Gestion camera
     glm::mat4 projection = glm::perspective(70.0f, float(640/480), 0.1f, 100.0f);
     glm::mat4 model      = glm::mat4(1.0);
     glm::mat4 model_node = glm::mat4(1.0);
     glm::mat4 model_text = glm::mat4(1.0);
     glm::mat4 view       = glm::lookAt(glm::vec3(_cam_pos.x(), _cam_pos.y(), _cam_pos.z()),
                                        glm::vec3(0.0, 0.0, 0.0),
                                        glm::vec3(0.0, 1.0, 0.0));

     glm::mat4 mvp = projection * view * model;
     glm::vec4 diffuse_color;
     GLint loc_dif_color = glGetUniformLocation(_shader_normal->get_id_shader_program(), "diffuse_color");

     glUseProgram(_shader_normal->get_id_shader_program());
     // Rotation de camera
     //_coef_rotation += 0.1;
     //model = glm::rotate(model, float(_coef_rotation*3.1415926535/180.0), glm::vec3(0.0, 1.0, 0.0));
     model = glm::rotate(model, float(angleY), glm::vec3(0.0, 1.0, 0.0));
     model = glm::rotate(model, float(angleZ), glm::vec3(0.0, 0.0, 1.0));
     mvp = projection * view * model;

     glUniformMatrix4fv(glGetUniformLocation(_shader_normal->get_id_shader_program(), "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

     // Dessin des aretes
     diffuse_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
     glUniform4fv(loc_dif_color, 1, &(diffuse_color[0]));

     glBindVertexArray(_arete_VAO);
     glDrawElements(GL_LINES, _num_scene_indice, GL_UNSIGNED_INT, NULL);

     // Dessin des noeuds
     std::vector<SceneVertex>* vertices       = _structure->get_vertices();
     std::vector<Etat::Noeud>* vertex_to_etat = _structure->get_vertex_to_etat();
     double scale = _structure->get_scale();

     // Vecteur modifiant la teinture des couleurs des pixels
     glUniform4fv(glGetUniformLocation(_shader_normal->get_id_shader_program(), "Tint"), 1, &_teinture[0]);

     for(unsigned int i = 0; i < vertices->size(); i++)
     {
          // Translation du repere vers les coordonnées du vertex pour dessiner son model etat le representant
          model_node = model;
          model_node = glm::translate(model_node, glm::vec3((*vertices)[i].Position[0],
                                                            (*vertices)[i].Position[1],
                                                            (*vertices)[i].Position[2]));

          // Rescale model
          // FIXME: trouver une relation satisfaisante de downscale en fonction du nombre de sommet (ou en fonction d'un autre parametre)
          model_node = glm::scale(model_node, glm::vec3(scale));
          mvp = projection * view * model_node;
          glUniformMatrix4fv(glGetUniformLocation(_shader_normal->get_id_shader_program(), "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));

          diffuse_color = glm::vec4(_etat_to_data[(*vertex_to_etat)[i]]._lumiere_diffuse.x(),
                                    _etat_to_data[(*vertex_to_etat)[i]]._lumiere_diffuse.y(),
                                    _etat_to_data[(*vertex_to_etat)[i]]._lumiere_diffuse.z(),
                                    1.0f);

          glUniform4fv(loc_dif_color, 1, &(diffuse_color[0]));

          // Dessin noeud courant
          for(std::pair<GLuint,ColGeom>& vao_mesh : _etat_to_data[(*vertex_to_etat)[i]]._list_vao_mesh)
          {
               // first de la paire -> GLuint representant le VAO
               // second de la paire -> Geometry
               glBindVertexArray(vao_mesh.first);
               glDrawElements(vao_mesh.second.primitive, vao_mesh.second.index_count, GL_UNSIGNED_SHORT, vao_mesh.second.indices);
          }
     }
     
     // Dessin du texte (label/valeur/etc)
     glUseProgram(_shader_texture->get_id_shader_program());
     glEnable(GL_DEPTH_TEST);
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

     for(unsigned int i = 0; i < vertices->size(); i++)
     {
          glm::mat4 model_tmp(1.0);

          model_text = glm::rotate(model_tmp, float(angleY), glm::vec3(0.0, 1.0, 0.0));
          model_text = glm::rotate(model_text, float(angleZ), glm::vec3(0.0, 0.0, 1.0));

          model_text = glm::translate(model_text, glm::vec3((*vertices)[i].Position[0],
                                                            (*vertices)[i].Position[1],
                                                            (*vertices)[i].Position[2]));

          model_text = glm::scale(model_text, glm::vec3(scale));

          mvp = view * model_text;

          // Billboarding, permet d'afficher text face camera
          // Colonne 0:
          mvp[0][0] = scale;
          mvp[0][1] = 0;
          mvp[0][2] = 0;

          // Colonne 1:
          mvp[1][0] = 0;
          mvp[1][1] = scale;
          mvp[1][2] = 0;

          // Colonne 2:
          mvp[2][0] = 0;
          mvp[2][1] = 0;
          mvp[2][2] = scale;

          mvp = projection * mvp;

          //
          // Modification contenu texture (pour afficher text courant
          //

          // Creation de l'image
          if(_texture_img != nullptr)
               delete _texture_img;

          _texture_img = new QImage(64, 64, QImage::Format_ARGB32);
          _texture_img->fill(0);

          QFont font;
          font.setStyleHint(_etat_to_data[(*vertex_to_etat)[i]]._style_font);
          font.setBold(true);
          font.setPixelSize(15);

          QPainter painter;
          painter.begin(_texture_img);
          painter.setPen(_etat_to_data[(*vertex_to_etat)[i]]._color_font);
          painter.setFont(font);
          painter.drawText(0, 0, 25, 25, Qt::AlignCenter, std::to_string(i).c_str());
          painter.end();

          *_texture_img = _texture_img->mirrored(false, true);
          // chargement texture à partir de l'image correspondant à du texte
          glBindVertexArray(_texte_VAO);
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, _texte_ID);

          glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _texture_img->width(), _texture_img->height(), GL_RGBA, GL_UNSIGNED_BYTE, _texture_img->bits());

          glUniformMatrix4fv(glGetUniformLocation(_shader_texture->get_id_shader_program(), "mvp"), 1, GL_FALSE, glm::value_ptr(mvp));
          glUniform1i(glGetUniformLocation(_shader_texture->get_id_shader_program(), "tex"), 0);

          glDrawArrays(GL_TRIANGLES, 0, 6);
     }
}

Structure* SceneGL::get_structure()
{
     return _structure;
}

SceneGL::~SceneGL()
{
     if(_shader_normal)
          delete _shader_normal;

     if(_shader_texture)
          delete _shader_texture;

     if(_structure)
          delete _structure;
}

// Camera
void SceneGL::mousePressEvent(QMouseEvent * event)
{
     std::cout << "\n\n[clique]" << std::endl;
     _hold = !_hold;
}

void SceneGL::mouseMoveEvent(QMouseEvent * event)
{
     if(_hold)
     {
          int relx = -1*(old_posx - event->x());
          int rely = (old_posy - event->y());

          std::cout << "\n[mouvement] "
                    << "[x:" << event->x() << "]"
                    << "[y:" << event->y() << "]\n"
                    << "[angleZ:" << angleZ << "]"
                    << "[angleY:" << angleY << "]"
                    << std::endl;

          angleY += relx*0.003;
          angleZ += rely*0.003;

          if(angleZ > 90)
               angleZ = 90;

          if(angleZ < -90)
               angleZ = -90;

          old_posx = event->x();
          old_posy = event->y();
     }
     else
     {
          old_posx = event->x();
          old_posy = event->y();

          _hold = !_hold;
     }
}

void SceneGL::wheelEvent(QWheelEvent * event)
{
     std::cout << "\n\n[scroll]" << std::endl;

     QPoint num_pix = event->pixelDelta();
     QPoint num_deg = event->angleDelta() / 8;

     std::cout << "\n[scroll] "
               << "[y:" << num_deg.y() << "]\n";

     std::cout << "\n[scroll pix] "
               << "[y:" << num_pix.y() << "]\n";

     if(!num_pix.isNull())
     {
          double coef = 1.0;
          if(num_pix.y() > 0)
               coef += num_pix.y()*0.0001;
          else
               coef += num_pix.y()*0.0001;

          _cam_pos *= coef;
     }
     else if(!num_deg.isNull())
     {
          if(num_deg.y() > 0)
               _cam_pos *= 1.1;
          else
               _cam_pos *= 0.9;
     }

     event->accept();
}
