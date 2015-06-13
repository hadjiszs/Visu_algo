#define GLM_FORCE_RADIANS

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "graphe.hpp"

SceneGL::SceneGL(QWidget* parent, Qt::WindowFlags f)
     : QOpenGLWidget(parent, f)
{
     mSceneVAO = 0;
     mSceneVBO = 0;
     mSceneEBO = 0;
     
     mSceneProgram = 0;
     mSceneVertexShader = 0;
     mSceneFragmentShader = 0;

     mShowDebugNotifications = true;

     mShaderFileWatcher = new QFileSystemWatcher(this);
     connect(mShaderFileWatcher, &QFileSystemWatcher::fileChanged, this, &SceneGL::reloadShaders);

     mTint = QVector4D(0.0f,0.0f,0.0f,0.0f);

     dump = 0;
}

void SceneGL::initializeGL()
{
     QPair<int,int> myGLVersion = context()->format().version();
     QSet<QByteArray> myGLExtensions = context()->extensions();

     getProc(glClear, "glClear");
     getProc(glViewport, "glViewport");
     getProc(glEnable, "glEnable");
     getProc(glDrawElements, "glDrawElements");

     getProc(glGenBuffers, "glGenBuffers");
     getProc(glDeleteBuffers, "glDeleteBuffers");
     getProc(glBindBuffer, "glBindBuffer");
     getProc(glBufferData, "glBufferData");

     getProc(glGenVertexArrays, "glGenVertexArrays");
     getProc(glDeleteVertexArrays, "glDeleteVertexArrays");
     getProc(glBindVertexArray, "glBindVertexArray");
     getProc(glVertexAttribPointer, "glVertexAttribPointer");
     getProc(glEnableVertexAttribArray, "glEnableVertexAttribArray");

     getProc(glCreateShader, "glCreateShader");
     getProc(glDeleteShader, "glDeleteShader");
     getProc(glShaderSource, "glShaderSource");
     getProc(glCompileShader, "glCompileShader");
     getProc(glGetShaderiv, "glGetShaderiv");
     getProc(glGetShaderInfoLog, "glGetShaderInfoLog");

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

     if (context()->format().options() & QSurfaceFormat::DebugContext)
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

     glGenVertexArrays(1, &mSceneVAO);

     graphe* g = new graphe("graphtest2.txt");
     std::vector<SceneVertex> vertices;
     float couleur[3] = {1.0f, 1.0f, 1.0f};
     g->exporter_sommet(vertices, g->barycentric_drawing(),couleur);
     std::vector<GLuint> indices;
     g->exporter_aretes(indices);
     
     glGenBuffers(1, &mSceneVBO);
     glBindBuffer(GL_ARRAY_BUFFER, mSceneVBO);
     glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);

     glGenBuffers(1, &mSceneEBO);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSceneEBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);
     mNumSceneIndices = indices.size();

     reloadShaders();

     connect(context(), &QOpenGLContext::destroyed, this, &SceneGL::cleanupGL);
}

void SceneGL::cleanupGL()
{
     glDeleteBuffers(1, &mSceneEBO);
     glDeleteBuffers(1, &mSceneVBO);
     glDeleteVertexArrays(1, &mSceneVAO);

     glDeleteShader(mSceneFragmentShader);
     glDeleteShader(mSceneVertexShader);
     glDeleteProgram(mSceneProgram);

     mSceneEBO = 0;
     mSceneVBO = 0;
     mSceneVAO = 0;

     mSceneFragmentShader = 0;
     mSceneVertexShader = 0;
     mSceneProgram = 0;
}

void SceneGL::resizeGL(int w, int h)
{
     glViewport(0, 0, w, h);
}

void SceneGL::paintGL()
{
     glm::mat4 projection = glm::perspective(70.0f, float(640/480), 1.0f, 100.0f);
     glm::mat4 modelview  = glm::mat4(1.0);
     glm::mat4 view       = glm::lookAt(glm::vec3(1.0, 2.0, 3.0),
                                        glm::vec3(0.0, 0.0, 0.0),
                                        glm::vec3(0.0, 1.0, 0.0));

     dump += 2.5;
     modelview = glm::rotate(modelview, float(dump*3.1415926535/180.0), glm::vec3(0.0, 1.0, 0.0));
     
     glm::mat4 mvp = projection * view * modelview;
     
     glEnable(GL_DEPTH_TEST);
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

     glUseProgram(mSceneProgram);

     GLint tintLoc = glGetUniformLocation(mSceneProgram, "Tint");
     glUniform4fv(tintLoc, 1, &mTint[0]);
     
     GLint imvp = glGetUniformLocation(mSceneProgram, "mvp");
     glUniformMatrix4fv(imvp, 1, GL_FALSE, glm::value_ptr(mvp));

     glBindVertexArray(mSceneVAO);

     glDrawElements(GL_LINES, mNumSceneIndices, GL_UNSIGNED_INT, NULL);
}

void SceneGL::reloadShaders()
{
     qDebug("Reloading shaders...");

     glDeleteShader(mSceneFragmentShader);
     glDeleteShader(mSceneVertexShader);
     glDeleteProgram(mSceneProgram);

     mSceneVertexShader = glCreateShader(GL_VERTEX_SHADER);
     mSceneFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
     mSceneProgram = glCreateProgram();

     auto compile = [this](const char *shaderFileName, GLuint shader) {
          mShaderFileWatcher->addPath(shaderFileName);

          QFile shaderFile(shaderFileName);
          shaderFile.open(QFile::ReadOnly | QFile::Text);

          QByteArray shaderSource = shaderFile.readAll();
          const char *shaderSourceData = shaderSource.constData();

          glShaderSource(shader, 1, &shaderSourceData, NULL);
          glCompileShader(shader);

          GLint compileStatus;
          glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
          if (!compileStatus)
          {
               std::vector<char> log;
               GLint logLength;
               glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
               log.resize(logLength);
               glGetShaderInfoLog(shader, logLength, NULL, &log[0]);
               qWarning("GL compile error (%s): %s", shaderFileName, log.data());
               return false;
          }
          return true;
     };

     auto link = [this](const char *programName, GLuint program, GLuint vshader, GLuint fshader)
     {
          glAttachShader(program, vshader);
          glAttachShader(program, fshader);
          glLinkProgram(program);

          GLint linkStatus;
          glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
          if (!linkStatus)
          {
               std::vector<char> log;
               GLint logLength;
               glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
               log.resize(logLength);
               glGetProgramInfoLog(program, logLength, NULL, &log[0]);
               qWarning("GL link error (%s): %s", programName, log.data());
               return false;
          }
          return true;
     };

     bool compileFailed = !compile("scene.vert", mSceneVertexShader);
     compileFailed     |= !compile("scene.frag", mSceneFragmentShader);
     if (compileFailed ||
         !link("scene", mSceneProgram, mSceneVertexShader, mSceneFragmentShader))
     {
          glDeleteShader(mSceneVertexShader);
          glDeleteShader(mSceneFragmentShader);
          glDeleteProgram(mSceneProgram);

          mSceneVertexShader = 0;
          mSceneFragmentShader = 0;
          mSceneProgram = 0;

          return;
     }

     GLint positionLoc = glGetAttribLocation(mSceneProgram, "Position");
     GLint colorLoc = glGetAttribLocation(mSceneProgram, "Color");

     glBindVertexArray(mSceneVAO);

     glBindBuffer(GL_ARRAY_BUFFER, mSceneVBO);
     glVertexAttribPointer(positionLoc, SceneVertex::PositionSize, GL_FLOAT, GL_FALSE, sizeof(SceneVertex), (GLvoid*) offsetof(SceneVertex, Position));
     glVertexAttribPointer(colorLoc, SceneVertex::ColorSize, GL_FLOAT, GL_FALSE, sizeof(SceneVertex), (GLvoid*) offsetof(SceneVertex, Color));

     glEnableVertexAttribArray(positionLoc);
     glEnableVertexAttribArray(colorLoc);

     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSceneEBO); // binds EBO to VAO

     glBindVertexArray(0);
}

void SceneGL::disableTint()
{
     mTint = QVector4D(0.0f,0.0f,0.0f,0.0f);
}

void SceneGL::setWarmTint()
{
     mTint = QVector4D(0.827f,0.18f,0.275f,1.0f);
}

void SceneGL::setColdTint()
{
     mTint = QVector4D(0.59f,0.79f,1.0f,1.0f);
}
