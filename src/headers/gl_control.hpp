#pragma once
#include <vector>
#include <list>
#include <memory>
#include "gl_mesh_types.hpp"

// @Keep track of all OpenGL objects to be removed.
extern std::vector<unsigned int> vaoGarbageCollector, vboGarbageCollector, programGarbageCollector;
extern std::vector<GLMesh*> meshesCollector;
extern std::list<GLLightSource*> globalLightSources;

void setMeshCoordSystem(unsigned int& shaderProgram);

void setBasicMeshSpawnPos(GLMesh& mesh);

void spinBasicMeshAnim(GLMesh& mesh);

void drawBasicMesh(GLMesh& mesh);


void checkCompileErrors(unsigned int shader, char* type);
void createShaderProgram(unsigned int& vertexShader, std::string vsPath, unsigned int& fragmentShader, std::string fsPath, unsigned int& shaderProgram);
void createVAOs(unsigned int count, unsigned int* vaos);
void createVBOs(unsigned int count, unsigned int* vbos);
void cleanProgramsGarbage();
void cleanVAOsGarbage();
void cleanVBOsGarbage();
void cleanGLObjectsGarbage();


class GLGlobalControl {
private:
	static std::unique_ptr<glm::vec4> viewBackgroundColor;
	static std::unique_ptr<glm::vec4> defaultObjectColor;
	static std::unique_ptr<glm::vec4> defaultAmbientColor;
	static std::unique_ptr<glm::vec4> defaultAmbientStrength;
public:
	// @Set delta time according to each fps.
	static void setDeltaTime();
	static void updateGlobalLightSource();
	static void enableMeshesTransparency();
	static glm::vec4 getViewBackgroundColor();
	static glm::vec4 getDefaultObjectColor();
	static glm::vec4 getDefaultAmbientColor();
	static glm::vec4 getDefaultAmbientStrength();
	static void changeDefaultColor(glm::vec4 color, bool normalized); // @Changes all current meshes having default color.
	static void changeDefaultAmbientColor(glm::vec4 ambientColor, bool normalized);
	static void changeDefaultAmbientStrength(glm::vec4 ambientStrength, bool normalized);
	static void resetAllDefaultValues();
	static void resetAllMeshesColor();
	static void resetAllMeshesAmbientColor();
	static void resetAllMeshesAmbientStrength();
	static void destructAllPointersValue();
};

typedef GLGlobalControl GLGC;