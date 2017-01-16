#pragma once
#include "BaseIncludes.h"
#include "TextureAsset.h"

struct Partikel
{
	glm::vec3 direction;
	float lifeSpan;
};

struct Color
{
	GLfloat r, g, b;
};

namespace Gear
{
	class ParticleSystem
	{

	public:
		GEAR_API ParticleSystem();
		GEAR_API ParticleSystem(int n, float life, float speed, float rate, int number);
		GEAR_API ~ParticleSystem();

		GEAR_API void systemInit(int n, float life, float speed, float rate, int number);

		GEAR_API void update(const float &dt);
		GEAR_API void explode();
		GEAR_API GLuint getPartVertexBuffer();
		GEAR_API void setEmmiterPos(glm::vec3 pos);
		GEAR_API void setDirection(glm::vec3 dir);
		GEAR_API int getNrOfActiveParticles();
		GEAR_API void activate();
		GEAR_API void deActivate();
		GEAR_API Partikel* getThePartikels();
		GEAR_API glm::vec3* getPositions();
		GEAR_API void setParticlePosition(glm::vec3* pos);
		GEAR_API void setColor(float r, float g, float b);
		GEAR_API Color getColor() const;
		GEAR_API void setTextrue(Importer::TextureAsset* tAParticles);
		GEAR_API Importer::TextureAsset* getTexture();

		GEAR_API void resetEmitter();
		bool isActive;
		bool alive;

		GLuint particleVertexBuffer;
		glm::vec3 position;
		glm::vec3 direction;
		int maxParticles;
		int nrOfActiveParticles;
		Partikel* allParticles;
		glm::vec3* particlePos;
		float lifeTime;
		float partSpeed;
		float particleRate;
		int partPerRate;
		float timer;
		float gravityFactor;
		float focus;
		Color color;
		Importer::TextureAsset* textureAssetParticles;
	};
}