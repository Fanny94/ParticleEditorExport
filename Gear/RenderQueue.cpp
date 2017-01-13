#include "RenderQueue.h"


RenderQueue::RenderQueue() 
{

}

RenderQueue::~RenderQueue()
{
    delete allShaders[ShaderType::PARTICLES];
}

void RenderQueue::init()
{
	allShaders[ShaderType::PARTICLES] = new ShaderProgram(shaderBaseType::VERTEX_GEOMETRY_FRAGMENT, "particle");
}

void RenderQueue::updateUniforms(Camera* camera)
{
	allShaders[PARTICLES]->use();
	allShaders[PARTICLES]->addUniform(camera->getProjectionMatrix(), "projectionMatrix");
	allShaders[PARTICLES]->addUniform(camera->getViewMatrix(), "viewMatrix");
	allShaders[PARTICLES]->unUse();
}

void RenderQueue::particlePass(std::vector<Gear::ParticleSystem*>* particleSystems)
{
	allShaders[PARTICLES]->use();
	GLuint loc = glGetUniformLocation(allShaders[PARTICLES]->getProgramID(), "particleSize");
	glUniform1f(loc, 1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Color c;
	TextureAsset* tA;
	glm::vec3* pos;

	for (size_t i = 0; i < particleSystems->size(); i++)
	{
		if (particleSystems->at(i)->isActive)
		{
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
	allShaders[PARTICLES]->unUse();
	glDisable(GL_BLEND);
}