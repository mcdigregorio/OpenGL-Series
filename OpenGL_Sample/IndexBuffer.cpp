//
//  VertexBuffer.cpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 11/2/22.
//

#include "IndexBuffer.hpp"
#include "Renderer.h"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count):
    m_Count(count)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    //May be some danger here because assuming size of an unsigned int is the same as GLuint
    //Cherno never seen an unsigned int not be 4 bytes, but could be different on different platforms
    //Could assert if really concerned
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
