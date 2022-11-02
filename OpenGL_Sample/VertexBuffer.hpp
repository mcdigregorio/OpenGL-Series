//
//  VertexBuffer.hpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 11/2/22.
//

#ifndef VertexBuffer_hpp
#define VertexBuffer_hpp

class VertexBuffer
{
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();
    
    void Bind() const;
    void Unbind() const;
};


#endif /* VertexBuffer_hpp */
