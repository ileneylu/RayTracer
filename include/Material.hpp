// Winter 2019

#pragma once

#include <glm/glm.hpp>

class Material {
public:
  virtual ~Material();

  virtual glm::vec3 getKs();
  virtual glm::vec3 getKd();
  virtual double getShininess();
  virtual double getReflectance();
  virtual double getRefractiveIndex();

protected:
  Material();
};
