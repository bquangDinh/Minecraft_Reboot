#include "Quad.h"

Quad::Quad(vec3 p1, vec3 p2, vec3 p3, vec3 p4, int type, int face, bool transparent) {
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->p4 = p4;

	this->type = type;
	this->transparent = transparent;
	this->face = face;
}

Quad::Quad() {
	this->p1 = vec3(0.0f);
	this->p2 = vec3(0.0f);
	this->p3 = vec3(0.0f);
	this->p4 = vec3(0.0f);

	this->type = VOXEL_TYPE::AIR;
	this->transparent = false;
	this->face = SIDES::FRONT;
}

int Quad::getType() const {
	return type;
}

int Quad::getFace() const {
	return face;
}

bool Quad::isTransparent() const {
	return transparent;
}

bool Quad::equals(const Quad q) const {
	return type == q.getType() && transparent == q.isTransparent();
}

void Quad::rotateQuadOrder90Counter() {
	vec3 temp = p1;
	p1 = p2;
	p2 = p3;
	p3 = p4;
	p4 = temp;
}

void Quad::rotateQuadOrder90() {
	vec3 temp = p1;
	p1 = p4;
	p4 = p3;
	p3 = p2;
	p2 = temp;
}