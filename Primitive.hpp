// Fall 2018

#pragma once

#include <glm/glm.hpp>

struct Ray {
	glm::vec3 origin;
	glm::vec3 direction;

	Ray(glm::vec3 o, glm::vec3 d): origin(o),direction(d) {}
};

class Primitive {
public:
  virtual ~Primitive();
  virtual bool hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal);
	virtual void initBoundingBox();
	void filterRoots(double t0, double t1, double * result, int & numResult, double * input, size_t numInput);
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  bool hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal);
	void initBoundingBox();
};

class Cube : public Primitive {
public:
  virtual ~Cube();
  bool hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal);
	void initBoundingBox();
};

class Torus : public Primitive {
public:
	Torus(double tubeRadius): m_tubeRadius(tubeRadius){}
	virtual ~Torus();
	bool hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal);
	void initBoundingBox();

private:
	double m_tubeRadius;
};

class Cylinder : public Primitive {
public:
	virtual ~Cylinder();
	bool hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal);
	void initBoundingBox();
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  bool hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal);
	void initBoundingBox();

  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }

  virtual ~NonhierBox();
  bool hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal);
	void initBoundingBox();

  glm::vec3 m_pos;
  double m_size;
};
