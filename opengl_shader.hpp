//
//  opengl_shader.hpp
//  stereo-visualization
//
//  Created by Carlos Zoido on 09/01/2019.
//  code from https://github.com/JoeyDeVries/LearnOpenGL

#ifndef opengl_shader_hpp
#define opengl_shader_hpp

#include<string>

class Shader
{
public:
    unsigned int ID;
    Shader();
    void init(const char* vertexPath, const char* fragmentPath);
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec4f(const std::string &name, float x, float y, float z, float w) const;
    void setMat4f(const std::string &name, float* mat) const;
private:
    void checkCompileErrors(unsigned int shader, std::string type);
};


#endif /* opengl_shader_hpp */
