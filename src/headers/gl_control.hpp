#pragma once
#include <vector>
#include <list>
#include <memory>
#include "gl_mesh_types.hpp"

// @Keep track of all OpenGL objects to be removed.
extern std::vector<unsigned int> vaoGarbageCollector, vboGarbageCollector, programGarbageCollector;
extern std::vector<GLMesh*> meshesCollector;
extern std::list<GLLightSource*> globalLightSources;

class GLMeshCtrl {
public:
	static void setMeshCoordSystem(GLMesh& mesh);
	static void setBasicMeshSpawnPos(GLMesh& mesh);
	static void spinBasicMeshAnim(GLMesh& mesh);
	static void drawBasicMesh(GLMesh& mesh);

	static void setDiffuseMap(GLMesh& mesh, std::string texturePath, bool hasAlpha);
	static void setSpecularMap(GLMesh& mesh, std::string texturePath, bool hasAlpha);
	static void overrideDiffuseColor(GLMesh& mesh, glm::vec3 color, bool isNormalized);
	static void setOpacity(GLMesh& mesh, float opacity);
	static void overrideAmbientColor(GLMesh& mesh, glm::vec3 ambientColor, bool isNormalized);
	static void overrideAmbientStrength(GLMesh& mesh, float ambientStrength);
	static void overrideSpecularStrength(GLMesh& mesh, float specularStrength);
	static void overrideShininess(GLMesh& mesh, float shininess);
	static void resetToDefaultColor(GLMesh& mesh); // @Resets to world value defaultObjectColor
	static void resetToDefaultAmbientColor(GLMesh& mesh); // @Resets to world value defaultAmbientColor
	static void resetToDefaultAmbientStrength(GLMesh& mesh); // @Resets to world value defaultAmbientStrength
	static void resetToDefaultSpecularStrength(GLMesh& mesh);
	static void resetToDefaultShininess(GLMesh& mesh);
};



void checkCompileErrors(unsigned int shader, char* type);
void createShaderProgram(unsigned int& vertexShader, std::string vsPath, unsigned int& fragmentShader, 
	std::string fsPath, unsigned int& shaderProgram);
void createVAOs(unsigned int count, unsigned int* vaos);
void createVBOs(unsigned int count, unsigned int* vbos);
void cleanProgramsGarbage();
void cleanVAOsGarbage();
void cleanVBOsGarbage();
void cleanGLObjectsGarbage();


class GLGlobalCtrl {
private:
	static glm::vec3 viewBackgroundColor;
	static glm::vec3 defaultObjectColor;
	static glm::vec3 defaultAmbientColor;
	static float defaultAmbientStrength;
	static float defaultSpecularStrength;
	static float defaultShininess;
public:
	// @Set delta time according to each fps.
	static void setDeltaTime();
	static void updateGlobalLightSource();
	static void updateCameraViewPosToMeshes();
	static void enableMeshesTransparency();

	static glm::vec3 getViewBackgroundColor();
	static glm::vec3 getDefaultObjectColor();
	static glm::vec3 getDefaultAmbientColor();
	static float getDefaultAmbientStrength();
	static float getDefaultSpecularStrength();
	static float getDefaultShininess();

	static void changeViewBackgroundColor(glm::vec3 color, bool normalized);
	static void changeDefaultColor(glm::vec3 color, bool normalized); // @Changes all current meshes having default color.
	static void changeDefaultAmbientColor(glm::vec3 ambientColor, bool normalized);
	static void changeDefaultAmbientStrength(float ambientStrength);
	static void changeDefaultSpecularStrength(float specularStrength);
	static void changeDefaultShininess(float shininess);

	static void resetAllDefaultValues();
	static void resetAllMeshesColor();
	static void resetAllMeshesAmbientColor();
	static void resetAllMeshesAmbientStrength();
	static void resetAllMeshesSpecularStrength();
	static void resetAllMeshesDefaultShininess();
};