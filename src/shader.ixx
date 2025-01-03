module;


#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


export module shader;

export class Shader
{
public:
    GLuint program_id;

    Shader(const std::string& vertexPath, const std::string& fragmentPath)
    {
        std::string vertexCode;
        std::ifstream vShaderFile(vertexPath);
        std::stringstream vShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        vertexCode = vShaderStream.str();
        vShaderFile.close();

        std::string fragmentCode;
        std::ifstream fShaderFile(fragmentPath);
        std::stringstream fShaderStream;
        fShaderStream << fShaderFile.rdbuf();
        fragmentCode = fShaderStream.str();
        fShaderFile.close();

        GLuint vertexShader = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
        GLuint fragmentShader = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

        program_id = glCreateProgram();
        glAttachShader(program_id, vertexShader);
        glAttachShader(program_id, fragmentShader);
        glLinkProgram(program_id);

       
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(program_id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(program_id, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void use()
    {
        glUseProgram(program_id);
    }

private:
    GLuint compileShader(const char* shaderSource, GLenum shaderType)
    {
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderSource, nullptr);
        glCompileShader(shader);

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        return shader;
    }
};