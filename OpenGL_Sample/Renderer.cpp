//
//  Renderer.cpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 11/2/22.
//

#include "Renderer.h"

#include <iostream>

void GLClearError()
{
    //Could also write while(!glGetError());
    while(glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while(GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line << std::endl;
        //If return false, means gl call was not successful
        return false;
    }
    return true;
}

