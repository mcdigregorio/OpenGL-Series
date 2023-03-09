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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.hpp"

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
    
    //How OpenGL is going to blend alpha pixels
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));
    
    Renderer renderer;
    
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();
    
    test::Test* currentTest = nullptr;
    test::TestMenu* menu = new test::TestMenu(currentTest);
    //Want to start off with menu
    //Could add CLI arg here to boot up with specific test here
    currentTest = menu;
    
    menu->RegisterTest<test::TestClearColor>("Clear Color");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //Reset window clear color to black when exiting color test
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        //Render here
        renderer.Clear();
        
        //Nothing to do with GLFW new frame
        //Can put pretty much anywhere, just as long as it's
        //Before other ImGui code that isn't for init
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        if(currentTest)
        {
            currentTest->OnUpdate(0.0f);
            currentTest->OnRender();
            ImGui::Begin("Test");
            if(currentTest != menu && ImGui::Button("<-"))
            {
                delete currentTest;
                currentTest = menu;
            }
            currentTest->OnImGuiRender();
            ImGui::End();
        }
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        GLCall(glfwPollEvents());
    }
    
    //If currentTest is menu, deleting things twice
    //Add if statement below
    delete currentTest;
    if(currentTest != menu)
        delete menu;
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    GLCall(glfwTerminate());
    return 0;
}
