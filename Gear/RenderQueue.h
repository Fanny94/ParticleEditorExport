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
struct ModelInstance
{
	ModelAsset* asset;
	//Material material;
	std::vector<int> worldIndices;
};

struct AnimatedInstance
{
	ModelAsset* asset;
	//Material material;
	std::vector<int> worldIndices;
	std::vector<Animation*> animations;
};

class RenderQueue
{
public:
	RenderQueue();
	~RenderQueue();

	//int addModelInstance(ModelAsset * asset);
	//int generateWorldMatrix();

	void init();
	void updateUniforms(Camera* camera);

	//void forwardPass(std::vector<ModelInstance>* staticModels, std::vector<ModelInstance>* dynamicModels);
	// TEMP:
	std::vector<Gear::ParticleSystem*> particleSystem;

	void particlePass(std::vector<Gear::ParticleSystem*>* particleSystems);

private:
	ShaderProgram* allShaders[1];
	//ShaderProgram* allShaders[ShaderType::NUM_SHADER_TYPES];
	std::vector<ModelInstance> instances;
	std::vector<ModelInstance> staticInstances;
	//glm::mat4* worldMatrices;
	//glm::mat4* tempMatrices;
	//int nrOfWorlds;
	//int totalWorlds;

};