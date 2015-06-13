#
# Inclusion des modules du standard Qt
#
QT      += core gui opengl widgets
CONFIG  += c++11

#
# Flags compilation pour CXX (g++)
#
QMAKE_CXXFLAGS += -Wno-strict-aliasing -Wno-comment -Wno-unused-parameter

#
# Flags librairies pour CXX (g++)
#
QMAKE_LIBS += -lgsl -lgslcblas

#
# Nom du programme compilé
#
TARGET	 = programme

#
# Headers
#
HEADERS	 = \
 		   daeloader/tinyxml/tinyxml.h \
 		   daeloader/tinyxml/tinystr.h \
 		   daeloader/colladainterface.h \
           file.hpp \
           cellule.hpp \
		   liste.hpp \
                   graphe.hpp \
                   textbox.hpp \
           scene.hpp \
		   shader.hpp \
		   structure.hpp \
           mainwindow.hpp
#
# Sources
#
SOURCES	 = \
		   daeloader/tinyxml/tinyxml.cpp \
		   daeloader/tinyxml/tinystr.cpp \
		   daeloader/tinyxml/tinyxmlerror.cpp \
		   daeloader/tinyxml/tinyxmlparser.cpp \
		   daeloader/colladainterface.cpp \
           file.cpp \
           cellule.cpp \
		   liste.cpp \
		   graphe.cpp \
                   main.cpp \
                   textbox.cpp \
		   scene.cpp \
		   shader.cpp \
		   structure.cpp \
           mainwindow.cpp
