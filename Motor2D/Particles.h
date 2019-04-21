#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#include "Module.h"

class Particle;
class SDL_Texture;

enum class ParticleType {
	NONE = -1,
	ATTACK_EXPLOSION
};

class Particles : public Module
{
public:
	Particles();
	~Particles();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	bool Update(float dt);
	bool CleanUp();
	bool PostUpdate();
	bool Pause();
	bool Resume();

	Particle* CreateParticle(const ParticleType &particle, const fPoint &pos);

private:
	SDL_Texture* particle_atlas = nullptr;
	std::list<Particle*> particles;

	pugi::xml_document particle_file;
	pugi::xml_node particle_configs;

	bool paused = false;
};

#endif //__j1PARTICLES_H__
