#pragma once
class Mesh
{
protected:
	vector<glm::vec4> vertices;
	vector<glm::vec3> normals;
	vector<GLushort> elements;

public:
    void load_obj(const char* filename, vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<GLushort>& elements);
   
};

