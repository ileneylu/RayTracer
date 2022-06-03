// Winter 2019

#include "PhongMaterial.hpp"

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess, double reflectance, double refractiveIndex )
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
	, m_reflectance(reflectance)
	, m_refractiveIndex(refractiveIndex)
{}

PhongMaterial::~PhongMaterial()
{}

glm::vec3 PhongMaterial::getKs() {
	return m_ks;
}

glm::vec3 PhongMaterial::getKd() {
	return m_kd;
}

double PhongMaterial::getShininess() {
	return m_shininess;
}

double PhongMaterial::getReflectance() {
	return m_reflectance;
}

double PhongMaterial::getRefractiveIndex() {
	return m_refractiveIndex;
}
