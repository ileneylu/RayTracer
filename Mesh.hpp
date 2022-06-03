// Winter 2019

#pragma once

#include <vector>
#include <iosfwd>
#include <string>

#include <glm/glm.hpp>

#include "Primitive.hpp"

static double EPSILON = 0.00000001;

struct Triangle
{
	size_t v1;
	size_t v2;
	size_t v3;

	Triangle( size_t pv1, size_t pv2, size_t pv3 )
		: v1( pv1 )
		, v2( pv2 )
		, v3( pv3 )
	{}
};

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh( const std::string& fname );
	bool hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal);
	bool hitBB(Ray r, double t0, double t1, double t, glm::vec3 r_normal);
	void initBoundingBox();

private:
	std::vector<glm::vec3> m_vertices;
	std::vector<Triangle> m_faces;
	glm::vec3 bbPosition;
	float bbRadius;

    friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
};
