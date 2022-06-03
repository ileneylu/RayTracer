#include "PerlinNoise.hpp"

#include <algorithm>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <vector>

#include <mutex>

static std::mutex m_lock;
using namespace std;

PerlinNoise::PerlinNoise(unsigned int seed) {
  p.resize(256);
  //std::cout <<"hey"<< std::endl;
  std::iota(p.begin(), p.end(), 0);
  //std::cout << p[0] << "," << p[255] << std::endl;
  std::default_random_engine engine(seed);
  std::shuffle(p.begin(), p.end(), engine);
  //p.insert(p.end(), p.begin(), p.end());
  //std::cout << p[0] << "," << p[255] << std::endl;
  //std::cout << "constructor: "<< p.size() << std::endl;
  p.insert(p.end(), p.begin(), p.end());
}

PerlinNoise::~PerlinNoise(){}

double PerlinNoise::noise(double x, double y, double z) {
  int X = (int)floor(x) & 255;
  int Y = (int)floor(y) & 255;
  int Z = (int)floor(z) & 255;
  x -= floor(x);
  y -= floor(y);
  z -= floor(z);
  double u = fade(x);
  double v = fade(y);
  double w = fade(z);
  //std::lock_guard<std::mutex> lockGuard(m_lock);
  //std::cout << "hey" << std::endl;
  //cout << "yo" << p << endl;
  //std::cout << p.size() << std::endl;
  //std::cout << X << "," << Y << "," << Z << std::endl;
  //std::cout << p[0] << "," << p[255] << std::endl;
  int A = p[X]+Y;
  //std::cout << "A: " << A << std::endl;
  int AA = p[A]+Z;
  //std::cout << "AA: " << A << std::endl;
  int AB = p[A+1]+Z;
  //std::cout << "AB: " << A << std::endl;
  int B = p[X+1]+Y;
  //std::cout << "B: " << A << std::endl;
  int BA = p[B]+Z;
  //std::cout << "BA: " << A << std::endl;
  int BB = p[B+1]+Z;
  //std::cout << "BB: " << A << std::endl;

  return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),grad(p[BA], x-1, y, z)),lerp(u, grad(p[AB], x, y-1, z), grad(p[BB], x-1, y-1, z ))),lerp(v, lerp(u, grad(p[AA+1], x, y, z-1), grad(p[BA+1], x-1, y , z-1)),lerp(u, grad(p[AB+1], x, y-1, z-1), grad(p[BB+1], x-1, y-1, z-1))));
}

double PerlinNoise::fade(double t) {
  return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double t, double a, double b) {
  return a + t * (b - a);
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
  int h = hash & 15;
  double u = h<8 ? x : y;
  double v = h<4 ? y : h==12||h==14 ? x : z;
  return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}
