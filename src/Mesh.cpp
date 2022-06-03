// Winter 2019

#include <iostream>
#include <fstream>
#include "polyroots.hpp"

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"
using namespace std;
using namespace glm;

bool drawBB = false;

Mesh::Mesh( const std::string& fname )
	: m_vertices()
	, m_faces()
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;

	std::ifstream ifs( ("Assets/" + fname).c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}
}

void Mesh::initBoundingBox() {
	float maxX, maxY, maxZ, minX, minY, minZ;
	if (m_vertices.size()>0) {
		vec3 v1 = m_vertices[0];
		maxX = v1[0];
		minX = v1[0];
		maxY = v1[1];
		minY = v1[1];
		maxZ = v1[2];
		minZ = v1[2];
		for (auto vertex : m_vertices) {
			if (vertex[0] > maxX) {
				maxX = vertex[0];
			} else if (vertex[0] < minX) {
				minX = vertex[0];
			}
			if (vertex[1] > maxY) {
				maxY = vertex[1];
			} else if (vertex[1] < minY) {
				minY = vertex[1];
			}
			if (vertex[2] > maxZ) {
				maxZ = vertex[2];
			} else if (vertex[2] < minZ) {
				minZ = vertex[2];
			}
		}
		bbPosition = vec3((maxX + minX)/2, (maxY + minY)/2, (maxZ + minZ)/2);
		bbRadius = (fmax(fmax(maxX - minX, maxY - minY), maxZ - minZ) + EPSILON)/2;
	}
}

bool Mesh::hitBB(Ray r, double t0, double t1, double t, glm::vec3 r_normal){
	glm::vec3 disposition = r.origin - bbPosition;
  float a = dot(r.direction, r.direction);
  float b = 2*dot(r.direction, disposition);
  float c = dot(disposition, disposition) - float(bbRadius*bbRadius);

  double *root = new double[2];

  size_t numIntersection = quadraticRoots(double(a), double(b), double(c), root);

  double finalRoot = t;
  if (numIntersection == 2) {
    double root1 = root[0];
    double root2 = root[1];
    if ((root1 >= t0 && root1 <= t1) &&
        (root2 >= t0 && root2 <= t1)) {
          finalRoot = fmin(root1, root2);
    }
  } else if (numIntersection == 1) {
    double root1 = root[0];
    if (root1 >= t0 && root1 <= t1) {
      finalRoot = root1;
    }
  }
  if (finalRoot < t) {
    t = finalRoot;
    r_normal = normalize((r.origin + float(t)*r.direction) - bbPosition);
		//cout << "hit" << endl;
		return true;
  }
  return false;
}

bool Mesh::hit(Ray r, double t0, double t1, double &t, glm::vec3 &r_normal){
	bool result = false;
	double tempT = t;
	vec3 tempN = r_normal;
	if (hitBB(r, t0, t1, tempT, tempN)) {
		if (drawBB) {
			result = hitBB(r, t0, t1, t, r_normal);
		} else {
			//cout << "hit" << endl;
	for (auto triangle : m_faces) {
		vec3 vertex0 = m_vertices[triangle.v1];
		vec3 vertex1 = m_vertices[triangle.v2];
		vec3 vertex2 = m_vertices[triangle.v3];
		vec3 u = vertex1 - vertex0;
		vec3 v = vertex2 - vertex0;
		vec3 normal = cross(u, v);
		if (normal != vec3(0.0)) {
			vec3 w0 = r.origin - vertex0;
			float a = -dot(normal, w0);
			float b = dot(normal, r.direction);
			if (fabs(b) > float(EPSILON)) {
				float tTemp = a / b;
				if (tTemp >= t0 && tTemp <= t1 && tTemp < t) {
					vec3 intersection = r.origin + tTemp * r.direction;
					double uu = dot(u,u), uv = dot(u,v), vv = dot(v,v);
    			vec3 w = intersection - vertex0;
    			double wu = dot(w,u), wv = dot(w,v);
					double d = uv * uv - uu * vv;
			    double p = (uv * wv - vv * wu) / d;
			    if (p > 0.0 && p < 1.0) {    // I is inside T
			    	double q = (uv * wu - uu * wv) / d;
			    	if (q > 0.0 && (p + q) < 1.0) {  // I is inside T
							t = tTemp;
							r_normal = normalize(normal);
							result = true;
						}
					}
				}
			}
		}
	}
}
}
	return result;
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
  out << "mesh {";
  /*

  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
  out << "}";
  return out;
}
