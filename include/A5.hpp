// Winter 2019

#pragma once

#include <glm/glm.hpp>

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Light.hpp"
#include "Image.hpp"

const static double exposureTime = 5;
const static double shutterInterval = 0.1;

glm::vec3 backGround(double x, double y, int k, int l);

glm::vec3 getColour(SceneNode * root, Ray r, glm::vec3 uv, int depth, glm::vec3 ambient, std::list<Light *> lights);

glm::vec3 traceGlossyReflection(SceneNode * root, Ray r, glm::vec3 uv, int depth, glm::vec3 ambient, std::list<Light *> lights, double shininess, Ray reflectedRay);

void getLightColour(Light & light, Ray r, glm::vec3 &colour, double & t);

void getNodeColour(SceneNode & node, Ray r, glm::vec3 ambient,
	 std::list<Light *> lights, double &t, glm::vec3 &colour, SceneNode * root, Ray & reflectedRay, double & reflectance, Ray & refractedRay, double & shininess);

// Given the pixel position, return the ray object in world coord
//Ray getPrimaryRay(double x, double y, glm::vec3 eye, glm::vec3 view, glm::vec3 up, glm::vec3 w);

void calculateGlobalTransform(SceneNode & node, glm::mat4 parentTrans, glm::mat4 parentScale);

void moveNodes(SceneNode &node, int t);

void getSuperSampledColour(int x, int y, double nx, double ny,
	double w, double h, glm::vec3 eye, glm::vec3 viewNorm, glm::vec3 upNorm, glm::vec3 xNorm,
	SceneNode * root, const glm::vec3 & ambient, const std::list<Light *> & lights, Image & image);

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		const glm::vec3 & eye,
		const glm::vec3 & view,
		const glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
);
