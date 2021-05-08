#include "Mesh.h"
#include "tools/vboindexer.h"

void Mesh::load_obj(const char* filename, vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<GLushort>& elements)
{
    ifstream in(filename, ios::in);
    if (!in)
    {
        cerr << "Cannot open " << filename << endl; exit(1);
    }
    string line;
    while (getline(in, line))
    {
        if (line.substr(0, 2) == "v ")
        {
            istringstream s(line.substr(2));
            glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
            vertices.push_back(v);
        }
        else if (line.substr(0, 2) == "f ")
        {
            istringstream s(line.substr(2));
            GLushort a, b, c;
            s >> a; s >> b; s >> c;
            a--; b--; c--;
            elements.push_back(a); elements.push_back(b); elements.push_back(c);
        }
        /* anything else is ignored */
    }

    normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
    for (int i = 0; i < elements.size(); i += 3)
    {
        GLushort ia = elements[i];
        GLushort ib = elements[i + 1];
        GLushort ic = elements[i + 2];
        glm::vec3 normal = glm::normalize(glm::cross(
            glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
            glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
        normals[ia] = normals[ib] = normals[ic] = normal;
    }
}

void Mesh::create()
{
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//// Create and compile our GLSL program from the shaders
	//// TOdo: shader management for instancing
	programID = LoadShaders("assets/shaders/StandardShadingVe.txt","assets/shaders/StandardShadingFr.txt");

	//// Load the texture
	Texture = loadDDS("assets/uvmap.DDS");

	//// Get a handle for our "myTextureSampler" uniform
	TextureID = glGetUniformLocation(programID, "myTextureSampler");
	//// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ("assets/suzanne.obj", vertices, uvs, normals);

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	//// Load it into a VBO

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	//// Generate a buffer for the indices as well
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
}

void Mesh::display()
{
	std::cout << "mesh draw";
	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);
	//glm::translate(glm::mat4(1.0f), glm::vec3(1,1,1));
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_SHORT,   // type
		(void*)0           // element array buffer offset
	);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Mesh::destroy()
{
	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);
}
