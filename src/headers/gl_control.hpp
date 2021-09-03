#pragma once
#include <vector>
#include "gl_mesh_types.hpp"

// @Keep track of all OpenGL objects to be removed.
extern std::vector<unsigned int> vaoGarbageCollector, vboGarbageCollector, programGarbageCollector;
extern std::vector<GLMesh*> meshesCollector;

// @Set delta time according to each fps.
void setDeltaTime();

void setMeshCoordSystem(unsigned int& shaderProgram);

void setBasicMeshSpawnPos(GLMesh& mesh);

void spinBasicMeshAnim(GLMesh& mesh);

void drawBasicMesh(GLMesh& mesh);

void enableMeshesTransparency();

void checkCompileErrors(unsigned int shader, char* type);
void createShaderProgram(unsigned int& vertexShader, std::string vsPath, unsigned int& fragmentShader, std::string fsPath, unsigned int& shaderProgram);
void createVAOs(unsigned int count, unsigned int* vaos);
void createVBOs(unsigned int count, unsigned int* vbos);
void cleanProgramsGarbage();
void cleanVAOsGarbage();
void cleanVBOsGarbage();
void cleanGLObjectsGarbage();
