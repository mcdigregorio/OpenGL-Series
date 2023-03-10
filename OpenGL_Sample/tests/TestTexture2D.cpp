//
//  TestClearColor.cpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 3/8/23.
//

#include "TestTexture2D.hpp"

namespace test {

    TestTexture2D::TestTexture2D()
        : m_translationA(200, 200, 0), m_translationB(400, 200, 0),
        m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)))
    {
        //2 floats per position -> X and Y coordinate
        //Need to define for OpenGL
        //Unique vertices needed
        //Last two columns are texture coordinates
        //Centered around 0,0. Translations occur about this point.
        //100x100 px square
        //0,0 is bottom left of screen
        float positions[] {
            -50.0f, -50.0f, 0.0f, 0.0f, //Index 0
             50.0f, -50.0f, 1.0f, 0.0f, //Index 1
             50.0f,  50.0f, 1.0f, 1.0f, //Index 2
            -50.0f,  50.0f, 0.0f, 1.0f  //Index 3
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
        
        m_VAO = std::make_unique<VertexArray>();

        //If we have multiple we'll have to rebind the ones we want to use
        //This will be handled by Vertex Array anyways though
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        //Add another 2 floats for texture coordinates
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        
        //ibo - index buffer object
        //Cherno using unsigned ints here because will use in future
        //and no measureable performance difference for this example
        //Could use unsigned char, unsigned short, etc. to save memory
        //However, something like unsigned char would limit you to 256 indices
        //Key here: TYPE HAS TO BE UNSIGNED
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);
        //For instructional purposes can add break point and see shader math here on CPU to see what we get
        //Take our coordinate and convert it to a space between -1 and 1
        glm::vec4 result = m_Proj*vp;

        m_shader = std::make_unique<Shader>("/Users/michaeldigregorio/devspace/OpenGL_Sample/OpenGL_Sample/res/shaders/basic.shader");
        m_shader->Bind();
        m_shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        m_Texture = std::make_unique<Texture>("/Users/michaeldigregorio/devspace/OpenGL_Sample/OpenGL_Sample/res/textures/bananas.png");
        //0 needs to match Bind arg
        //If called Bind(2), 2nd arg below would be 2
        m_shader->SetUniform1i("u_Texture", 0);
    }

    TestTexture2D::~TestTexture2D()
    {
    }

    void TestTexture2D::OnUpdate(float deltaTime)
    {
    }

    void TestTexture2D::OnRender()
    {
        GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        
        Renderer renderer;
        
        m_Texture->Bind();
        
        {
            //Recalculating model matrix and mvp every frame
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);
            //Model, view, projection matrix
            glm::mat4 mvp = m_Proj * m_View * model;
            //Need to bind to set uniforms
            //Only need to do once since Draw binds shader and doesn't unbind
            m_shader->Bind();
            //Need to set MVP uniform
            //Setting once is enough, but can set every frame if we want to
            m_shader->SetUniformMat4f("u_MVP", mvp);
            //Draw will bind shader and never unbind
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_shader);
        }
    }

    void  TestTexture2D::OnImGuiRender()
    {
        //Single passing the memory address of translation, y and z will get passed along since memory layout is the same
        //Float3 arg 2 is float array
        //In theory could get float array out of GLM, but for now just passing memory address
        ImGui::SliderFloat3("TranslationA", &m_translationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("TranslationB", &m_translationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

}
