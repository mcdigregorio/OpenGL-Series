//
//  Shader.cpp
//  OpenGL_Sample
//
//  Created by Michael DiGregorio on 11/4/22.
//

#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Shader::Shader(const std::string& filepath)
: m_Filepath(filepath), m_RendererID(0)
{
    //Xcode really isn't setup for relative paths
    ShaderProgramSouce source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}
Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

ShaderProgramSouce Shader::ParseShader(const std::string& filepath)
{
    //Opens file
    std::ifstream stream(filepath);
    
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    
    //Search line by line for shader type using our #shader syntax
    std::string line;
    //1 stringsream for vertex shader, 1 for fragment shader
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while(getline(stream, line))
    {
        if(line.find("#shader") != std::string::npos)
        {
            //Find type of shader
            if(line.find("vertex")!= std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            //Line from file isn't a #shader line, need know which type to push to
            //Using ararys in clever way to automate a little more
            //Avoids if else if branch
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}


//Abstract out shader compilation. Have to do multiple times
unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id  = glCreateShader(type);
    //source needs to exist at ths point
    //If string goes out of scope this char* will point to garbage
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));
    
    //Can query result of glCompileShader
    //iv
    //i specifies we're specifying an integer
    //v specifies it wants a vector, in this case really a pointer
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        //Shader didn't compile successfully
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        //Use alloca to keep message instantiation on stack
        //Lets you allocate on stack dynamically
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    
    return id;
}

//Both inputs are respective shader source code
//Returns int for unique identifier
unsigned int Shader::CreateShader(const std::string&vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    //Attach both shaders to program
    //Think of this almost like compiling C++ code
    //Link to 1 program
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    
    //Can delete shaders since now linked into program
    //Delete the intermediates (like .o objects in C++)
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    
    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}
void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

//Set uniforms
void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

//If using a maths library would just use some kind of vec4 hereShader::
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

//
void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    //v means passing in array (float array)
    //If row major matrix would need to transpose
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

//Marking as const because just supposed to retrieve uniform location, not really modifying shader or anything
GLint Shader::GetUniformLocation(const std::string& name) const
{
    //Caching for performance boost 19:15
    //Every time we set this uniform we retrieve the location
    //Again, and again, and again -> Not very fast
    //Retrieve it the first time and then we're done
    if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }
    
    GLCall(GLint location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        //Avoiding assert here because somtimes its quite valid for us to have -1 here
        //Good if we delcare uniform but don't use it?
        std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
    }
    
    m_UniformLocationCache[name] = location;
    return location;
}
