#ifndef TEXTURE_H
#define TEXTURE_H

// Simple shader colors
#ifndef SHADER_COLORS
#define SHADER_COLORS true
#define red {0xff, 0x00, 0x00}
#define yellow {0xff, 0xaa, 0xaa}
#define magenta {0xff, 0, 0xff}
#endif

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char* imagepath);

//// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
//// or do it yourself (just like loadBMP_custom and loadDDS)
//// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char* imagepath);

#endif