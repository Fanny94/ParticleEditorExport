#pragma once
#include "ShaderProgram.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "ModelAsset.h"
#include "TextureAsset.h"
#include "Material.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include "Animation.h"

using namespace Importer;

class RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();

	//int addModelInstance(ModelAsset * asset);
	//int generateWorldMatrix();

	void init();
	void updateUniforms(Camera* camera);

	void forwardPass(std::vector<ModelInstance>* staticModels);
	// TEMP:
	std::vector<Gear::ParticleSystem*> particleSystem;

	void particlePass(std::vector<Gear::ParticleSystem*>* particleSystems);

private:
	ShaderProgram* allShaders[2];
	//ShaderProgram* allShaders[ShaderType::NUM_SHADER_TYPES];
	std::vector<ModelInstance> instances;
	//std::vector<ModelInstance> staticInstances;
	glm::mat4* worldMatrices;
	glm::mat4* tempMatrices;
	int nrOfWorlds;
	int totalWorlds;

};