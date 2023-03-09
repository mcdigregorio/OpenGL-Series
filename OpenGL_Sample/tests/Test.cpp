//
//  Test.cpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 3/9/23.
//

#include <Test.hpp>

#include "imgui/imgui.h"

namespace test {

    TestMenu::TestMenu(Test*& currentTestPointer)
        : m_currentTest(currentTestPointer)
    {
        
    }

    void TestMenu::OnImGuiRender()
    {
        for (auto& test: m_Tests)
        {
            if(ImGui::Button(test.first.c_str()))
                //Remember this returns test pointer
                //Lambda being run here
                m_currentTest = test.second();
        }
    }

}
