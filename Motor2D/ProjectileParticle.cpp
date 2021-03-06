#include "j1App.h"
#include "Render.h"
#include "ProjectileParticle.h"

const float SNAP_RANGE = 100.0f;

ProjectileParticle::ProjectileParticle(const pugi::xml_node &config, const fPoint &pos, const fPoint &dest, SDL_Texture* sprite):Particle(config,pos,sprite)
{
	destination = dest;
	speed = config.child("stats").attribute("speed").as_float();

	fPoint move_pos = { destination.x - pos.x, destination.y - pos.y };
	float m = sqrtf(pow(move_pos.x, 2.0f) + pow(move_pos.y, 2.0f));
	if (m > 0.0f) {
		move_pos.x /= m;
		move_pos.y /= m;
	}
	direction = { move_pos.x, move_pos.y };
	fPoint base_vector = { 1.0f,0.0f };

	float dot = base_vector.x * direction.x + base_vector.y * direction.y;
	float det = base_vector.x * direction.y - direction.x * base_vector.y;
	angle = atan2(det, dot)*180 / M_PI;
}


ProjectileParticle::~ProjectileParticle()
{

}

bool ProjectileParticle::Update(float dt)
{
	if (reached_dest)
		return false;
	else
		Move(dt);

	current_frame = animation.GetCurrentFrame(dt);

	return true;
}

void ProjectileParticle::Move(float dt)
{
	fPoint movement_vector = direction;
	movement_vector.x *= speed * dt;
	movement_vector.y *= speed * dt;

	fPoint end_position = { position.x + movement_vector.x, position.y + movement_vector.y };

	if (end_position.DistanceNoSqrt({ destination.x, destination.y }) < SNAP_RANGE)
	{
		position = destination;
		reached_dest = true;
	}
	else
	{
		position.x += movement_vector.x;
		position.y += movement_vector.y;
	}
}

bool ProjectileParticle::PostUpdate()
{
	App->render->Blit(sprite, position.x, position.y - current_frame.h*0.5f, &current_frame, 1.0, (double)angle, 0, current_frame.h*0.5f);
	return true;
}