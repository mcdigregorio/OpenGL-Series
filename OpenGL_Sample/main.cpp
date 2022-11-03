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

#include "Renderer.h"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"

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
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));
    
    //Can query result of glCompileShader
    //iv
    //i specifies we're specifying an integer
    //v specifies it wants a vector, in this case really a pointer
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        //Shader didn't compile successfully
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        //Use alloca to keep message instantiation on stack
        //Lets you allocate on stack dynamically
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
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
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    
    //Can delete shaders since now linked into program
    //Delete the intermediates (like .o objects in C++)
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    
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
    
    //Synchronize this with our vsync (aka our monitor's refresh rate)
    glfwSwapInterval(1);
    
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
    
    VertexArray va;
    //If we have multiple we'll have to rebind the ones we want to use
    //This will be handled by Vertex Array anyways though
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    //ibo - index buffer object
    //Cherno using unsigned ints here because will use in future
    //and no measureable performance difference for this example
    //Could use unsigned char, unsigned short, etc. to save memory
    //However, something like unsigned char would limit you to 256 indices
    //Key here: TYPE HAS TO BE UNSIGNED
    
    IndexBuffer ib(indices, 6);
    
    //Xcode really isn't setup for relative paths
    ShaderProgramSouce source = ParseShader("/Users/michaeldigregorio/devspace/OpenGL_Sample/OpenGL_Sample/res/shaders/basic.shader");
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    //Binding shader
    GLCall(glUseProgram(shader));
    
    //u_Color needs to match spelling and casing used in shader
    //Retrieve location of u_Color variable
    GLCall(int location = glGetUniformLocation(shader, "u_Color"));
    //Might be okay, if uniform is unused, OpenGL will strip when compiling shader, and -1 will be return
    ASSERT(location != -1);
    //Set data in shader
    GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));
    
    //Clear everything
    va.Unbind();
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    
    //Red channel
    float r = 0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
        GLCall(glUseProgram(shader));
        GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));
    
        va.Bind();
        ib.Bind();
        
        //6 is number of indices, NOT vertices
        //GL_UNSIGNED_INT is type of data in index buffer
        //Can use nullptr since we bind ibo above
        //Element buffer is synonymous with index buffer
        //Could theoretically put into IndexBuffer class, but for our implementation, we'll leave that up to the Renderer
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
        
        //Bounce r channel value between 0 and 1
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0.0f)
            increment =  0.05f;
        
        r += increment;
        
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }

    glDeleteProgram(shader);
    
    GLCall(glfwTerminate());
    return 0;
}
