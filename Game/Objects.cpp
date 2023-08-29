#include "Objects.h"

double Floor::centerx() { return width / 2; }
double Floor::centerz() { return depth / 2; }

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"

"void main()\n"
"{\n"
"    FragColor = vec4(0.5f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

Rain::Rain(float x, float y, float z, GLfloat* c) : color(c) {
    body = Engine::Instance()->createSphere(r, x, y, z, 1.23);

    if (body && body->getMotionState())
    {
        body->getMotionState()->getWorldTransform(tform);
    }
}

void Rain::update()
{
    life += 1.0f;

}

void Rain::display()
{
    if (body && body->getMotionState())
    {
        body->getMotionState()->getWorldTransform(tform);
    }
    btVector3 pos = tform.getOrigin();
    
    // Set up and draw a sphere
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    glTranslated(pos.getX(), pos.getY(), pos.getZ());
    glutSolidSphere(r, 30, 30);
    glPopMatrix();

}


Floor::Floor(int width, int depth) : VBO(), VAO(), EBO(), shaderProgram(), width(width), depth(depth) {

    // position vector    
    origin = btVector3(0, -5, 0);
    // shape vector
    //btVector3 shape = btVector3(btScalar(width), btScalar(1.), btScalar(depth));
    btVector3 shape = btVector3(btScalar(width), btScalar(1.), btScalar(depth));

    // Add to Physics World
    body = Engine::Instance()->createGround(origin, shape);

    // Colours
    color[0] = 0.5;
    color[1] = 0.71;
    color[2] = 0.1;

}

void Floor::draw()
{
    //glUseProgram(shaderProgram);
    //glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 4);
    //glBindVertexArray(0);

}

GLfloat vertices[] = {
    -10.5f,  0.5f, 1.0f, // Top-left
     10.5f,  0.5f, 0.0f, // Top-right
     10.5f, -0.5f, 0.0f, // Bottom-right
    -10.5f, -0.5f, 1.0f, // Bottom-left
};
GLuint elements[] = {
    0, 1, 2,
    2, 3, 0
};

void Floor::create()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);

    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    // Unbind the VAO, VBO, and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Create a shader (in modern gl this shader could apply to more objects, but for demo its fine here.)
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);


    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

void Floor::update() {
    if (body && body->getMotionState())
    {
        body->getMotionState()->getWorldTransform(tform);
    }
}

glm::vec3 rotationaxis(0.0f, 1.0f, 0.0f);

void Triangles::draw()
{
    // Draw a textured triangle
    // 
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);

    glTexCoord2f(0.5, 1.0);    glVertex2f(-3, 3);
    glTexCoord2f(0.0, 0.0);    glVertex2f(-3, 0);
    glTexCoord2f(1.0, 0.0);    glVertex2f(0 , 0);

    glTexCoord2f(4, 8);        glVertex2f(3, 3);
    glTexCoord2f(0.0, 0.0);    glVertex2f(0, 0 );
    glTexCoord2f(8, 0.0);      glVertex2f(3 , 0 );

    glTexCoord2f(5, 5);        glVertex2f(0 , 0 );
    glTexCoord2f(0.0, 0.0);    glVertex2f(-1.5 , -3 );
    glTexCoord2f(4, 0.0);      glVertex2f(1.5, -3 );
    glEnd();
    glDisable(GL_TEXTURE_2D);

}
