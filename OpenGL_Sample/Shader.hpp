//
//  Shader.hpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 11/4/22.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSouce
{
    std::string VertexSource;
    std::string FragmentSource;
};


class Shader
{
private:
    //Really maintaining Filepath as a member for debug purposes
    std::string m_Filepath;
    unsigned int m_RendererID;
    //caching for uniforms
    std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string& filepath);
    ~Shader();
    
    //Naming bind for consistency
    //Really using glUseProgram
    void Bind() const;
    void Unbind() const;
    
    //Set uniforms
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    //If using a maths library would just use some kind of vec4 here
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
    
private:
    ShaderProgramSouce ParseShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string&vertexShader, const std::string& fragmentShader);
    int GetUniformLocation(const std::string& name);
};

#endif /* Shader_hpp */
