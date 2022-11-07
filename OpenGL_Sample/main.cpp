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

#include "Renderer.h"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"

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
    Shader shader("/Users/michaeldigregorio/devspace/OpenGL_Sample/OpenGL_Sample/res/shaders/basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
    
    //Clear everything
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    
    Renderer renderer;
    
    //Red channel
    float r = 0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //Render here
        renderer.Clear();
        
        shader.Bind();
        shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
    
        renderer.Draw(va, ib, shader);
        
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
    
    GLCall(glfwTerminate());
    return 0;
}
