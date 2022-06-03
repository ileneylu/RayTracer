// Winter 2019

#include "Material.hpp"

Material::Material()
{}

Material::~Material()
{}

glm::vec3 Material::getKs() {
	return glm::vec3(0.0, 0.0, 0.0);
}

glm::vec3 Material::getKd() {
	return glm::vec3(0.0, 0.0, 0.0);
}

double Material::getShininess() {
	return 0;
}

double Material::getReflectance() {
	return 0;
}

double Material::getRefractiveIndex() {
	return 0;
}
