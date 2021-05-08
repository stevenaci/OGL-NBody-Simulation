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

#include "Tools/shader.h"
#include "Tools/texture.h"
#include "Tools/objloader.h"
#include "Tools/vboindexer.h"

using namespace std;
class Mesh
{
protected:
	GLuint VertexArrayID;
	GLuint programID;
	GLuint Texture;
	GLuint TextureID;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer; 

public:
    static void load_obj(const char* filename, vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<GLushort>& elements); 
	void create();
	void display();
	void destroy();
};

