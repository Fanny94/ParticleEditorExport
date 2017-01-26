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

struct SendStruct
{
	glm::vec3 pos;
	float size;
};

namespace Gear
{
	class ParticleEmitter
	{

	public:
		GEAR_API ParticleEmitter();
		GEAR_API ~ParticleEmitter();
		GEAR_API ParticleEmitter(float gravity, int n, float life, float speed, float rate, int number, float focusSpread, float dirx, float dirY, float dirZ, float size, float shrink);
		GEAR_API void emitterInit(float gravity, int n, float life, float speed, float rate, int number, float focusSpread, float dirx, float dirY, float dirZ, float size, float shrink);

		bool update(const float &dt);
		GEAR_API void explode();
		GEAR_API GLuint getPartVertexBuffer();
		GEAR_API void setEmitterPos(glm::vec3 pos);
		GEAR_API void setDirection(glm::vec3 dir);
		GEAR_API int getNrOfActiveParticles();
		GEAR_API void activate();
		GEAR_API void deActivate();
		GEAR_API Partikel* getThePartikels();
		GEAR_API SendStruct * getPositions();

		GEAR_API void setTextrue(Importer::TextureAsset* tAParticles, char* textureName);
		GEAR_API Importer::TextureAsset* getTexture();
		GEAR_API char* getTextureName();		
		GEAR_API void resetEmitter();

		Importer::TextureAsset* textureAssetParticles;
		bool isActive;
		float particleSize;
		glm::vec3 position;
		glm::vec3 localPos;
		int maxParticles;
		float lifeTime;
		float partSpeed;
		float particleRate;
		int partPerRate;
		float focus;
		glm::vec3 direction;
		float gravityFactor;
		float shrinkage;

	private:
		
		float timer;		
		Partikel* allParticles;
		SendStruct* particlePos;
		int nrOfActiveParticles;
		GLuint particleVertexBuffer;
		char* particleTextureName;	
		bool alive;

		void spawn(float dt);
	};
}