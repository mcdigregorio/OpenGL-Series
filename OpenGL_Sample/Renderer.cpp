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

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    //Binds a program for our GPU to actually use to render
    shader.Bind();
    //Array that contains all of the actual data
    va.Bind();
    //Contains the indices into the vertex buffer to choose which indices we want to render and how to assemble them together
    ib.Bind();
    //GL_UNSIGNED_INT is type of data in index buffer
    //Can use nullptr since we bind ibo above
    //Element buffer is synonymous with index buffer
    //Could theoretically put into IndexBuffer class, but for our implementation, we'll leave that up to the Renderer
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
