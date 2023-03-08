//
//  TestClearColor.hpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 3/8/23.
//

#ifndef TestClearColor_hpp
#define TestClearColor_hpp

#include "Test.hpp"

namespace test {

    class TestClearColor: public Test
    {
    public:
        TestClearColor();
        ~TestClearColor();
        
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private:
        float m_ClearColor[4];
    };

}

#endif /* TestClearColor_hpp */
