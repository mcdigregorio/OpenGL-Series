//
//  VertexBufferLayout.hpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 11/3/22.
//

#include "Renderer.h"

#include <vector>
#include <GL/glew.h>
#include <stdexcept>

#ifndef VertexBufferLayout_hpp
#define VertexBufferLayout_hpp

struct VertexBufferElement
{
    //OpenGL type
    unsigned int type;
    unsigned int count;
    unsigned char normalized;
    
    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch(type)
        {
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout()
    : m_Stride(0) {}
    
    template<typename T>
    void Push(unsigned int count)
    {
        //static_assert(false); Doesn't work
        
        //Isn't checked at compile time, but will throw verbose error at runtime
        std::runtime_error("Need to use specialization");
    }
    
    //Specializations
    template<>
    void Push<float>(unsigned int count)
    {
        m_Elements.push_back({GL_FLOAT, count, GL_FALSE});
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }
    
    template<>
    void Push<unsigned int>(unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);

    }
    
    //Unsigned char really are bytes
    template<>
    void Push<unsigned char>(unsigned int count)
    {
        m_Elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);

    }
    
    inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }
};

#endif /* VertexBufferLayout_hpp */
