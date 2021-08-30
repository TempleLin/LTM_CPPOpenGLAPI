#pragma once
class GLMesh;

void setMeshCoordSystem(unsigned int& shaderProgram);

void setBasicMeshSpawnPos(GLMesh& mesh);

void spinBasicMeshAnim(GLMesh& mesh);

void drawBasicMesh(GLMesh& mesh);