#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h> // Shader Streams
#include <GL/glut.h>
#include <glm/gtx/transform.hpp>
#include <glm/glm.hpp>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
class Mesh
{
protected:
	vector<glm::vec4> vertices;
	vector<glm::vec3> normals;
	vector<GLushort> elements;

public:
    static void load_obj(const char* filename, vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<GLushort>& elements); 
   
};

