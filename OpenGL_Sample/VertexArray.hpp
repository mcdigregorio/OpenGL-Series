//
//  VertexArray.hpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 11/3/22.
//

#include "VertexBuffer.hpp"

//Changing to forward declaration
//After changes to Renderer files, circular dependency was created when including VertexBufferLayout.hpp
class VertexBufferLayout;

#ifndef VertexArray_hpp
#define VertexArray_hpp

class VertexArray
{
private:
    unsigned int m_RendererID;
public:
    VertexArray();
    ~VertexArray();
    
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    
    void Bind() const;
    void Unbind() const;
};

#endif /* VertexArray_hpp */
