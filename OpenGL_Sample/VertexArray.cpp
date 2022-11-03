//
//  VertexArray.cpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 11/3/22.
//

#include "VertexArray.hpp"
#include "Renderer.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    //Bind vertex array
    Bind();
    //Bind vertex buffer
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        //Buffer layout
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        //Only need to call once since using one type of attribute
        //Specify the layout of the vertex buffer data
        //This is where "buffer" gets linked to vao
        //Index at first arg is being bound to currently bound GL_ARRAY_BUFFER
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
       offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}
