// Winter 2019
#include <math.h>
#include <iostream>
#include <vector>
#include <glm/gtx/string_cast.hpp>
#include "Primitive.hpp"
#include "polyroots.hpp"



using namespace std;
using namespace glm;

Primitive::~Primitive()
{

}

bool Primitive::hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal) {

  return false;
}

void Primitive::initBoundingBox() {}

void Primitive::filterRoots(double t0, double t1, double * result, int & numResult, double * input, size_t numInput){
  for (int i = 0; i < int(numInput); i++) {
    if (input[i] >= t0 && input[i] <= t1) {
      result[numResult] = input[i];
      numResult++;
    }
  }
}

Sphere::~Sphere()
{
}

bool Sphere::hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal) {
  vec3 s_origin = vec3(0.0);
  glm::vec3 disposition = r.origin;
  float a = dot(r.direction, r.direction);
  float b = 2*dot(r.direction, disposition);
  float c = dot(disposition, disposition) - 1.0f;

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
    normal = normalize((r.origin + float(t)*r.direction));
    return true;
    //cout << to_string(r.origin + float(t) * r.direction);
  }
  return false;
}

void Sphere::initBoundingBox() {}


Cube::~Cube()
{
}

bool Cube::hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal) {
  bool result = false;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      double plane = 0 + (j ? 1 : 0);
      if (r.direction[i] != 0) {
        double tTemp = (plane - double(r.origin[i]))/double(r.direction[i]);
        vec3 intersection = r.origin + float(tTemp) * r.direction;
        int A = (i + 1) % 3, B = (i + 2) % 3;
        double intersectionA = double(intersection[A]);
        double intersectionB = double(intersection[B]);
        if (intersectionA < 1 &&
            intersectionB < 1 &&
            intersectionA > 0 &&
            intersectionB > 0) {
          if (tTemp >= t0 && tTemp <= t1 && tTemp < t) {
            t = tTemp;
            normal = vec3(0.0, 0.0, 0.0);
            normal[i] = 1.0 * (j ? 1.0 : -1.0);
            result = true;
          }
        }
      }
    }
  }
  return result;
}

void Cube::initBoundingBox(){}

Torus::~Torus(){}

bool Torus::hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal) {
  float x = pow(dot(r.direction, r.direction), 2);
  float a = 4 * dot(r.direction, r.direction) * dot(r.origin, r.direction);
  float b = 2 * dot(r.direction, r.direction) * (dot(r.origin, r.origin) - (pow(m_tubeRadius, 2) + 1)) +
            4 * pow(dot(r.origin, r.direction), 2) +
            4 * pow(r.direction[1],2);
  float c = 4 * (dot(r.origin, r.origin) - (pow(m_tubeRadius, 2)+1)) * dot(r.origin, r.direction) +
            8 * r.origin[1] * r.direction[1];
  float d = pow(dot(r.origin, r.origin) - (pow(m_tubeRadius, 2) + 1), 2) -
            4 * (pow(m_tubeRadius, 2) - pow(r.origin[1], 2));

  double root[4];

  size_t numIntersection = quarticRoots(double(a/x), double(b/x), double(c/x), double(d/x), root);
  double finalRoot = t;
  int resultCount = 0;
  double validRoot[4];
  filterRoots(t0, t1, validRoot, resultCount, root, numIntersection);
  for (int i = 0; i < resultCount; i++) {
    if (validRoot[i] < finalRoot) {
        finalRoot = validRoot[i];
    }
  }
  if (finalRoot < t) {
    t = finalRoot;
    vec3 result = r.origin + float(t)*r.direction;
    vec3 normalOrigin = normalize(vec3(result[0], 0.0f, result[2]));
    normal = normalize(result - normalOrigin);
    return true;
  }
  return false;
}

void Torus::initBoundingBox(){}

Cylinder::~Cylinder(){}

bool Cylinder::hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal) {
  float a = pow(r.direction[0], 2) + pow(r.direction[2], 2);
  float b = 2 * (r.origin[0] * r.direction[0] + r.origin[2] * r.direction[2]);
  float c = pow(r.origin[0], 2) + pow(r.origin[2], 2) - 1;

  double root[2];

  size_t numIntersection = quadraticRoots(double(a), double(b), double(c), root);

  double finalRoot = t;
  vec3 normalTemp;
  if (numIntersection == 2) {
    double root1 = root[0];
    double root2 = root[1];
    vec3 intersectionA = r.origin + float(root1) * r.direction;
    vec3 intersectionB = r.origin + float(root2) * r.direction;
    if ((root1 >= t0 && root1 <= t1) &&
        (root2 >= t0 && root2 <= t1) &&
        intersectionA[1] <= 1 &&
        intersectionA[1] >= -1 &&
        intersectionB[1] <= 1 &&
        intersectionB[1] >= -1) {
          if (root1 < root2) {
            finalRoot = root1;
            normalTemp = normalize(vec3(intersectionA[0], 0.0f, intersectionA[2]));
          } else {
            finalRoot = root2;
            normalTemp = normalize(vec3(intersectionB[0], 0.0f, intersectionB[2]));
          }
    } else if (root1 >= t0 && root1 <= t1 &&
      intersectionA[1] <= 1 &&
      intersectionA[1] >= -1) {
      finalRoot = root1;
      normalTemp = normalize(vec3(intersectionA[0], 0.0f, intersectionA[2]));
    } else if (root2 >= t0 && root2 <= t1 &&
      intersectionB[1] <= 1 &&
      intersectionB[1] >= -1) {
      finalRoot = root2;
      normalTemp = normalize(vec3(intersectionB[0], 0.0f, intersectionB[2]));
    }
  } else if (numIntersection == 1) {
    double root1 = root[0];
    vec3 intersectionA = r.origin + float(root1) * r.direction;
    if (root1 >= t0 && root1 <= t1 &&
      intersectionA[1] <= 1 &&
      intersectionA[1] >= -1) {
      finalRoot = root1;
      normalTemp = normalize(vec3(intersectionA[0], 0.0f, intersectionA[2]));
    }
  }
  double tTemp;
  vec3 intersection;
  tTemp = (1 - double(r.origin[1]))/double(r.direction[1]);
  intersection = r.origin + float(tTemp) * r.direction;
  if (pow(intersection[0], 2) + pow(intersection[2], 2) < 1 &&
      tTemp >= t0 && tTemp <= finalRoot) {
        finalRoot = tTemp;
        normalTemp = vec3(0.0f, 1.0f, 0.0f);
  }
  tTemp = (-1 - double(r.origin[1]))/double(r.direction[1]);
  intersection = r.origin + float(tTemp) * r.direction;
  if (pow(intersection[0], 2) + pow(intersection[2], 2) < 1 &&
      tTemp >= t0 && tTemp <= finalRoot) {
        finalRoot = tTemp;
        normalTemp = vec3(0.0f, -1.0f, 0.0f);
  }
  if (finalRoot < t) {
    t = finalRoot;
    normal = normalTemp;
    return true;
  }
  return false;
}

void Cylinder::initBoundingBox(){}

NonhierSphere::~NonhierSphere()
{
}

void NonhierSphere::initBoundingBox() {}

bool NonhierSphere::hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal) {
  glm::vec3 disposition = r.origin - m_pos;
  float a = dot(r.direction, r.direction);
  float b = 2*dot(r.direction, disposition);
  float c = dot(disposition, disposition) - float(m_radius*m_radius);

  double *root = new double[2];

  size_t numIntersection = quadraticRoots(double(a), double(b), double(c), root);

  double finalRoot = t;
  if (numIntersection == 2) {
    double root1 = root[0];
    double root2 = root[1];
    if ((root1 >= t0 && root1 <= t1) &&
        (root2 >= t0 && root2 <= t1)) {
          finalRoot = fmin(root1, root2);
    } else if (root1 >= t0 && root1 <= t1) {
      finalRoot = root1;
    } else if (root2 >= t0 && root2 <= t1) {
      finalRoot = root2;
    }
  } else if (numIntersection == 1) {
    double root1 = root[0];
    if (root1 >= t0 && root1 <= t1) {
      finalRoot = root1;
    }
  }
  if (finalRoot < t) {
    t = finalRoot;
    normal = normalize((r.origin + float(t)*r.direction)- m_pos);
    return true;
  }
  return false;
}

NonhierBox::~NonhierBox()
{
}

void NonhierBox::initBoundingBox() {}

bool NonhierBox::hit(Ray r, double t0, double t1, double &t, glm::vec3 &normal) {
  bool result = false;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      double plane = double(m_pos[i]) + (j ? m_size : 0);
      if (r.direction[i] != 0) {
        double tTemp = (plane - double(r.origin[i]))/double(r.direction[i]);
        vec3 intersection = r.origin + float(tTemp) * r.direction;
        int A = (i + 1) % 3, B = (i + 2) % 3;
        double intersectionA = double(intersection[A]);
        double intersectionB = double(intersection[B]);
        if (intersectionA < fmax(m_pos[A], m_pos[A] + m_size) &&
            intersectionB < fmax(m_pos[B], m_pos[B] + m_size) &&
            intersectionA > fmin(m_pos[A], m_pos[A] + m_size) &&
            intersectionB > fmin(m_pos[B], m_pos[B] + m_size)) {
          if (tTemp >= t0 && tTemp <= t1 && tTemp < t) {
            t = tTemp;
            normal = vec3(0.0, 0.0, 0.0);
            normal[i] = 1.0 * (j ? 1.0 : -1.0) * (m_size > 0 ? 1.0 : -1.0);
            result = true;
          }
        }
      }
    }
  }
  return result;
}
