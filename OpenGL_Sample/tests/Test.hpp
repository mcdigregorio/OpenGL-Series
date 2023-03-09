//
//  Test.hpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 3/8/23.
//

#ifndef Test_hpp
#define Test_hpp

#include <iostream>
#include <vector>
#include <functional>

namespace test {

    class Test
    {
    public:
        Test() {}
        virtual ~Test() {}
        
        //Ideally run at fixed frame rate
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
        //Where we'll draw ImGui stuff
        virtual void OnImGuiRender() {}
    };

    class TestMenu: public Test
    {
    public:
        TestMenu(Test*& currentTestPointer);
    
        //Where we'll draw ImGui stuff
        void OnImGuiRender() override;
        
        //As we call this method with different test types, the compiler will create a new version of this function that returns the new class instance we requested (becuase templates)
        template<typename T>
        void RegisterTest(const std::string& name)
        {
            std::cout << "Registering test " << name << std::endl;
            m_Tests.push_back(std::make_pair(name, []() {return new T();}));
        }
    private:
        //Store names and pointers to tests
        //Can't use straight pointer for 2nd arg in pair because tests don't exist yet. We want to crate them. Would have to create tests at beginning of application
        //Use std::function which allows us to pass lambda as argument and return a new test object
        //Actually constructs a test instance instead of just activating a previous one.
        //Could have done this with struct as well
        //Vector acts as a list. String is there to give button a label
        std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
        //Need this to keep track of active test
        //Reference to current test pointer
        //Allows test menu class to write to below pointer, and change current test
        Test*& m_currentTest;
    };

}

#endif /* Test_hpp */
