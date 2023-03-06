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
#include "Texture.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
    window = glfwCreateWindow(960, 540, "Hello There", NULL, NULL);
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
         100.0f, 100.0f, 0.0f, 0.0f, //Index 0
         200.0f, 100.0f, 1.0f, 0.0f, //Index 1
         200.0f, 200.0f, 1.0f, 1.0f, //Index 2
         100.0f, 200.0f, 0.0f, 1.0f  //Index 3
    };
    
    //Index buffer
    //Renders square without redundant vertices
    //Index into vertex buffer (see positions array)
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    //How OpenGL is going to blend alpha pixels
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));
    
    VertexArray va;
    //If we have multiple we'll have to rebind the ones we want to use
    //This will be handled by Vertex Array anyways though
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    //Add another 2 floats for texture coordinates
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    //ibo - index buffer object
    //Cherno using unsigned ints here because will use in future
    //and no measureable performance difference for this example
    //Could use unsigned char, unsigned short, etc. to save memory
    //However, something like unsigned char would limit you to 256 indices
    //Key here: TYPE HAS TO BE UNSIGNED
    IndexBuffer ib(indices, 6);
    
    //Create projection matrix 4x4
    //If you multiply by 2 you'll get 4:3
    //Really created something with distance of 3 units from top to bottom
    //and 4 units from left to right
    //Basically specifies boundaries of our windows
    //-2.0f is left edge
    // 2.0f is right edge
    //-1.5f is bottom edge
    // 1.5f is top edge
    //Last 2 args are near and far plane (If we try to render anthing
    //outside it will get culled)
    
    //If any vertex positions exceed these bounds they will not get rendered
    //This is the view, this is all that you see
    //Because we chose -2.0f and 2.0f, 0.0f is in fact the center for this case
    //glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
    
    //Still keeping 4:3 aspect ratio with below matrix
    //Makes projection matrix bigger which makes image smaller
    //glm::mat4 proj = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);
    
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);
    
    //For instructional purposes can add break point and see shader math here on CPU to see what we get
    //Take our coordinate and convert it to a space between -1 and 1
    glm::vec4 result = proj*vp;
    
    //Xcode really isn't setup for relative paths
    Shader shader("/Users/michaeldigregorio/devspace/OpenGL_Sample/OpenGL_Sample/res/shaders/basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
    //Need to set MVP uniform
    //Setting once is enough, but can set every frame if we want to
    shader.SetUniformMat4f("u_MVP", proj);
    
    Texture texture("/Users/michaeldigregorio/devspace/OpenGL_Sample/OpenGL_Sample/res/textures/bananas.png");
    texture.Bind();
    //0 needs to match Bind arg above
    //If called Bind(2), 2nd arg below would be 2
    shader.SetUniform1i("u_Texture", 0);
    
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
