//
//  main.cpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 10/21/22.
//

//Need this definition before glfew.h header which references it
//#define GL_SILENCE_DEPRECATION
#include "GL/glew.h"
#include "glfw3.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

struct ShaderProgramSouce
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSouce ParseShader(const std::string& filepath)
{
    //Opens file
    std::ifstream stream(filepath);
    
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    
    //Search line by line for shader type using our #shader syntax
    std::string line;
    //1 stringsream for vertex shader, 1 for fragment shader
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while(getline(stream, line))
    {
        if(line.find("#shader") != std::string::npos)
        {
            //Find type of shader
            if(line.find("vertex")!= std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            //Line from file isn't a #shader line, need know which type to push to
            //Using ararys in clever way to automate a little more
            //Avoids if else if branch
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

//Abstract out shader compilation. Have to do multiple times
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id  = glCreateShader(type);
    //source needs to exist at ths point
    //If string goes out of scope this char* will point to garbage
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    
    //Can query result of glCompileShader
    //iv
    //i specifies we're specifying an integer
    //v specifies it wants a vector, in this case really a pointer
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        //Shader didn't compile successfully
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        //Use alloca to keep message instantiation on stack
        //Lets you allocate on stack dynamically
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    
    return id;
}

//Both inputs are respective shader source code
//Returns int for unique identifier
static unsigned int CreateShader(const std::string&vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    //Attach both shaders to program
    //Think of this almost like compiling C++ code
    //Link to 1 program
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    
    //Can delete shaders since now linked into program
    //Delete the intermediates (like .o objects in C++)
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    //Needed to force using more modern OpenGL version?
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK)
        std::cout << "Error initializing GLEW" << std::endl;
    
    //Need to call after context created
    std::cout << glGetString(GL_VERSION) << std::endl;

    //2 floats per position -> X and Y coordinate
    //Need to define for OpenGL
    //Unique vertices needed
    float positions[] {
        -0.5f, -0.5f, //Index 0
         0.5f, -0.5f, //Index 1
         0.5f,  0.5f, //Index 2
        -0.5f,  0.5f, //Index 3
    };
    
    //Index buffer
    //Renders square without redundant vertices
    //Index into vertex buffer (see positions array)
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    //Need a vertex array object (VAO), to render anything in core context
    unsigned int vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //Each vertice has 2 floats, and we have 6 vertices
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    //Only need to call once since using one type of attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    //ibo - index buffer object
    //Cherno using unsigned ints here because will use in future
    //and no measureable performance difference for this example
    //Could use unsigned char, unsigned short, etc. to save memory
    //However, something like unsigned char would limit you to 256 indices
    //Key here: TYPE HAS TO BE UNSIGNED
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    //Send index buffer to GPU
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    
    //Xcode really isn't setup for relative paths
    ShaderProgramSouce source = ParseShader("/Users/michaeldigregorio/devspace/OpenGL_Sample/OpenGL_Sample/res/shaders/basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);
    
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        //6 is number of indices, NOT vertices
        //GL_UNSIGNED_INT is type of data in index buffer
        //Can use nullptr since we bind ibo above
        //Element buffer is synonymous with index buffer
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);
    
    glfwTerminate();
    return 0;
}
