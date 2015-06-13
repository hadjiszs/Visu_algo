#ifndef _SCENE_
#define _SCENE_

#include <QSet>
#include <QFile>
#include <QFileSystemWatcher>

#include <GL/glcorearb.h>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QVector4D>

struct SceneVertex
{
     static const int PositionSize = 3;
     static const int ColorSize = 3;

     float Position[PositionSize];
     float Color[ColorSize];
};

class SceneGL : public QOpenGLWidget {
private:
     
     template<class ProcT>
     ProcT & getProc(ProcT &proc, const QByteArray &procName)
          {
               return proc = (ProcT) context()->getProcAddress(procName);
          }
     
     PFNGLCLEARPROC glClear;
     PFNGLVIEWPORTPROC glViewport;
     PFNGLENABLEPROC glEnable;
     PFNGLDRAWELEMENTSPROC glDrawElements;

     PFNGLGENBUFFERSPROC glGenBuffers;
     PFNGLDELETEBUFFERSPROC glDeleteBuffers;
     PFNGLBINDBUFFERPROC glBindBuffer;
     PFNGLBUFFERDATAPROC glBufferData;

     PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
     PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
     PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
     PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
     PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;

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

     PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
     PFNGLDEBUGMESSAGECALLBACKARBPROC glDebugMessageCallbackARB;
     PFNGLDEBUGMESSAGECALLBACKAMDPROC glDebugMessageCallbackAMD;

     GLuint mSceneVAO;
     GLuint mSceneVBO;
     GLuint mSceneEBO;

     GLuint mSceneProgram;
     GLuint mSceneVertexShader;
     GLuint mSceneFragmentShader;

     GLsizei mNumSceneIndices;

     bool mShowDebugNotifications;

     QFileSystemWatcher *mShaderFileWatcher;

     QVector4D mTint;
     
     float dump;
     
public:
     SceneGL(QWidget* parent = 0, Qt::WindowFlags f = 0);
     void initializeGL(); //override;
     void cleanupGL();
     void resizeGL(int w, int h); //override;
     void paintGL(); //override;
     void reloadShaders();

     void disableTint();
     void setWarmTint();
     void setColdTint();

private:
     static void APIENTRY myGLDebugCallback(GLenum source, GLenum type, GLuint id,
                                            GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
          {
               // TODO: use these parameters to give a more descriptive message.
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

               qWarning("GL debug: %s", message);
          }

     static void APIENTRY myGLDebugCallbackAMD(GLuint id, GLenum category,
                                               GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
          {
               // TODO: use these parameters to give a more descriptive message.
               (void) id;
               (void) category;
               (void) severity;
               (void) length;

               SceneGL *widget = (SceneGL*) userParam;
               
               qWarning("GL debug: %s", message);
          }
};

#endif
