#include <fstream>
#include <sstream>
#include <iostream>
#include <exception>

#include "headers/gl_control.hpp"
#include "headers/gl_values.hpp"
#include "headers/gl_mesh_types.hpp"
#include "headers/assets_edit.hpp"
#include <LTM_CPPOpenGLAPIConfig.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

std::string shaders_assets_foldersPath;

std::vector<unsigned int> vaoGarbageCollector, vboGarbageCollector, programGarbageCollector;
std::vector<GLMesh*> meshesCollector;
std::list<GLLightSource*> globalLightSources;

void checkCompileErrors(unsigned int shader, char* type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void createShaderProgram(unsigned int& vertexShader, std::string vsPath, unsigned int& fragmentShader, std::string fsPath, unsigned int& shaderProgram) {
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexPath = std::string(AssetsRelativePath::getPath()).append(vsPath);
    std::string fragmentPath = std::string(AssetsRelativePath::getPath()).append(fsPath);
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl
            << "Check if CURRENT_RELATIVE_FOLDERPATH is correct." << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    // vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, (char*)"VERTEX");
    // fragment Shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, (char*)"FRAGMENT");

    // shader Program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    checkCompileErrors(shaderProgram, (char*)"PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    programGarbageCollector.push_back(shaderProgram);
}

void createVAOs(unsigned int count, unsigned int* vaos) {
    glGenVertexArrays(count, vaos);
    for (int i = 0; i < count; i++) {
        vaoGarbageCollector.push_back(vaos[i]);
    }
}
void createVBOs(unsigned int count, unsigned int* vbos) {
    glGenBuffers(count, vbos);
    for (int i = 0; i < count; i++) {
        vboGarbageCollector.push_back(vbos[i]);
    }
}
void cleanProgramsGarbage() {
    try {
        for (int i = 0; i < programGarbageCollector.size(); i++) {
            glDeleteProgram(programGarbageCollector.at(i));
        }
    } catch (...) {
        std::cout << "WARNING: PROGRAMS GARBAGE COLLECTOR ERROR" << "\n";
    }
}
void cleanVAOsGarbage() {
    try {
        glDeleteVertexArrays(vaoGarbageCollector.size(), &vaoGarbageCollector[0]);
    } catch (...) {
        std::cout << "WARNING: VAO GARBAGE COLLECTOR ERROR" << "\n";
    }
}
void cleanVBOsGarbage() {
    try {
        glDeleteBuffers(vboGarbageCollector.size(), &vboGarbageCollector[0]);
    } catch (...) {
        std::cout << "WARNING: VBO GARBAGE COLLECTOR ERROR" << "\n";
    }
}
void cleanGLObjectsGarbage() {
    cleanProgramsGarbage();
    cleanVAOsGarbage();
    cleanVBOsGarbage();
}