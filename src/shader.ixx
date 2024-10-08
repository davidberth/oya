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
    GLuint programID;

    Shader(const std::string& vertexPath, const std::string& fragmentPath)
    {
        // Read vertex shader from file
        std::string vertexCode;
        std::ifstream vShaderFile(vertexPath);
        std::stringstream vShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        vertexCode = vShaderStream.str();
        vShaderFile.close();

        // Read fragment shader from file
        std::string fragmentCode;
        std::ifstream fShaderFile(fragmentPath);
        std::stringstream fShaderStream;
        fShaderStream << fShaderFile.rdbuf();
        fragmentCode = fShaderStream.str();
        fShaderFile.close();

        // Compile shaders
        GLuint vertexShader = compileShader(vertexCode.c_str(), GL_VERTEX_SHADER);
        GLuint fragmentShader = compileShader(fragmentCode.c_str(), GL_FRAGMENT_SHADER);

        // Link shaders to create a program
        programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragmentShader);
        glLinkProgram(programID);

        // Check for linking errors
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(programID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programID, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        // Delete shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void use()
    {
        glUseProgram(programID);
    }

private:
    GLuint compileShader(const char* shaderSource, GLenum shaderType)
    {
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderSource, nullptr);
        glCompileShader(shader);

        // Check for compilation errors
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