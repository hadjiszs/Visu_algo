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
           file.hpp \
           cellule.hpp \
		   liste.hpp \
		   graphe.hpp \
           scene.hpp \
		   shader.hpp \
		   structure.hpp \
           mainwindow.hpp
#
# Sources
#
SOURCES	 = \
           file.cpp \
           cellule.cpp \
		   liste.cpp \
		   graphe.cpp \
		   main.cpp \
		   scene.cpp \
		   shader.cpp \
		   structure.cpp \
           mainwindow.cpp
