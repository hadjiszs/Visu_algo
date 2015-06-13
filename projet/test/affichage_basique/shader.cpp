/**
 * \file shader.cpp
 * \brief Implementation de shader.hpp
 */
#include <iostream>
#include <fstream>

#include "shader.hpp"

Shader::Shader(std::string ver_shad, std::string fra_shad)
{
     std::string	buffer;
     std::string	str_vertex_source;
     std::string	str_fragme_source;

     std::ifstream	flux_vertex(ver_shad.c_str(), std::ios::in);
     std::ifstream	flux_fragme(fra_shad.c_str(), std::ios::in);

     /* Lecture du code source du vertex shader */
     while(getline(flux_vertex, buffer))
          str_vertex_source += std::string(buffer + '\n');

     /* Lecture du code source du fragment shader */
     while(getline(flux_fragme, buffer))
          str_fragme_source += std::string(buffer + '\n');

     GLchar* tmp_vertex_source = new GLchar[_vertex_source_length = str_vertex_source.size()];
     GLchar* tmp_fragme_source = new GLchar[_fragme_source_length = str_fragme_source.size()];

     for(int i(0); i < _vertex_source_length; i++)
          tmp_vertex_source[i] = (GLchar) str_vertex_source[i];

     for(int i(0); i < _fragme_source_length; i++)
          tmp_fragme_source[i] = (GLchar) str_fragme_source[i];

     _vertex_source = tmp_vertex_source;
     _fragme_source = tmp_fragme_source;

     _resolve_fonctions = nullptr;
}

Shader::~Shader()
{
     if(_vertex_source)
          delete[] _vertex_source;

     if(_fragme_source)
          delete[] _fragme_source;
}

void Shader::charger(QOpenGLFunctions* function_contexte)
{
     // Si le resolveur de pointeurs sur fonction du contexte ogl de SceneGL est null, on s'en va
     if( ! (_resolve_fonctions = function_contexte))
          return;

     bool	compile_vert_ok;
     bool	compile_frag_ok;

     /* Création et compilation du vertex shader */
     _id_vertex_shader = _resolve_fonctions->glCreateShader(GL_VERTEX_SHADER);

     _resolve_fonctions->glShaderSource(_id_vertex_shader, 1, &_vertex_source, &_vertex_source_length);
     _resolve_fonctions->glCompileShader(_id_vertex_shader);

     compile_vert_ok = verif_compil_shader(_id_vertex_shader, "vertex");

     /* Création et compilation du fragment shader */
     _id_fragme_shader = _resolve_fonctions->glCreateShader(GL_FRAGMENT_SHADER);

     _resolve_fonctions->glShaderSource(_id_fragme_shader, 1, &_fragme_source, &_fragme_source_length);
     _resolve_fonctions->glCompileShader(_id_fragme_shader);

     compile_frag_ok = verif_compil_shader(_id_fragme_shader, "fragment");

     // S'il y a un des deux shader qui n'a pas compiler correctement, on s'en va
     if( !(compile_vert_ok && compile_frag_ok))
          return;

     // On crée le programme GLSL complet qui va contenir le vertex shader et le fragment shader
     _id_shader_program = _resolve_fonctions->glCreateProgram();

     _resolve_fonctions->glAttachShader(_id_shader_program, _id_vertex_shader);
     _resolve_fonctions->glAttachShader(_id_shader_program, _id_fragme_shader);

     _resolve_fonctions->glBindAttribLocation(_id_shader_program, 3, "in_position");
     _resolve_fonctions->glBindAttribLocation(_id_shader_program, 4, "in_color");

     _resolve_fonctions->glLinkProgram(_id_shader_program);
     if(! verif_link_shader(_id_shader_program, "program"))
          exit(-1);

     // Spécification et envoie des variables et données au programme GLSL
     _id_position = _resolve_fonctions->glGetAttribLocation(_id_shader_program, "in_position");
     _id_color    = _resolve_fonctions->glGetAttribLocation(_id_shader_program, "in_color");

     std::cout << "\ninpos: " << _id_position;
     std::cout << "\nincol: " << _id_color << std::endl;
}

bool Shader::verif_link_shader(GLint id_program, std::string nom)
{
     GLint	link_status = GL_TRUE;
     GLsizei	size_log       = 0;
     char*	log            = nullptr;
     bool	res            = true;

     _resolve_fonctions->glGetProgramiv(id_program, GL_LINK_STATUS, &link_status);

     // Si le shader est bien compiler
     if(link_status == GL_TRUE)
          std::cout << "\n[" << nom << " shader] -> linkage ok\n";
     // Sinon on verifie ce qu'il ne vas pas à partir des logs
     else
     {
          _resolve_fonctions->glGetProgramiv(id_program, GL_INFO_LOG_LENGTH, &size_log);

          log = new char[size_log + 1];
          _resolve_fonctions->glGetProgramInfoLog(id_program, size_log, &size_log, log);

          // Affichage du log d'erreur dans la sortie d'erreur standard
          std::cerr << "\n------------------"
                    << "\nERREUR linkage " << nom << " shader "
                    << "\n>> " << log
                    << "------------------" << std::endl;

          // On libere
          delete[] log;
          _resolve_fonctions->glDeleteProgram(id_program);

          res = false;
     }

     return res;
}

bool Shader::verif_compil_shader(GLint id_shader, std::string nom_shader)
{
     GLint	compile_status = GL_TRUE;
     GLsizei	size_log   = 0;
     char*	log            = nullptr;
     bool	res            = true;

     _resolve_fonctions->glGetShaderiv(id_shader, GL_COMPILE_STATUS, &compile_status);

     // Si le shader est bien compiler
     if(compile_status == GL_TRUE)
          std::cout << "\n[" << nom_shader << " shader] -> compile ok\n";
     // Sinon on verifie ce qu'il ne vas pas à partir des logs
     else
     {
          _resolve_fonctions->glGetShaderiv(id_shader, GL_INFO_LOG_LENGTH, &size_log);

          log = new char[size_log + 1];
          _resolve_fonctions->glGetShaderInfoLog(id_shader, size_log, &size_log, log);

          // Affichage du log d'erreur dans la sortie d'erreur standard
          std::cerr << "\n------------------"
                    << "\nERREUR compilation " << nom_shader << " shader "
                    << "\n>> " << log
                    << "------------------" << std::endl;

          // On libere
          delete[] log;
          _resolve_fonctions->glDeleteShader(id_shader);

          res = false;
     }

     return res;
}

void Shader::del()
{
     _resolve_fonctions->glDeleteProgram(_id_shader_program);

     _resolve_fonctions->glDeleteShader(_id_vertex_shader);
     _resolve_fonctions->glDeleteShader(_id_fragme_shader);

     _resolve_fonctions->glDisableVertexAttribArray(_id_position);
     _resolve_fonctions->glDisableVertexAttribArray(_id_color);
}

// Getters
GLint Shader::get_id_position()
{ return _id_position; }

GLint Shader::get_id_color()
{ return _id_color; }

GLint Shader::get_id_texture()
{ return _id_texture; }

GLint Shader::get_id_shader_program()
{ return _id_shader_program; }

GLint Shader::get_id_vertex_shader()
{ return _id_vertex_shader; }

GLint Shader::get_id_fragme_shader()
{ return _id_fragme_shader; }

GLint		Shader::get_vertex_source_length()
{ return _vertex_source_length; }

GLint		Shader::get_fragme_source_length()
{ return _fragme_source_length; }

const GLchar*	Shader::get_vertex_source()
{ return _vertex_source; }

const GLchar*	Shader::get_fragme_source()
{ return _fragme_source; }

// Setters
void Shader::set_id_position(GLint id)
{ _id_position = id; }

void Shader::set_id_color(GLint id)
{ _id_color = id; }

void Shader::set_id_texture(GLint id)
{ _id_texture = id; }

void Shader::set_id_shader_program(GLint id)
{ _id_shader_program = id; }

void Shader::set_id_vertex_shader(GLint id)
{ _id_vertex_shader = id; }

void Shader::set_id_fragme_shader(GLint id)
{ _id_fragme_shader = id; }
