//
//  TestClearColor.hpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 3/8/23.
//

#ifndef TestTexture2D_hpp
#define TestTexture2D_hpp

#include "Test.hpp"

#include <memory>

#include "Renderer.h"
#include "imgui/imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"

namespace test {

    class TestTexture2D: public Test
    {
    public:
        TestTexture2D();
        ~TestTexture2D();
        
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        std::unique_ptr<VertexArray> m_VAO;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<Shader> m_shader;
        std::unique_ptr<Texture> m_Texture;
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        
        glm::mat4 m_Proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        //With coordinates of 0,0,0 view matrix isn't applying any transformation. Redundant.
        glm::mat4 m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::vec3 m_translationA, m_translationB;
    };

}

#endif /* TestTexture2D_hpp */
