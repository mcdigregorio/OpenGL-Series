//
//  Texture.hpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 11/9/22.
//

#ifndef Texture_hpp
#define Texture_hpp

#include "Renderer.h"

class Texture
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    //Local storage for the texture
    unsigned char* m_LocalBuffer;
    //BPP = bits per pixel
    int m_Width, m_Height, m_BPP;
    
public:
    Texture(const std::string& path);
    ~Texture();
    
    //Slot is an optional parameter which allwos you to specify the slot you want to bind the texture to
    void Bind(unsigned int slot = 0) const;
    void Unbind();
    
    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
};

#endif /* Texture_hpp */
