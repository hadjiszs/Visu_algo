/**
 * \file textbox.cpp
 * \brief Implementation de textbox.hpp
 */
#include "textbox.hpp"

TextBox::TextBox(QWidget* parent)
     : QTextEdit(parent)
{
     std::cout << "\n[Build TextBox]\n" << std::endl;

     setMinimumWidth(300);
     setMaximumWidth(300);
     setReadOnly(true);

     setStyleSheet("background-color: black;"); // Fond noir

     QFont font;
     font.setStyleHint(QFont::Monospace);
     font.setBold(true);
     font.setPixelSize(13);

     setCurrentFont(font);

     _buffer.resize(10);
     clear();
}

void TextBox::afficher()
{
     std::string total_buffer;

     total_buffer += "<div style=\"color:white;\">";
     for(unsigned int i(0); i < _cpt_ligne; i++)
     {
          if(i == _last_ligne)
               total_buffer += "<b>";
          
          total_buffer
               += "<br/>[:"+std::to_string(i)+":] "
               += _buffer[i];
          
          if(i == _last_ligne)
               total_buffer += "</b>";
     }
     total_buffer += "</div>";
     
     setText(total_buffer.c_str());
}

void TextBox::ajouter(std::string str, int num_ligne)
{
     remplacer(str, num_ligne, 0);
}

void TextBox::remplacer(std::string str, int num_ligne, int npos)
{
     std::string& curr_str = _buffer.at(_last_ligne = num_ligne);

     curr_str.replace(npos, str.size(), str);
}

int TextBox::newline()
{
     std::cout << "\nsize[" << _buffer.size() << "]"
               << "| cpt[" << _cpt_ligne;
     
     if(_buffer.size() <= _cpt_ligne)
          _buffer.resize(_buffer.size() + 10);

     return _cpt_ligne++;
}

void TextBox::clear()
{
     _buffer.clear();
     _cpt_ligne = _last_ligne = 0;
}
