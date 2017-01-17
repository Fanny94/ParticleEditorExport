#include "RenderQueue.h"


RenderQueue::RenderQueue() 
{

}

RenderQueue::~RenderQueue()
{
	for (size_t i = 0; i < 2; i++)
		if (allShaders[i] != nullptr)
			delete allShaders[i];
}

void RenderQueue::init()
{
	allShaders[0] = new ShaderProgram(shaderBaseType::VERTEX_FRAGMENT, "forward");
	allShaders[1] = new ShaderProgram(shaderBaseType::VERTEX_GEOMETRY_FRAGMENT, "particle");
}

void RenderQueue::updateUniforms(Camera* camera)
{
	allShaders[0]->use();
	allShaders[0]->addUniform(camera->getProjectionMatrix(), "projectionMatrix");
	allShaders[0]->addUniform(camera->getViewMatrix(), "viewMatrix");
	allShaders[0]->unUse();

	allShaders[1]->use();
	allShaders[1]->addUniform(camera->getProjectionMatrix(), "projectionMatrix");
	allShaders[1]->addUniform(camera->getViewMatrix(), "viewMatrix");
	allShaders[1]->unUse();
}

void RenderQueue::forwardPass(std::vector<ModelInstance>* staticModels)
{
	allShaders[0]->use();

	for (int i = 0; i < staticModels->size(); i++)
	{
		ModelAsset* statAsset = staticModels->at(i).asset;
		int numInstance = 0;
		for (int j = 0; j < statAsset->getHeader()->numMeshes; j++)
		{
			//0 == STATIC 1 == DYNAMIC/ANIMATEDS
			size_t size = statAsset->getHeader()->TYPE == 0 ? sizeof(Importer::sVertex) : sizeof(Importer::sSkeletonVertex);
			glBindBuffer(GL_ARRAY_BUFFER, statAsset->getVertexBuffer(j));
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, 0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, size, (void*)(sizeof(float) * 3));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size, (void*)(sizeof(float) * 6));
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, statAsset->getIndexBuffer(j));
			//glDrawElementsInstanced(GL_TRIANGLES, statAsset->getBufferSize(j), GL_UNSIGNED_INT, 0, numInstance);
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES, 0, size);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
	allShaders[0]->unUse();
}

void RenderQueue::particlePass(std::vector<Gear::ParticleSystem*>* particleSystems)
{
	allShaders[1]->use();
	GLuint loc = glGetUniformLocation(allShaders[1]->getProgramID(), "particleSize");
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Color c;
	TextureAsset* tA;
	glm::vec3* pos;

	for (size_t i = 0; i < particleSystems->size(); i++)
	{
		if (particleSystems->at(i)->isActive)
		{
			glUniform1f(loc, particleSystems->at(i)->particleSize);
			pos = particleSystems->at(i)->getPositions();
			particleSystems->at(i)->getTexture()->bind(GL_TEXTURE0);
			size_t ParticleCount = particleSystems->at(i)->getNrOfActiveParticles();

			glBindBuffer(GL_ARRAY_BUFFER, particleSystems->at(i)->getPartVertexBuffer());
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
			glBufferData(GL_ARRAY_BUFFER, (sizeof(glm::vec3)) * ParticleCount, &pos[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glDrawArraysInstanced(GL_POINTS, 0, ParticleCount, 1);
		}
	}
	allShaders[1]->unUse();
	glDisable(GL_BLEND);
}