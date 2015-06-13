#
# Inclusion des modules du standard Qt
#
QT      += core gui opengl widgets

CONFIG += c++11
#
# Nom du programme compilé
#
TARGET	 = programme

#
# Headers
#
HEADERS	 = scene.hpp \
           mainwindow.hpp \
    graphe.hpp
#
# Sources
#
SOURCES	 = main.cpp \
		   scene.cpp \
           mainwindow.cpp \
    graphe.cpp