#pragma once

enum particleSystemShape
{
	ball,
	cone,
	cylinder
};

enum particleType
{
	point,
	triangle
};

struct particleSystemParams
{
	particleSystemShape shape;
	particleType particletype;
	int numParticle;
	vec3 emitPosistion;
	float minLifeTime ;
	float maxLifeTime ;
	float speed ;
	vec3 acceleration ;
};