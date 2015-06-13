#ifndef COLLADAINTERFACE_H
#define COLLADAINTERFACE_H

#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <cstdlib>
#include <iterator>

#include "GL3/gl3.h"
#include "tinyxml/tinyxml.h"

struct SourceData {
     GLenum type;
     unsigned int size;
     unsigned int stride;
     void* data;
};

typedef std::map<std::string, SourceData> SourceMap;

struct ColGeom {
     std::string name;
     SourceMap map;
     GLenum primitive;
     int index_count;
     unsigned short* indices;
  
     void afficher()
          {	
               std::cout << "\n===\nColGeom [" << name << "]"
			 << "\nprimitive\t[" << primitive << "]"
			 << "\nindex_cout\t[" << index_count << "]"
			 << "\nindices\t["
			 << "\nmap[POSITION].size\t[" << map["POSITION"].size << "]"
			 << "\nmap[POSITION].stride\t[" << map["POSITION"].stride << "]"
			 << "\nmap[POSITION].type\t[" << map["POSITION"].type << "]\n";
               
               float* tmp = (float*) map["NORMAL"].data;
               for(unsigned int i = 0; i < map["NORMAL"].size; i++ )
               {
                    printf("%f", tmp[i]);
                    if(i < map["NORMAL"].size - 1)
                         std::cout << "[" << i << "], ";
               }
	
               std::cout << "]";	
          }
};

SourceData readSource(TiXmlElement*);

class ColladaInterface {

public:
     ColladaInterface() {};
     static void readGeometries(std::vector<ColGeom>*, const char*);
     static void freeGeometries(std::vector<ColGeom>*);
  
     //  friend std::ostream& operator<<(std::ostream& flux, ColGeom& mesh);  
};

#endif

