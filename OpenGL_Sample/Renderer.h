//
//  Renderer.h
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 11/2/22.
//

#include <GL/glew.h>

#ifndef Renderer_h
#define Renderer_h

//If false will call function to insert breakpoint at that moment in code
//Intrinsic to compiler (will be different for each compiler)
//For example, for MSVC you call __debugbreak() isntead of __builtin_trap()
#define ASSERT(x) if(!(x)) __builtin_trap();

//Use this macro to wrap OpenGL function calls
//Avoids having to clear errors and check/log errors after every OpenGL function
//A few flaws with this
//  1. If we use a one line if statement this won't work, can fix by surrounding in scope (though not preferable)
//      See macros video for more info
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);

#endif /* Renderer_h */
