//
//  VertexBuffer.hpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 11/2/22.
//

#ifndef IndexBuffer_hpp
#define IndexBuffer_hpp

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    //Need to know how many indices this actually has
    unsigned int m_Count;
public:
    //For now going to support 32-bit indices
    IndexBuffer(const unsigned int* data, unsigned int count);
    ~IndexBuffer();
    
    void Bind() const;
    void Unbind() const;
    
    inline unsigned int GetCount() const { return m_Count; }
};


#endif /* IndexBuffer_hpp */
