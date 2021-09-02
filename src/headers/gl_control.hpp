#pragma once
class GLMesh;

// @Set delta time according to each fps.
void setDeltaTime();

void setMeshCoordSystem(unsigned int& shaderProgram);

void setBasicMeshSpawnPos(GLMesh& mesh);

void spinBasicMeshAnim(GLMesh& mesh);

void drawBasicMesh(GLMesh& mesh);