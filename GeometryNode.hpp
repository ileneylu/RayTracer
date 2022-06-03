// Winter 2019

#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"
#include "Image.hpp"
#include "Light.hpp"
#include "PhongMaterial.hpp"
#include "PerlinNoise.hpp"

const static double T0 = 0.01;
const static double T1 = 10000;
const static float cellSize = 2.0f;
const static glm::vec3 background = glm::vec3(0.0f);

class GeometryNode : public SceneNode {

public:
	GeometryNode( const std::string & name, Primitive *prim,
		Material *mat = nullptr );

	void setMaterial( Material *material );

	glm::vec3 getReflectedRay(glm::vec3 & in, glm::vec3 & normal);

	glm::vec3 getRefractedRay(glm::vec3 & in, glm::vec3 & normal);

	double getFresnelReflectance(glm::vec3 & in, glm::vec3 & normal);

	void setColour(SceneNode * root, Ray r, double &t, glm::vec3 &colour, glm::vec3 ambient, std::list<Light *> lights, Ray & reflectedRay, double & reflectance, Ray & refractedRay, double & shininess);

	glm::vec3 getPhongColour(SceneNode * root, glm::vec3 rayDirection, glm::vec3 fragPosition, glm::vec3 fragNormal,
		glm::vec3 ambient, std::list<Light *> lights);
	glm::vec3 getPointColor(glm::vec3 position);
	glm::vec3 getPointNormal(glm::vec3 normal, glm::vec3 position);
	float turbulance(float x, float y, float z, int frequency);

	bool isBlockedbyMe(Ray r);
	bool isLightSourceBlocked(SceneNode & node, Ray r);

	glm::vec3 getSingleLightSource(glm::vec3 rayDirection, glm::vec3 fragPosition,
		glm::vec3 fragNormal, Light& light);

	Material *m_material;
	Primitive *m_primitive;
	bool hasTexture;
	bool hasBump;
	bool hasPerlinNoise;
	Image *m_textureMap;
	Image *m_bumpMap;
	PerlinNoise *m_perlinNoise;
	bool isMoving;
	glm::vec3 m_initialVelocity;
	glm::vec3 m_acceleration;
};
