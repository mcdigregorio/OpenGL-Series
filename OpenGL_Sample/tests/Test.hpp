//
//  Test.hpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 3/8/23.
//

#ifndef Test_hpp
#define Test_hpp

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

}

#endif /* Test_hpp */
