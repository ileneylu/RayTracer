// Winter 2019

#include <glm/ext.hpp>
#include <math.h>
#include <glm/gtx/string_cast.hpp>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <mutex>

#include "A5.hpp"
#include "GeometryNode.hpp"
using namespace std;
using namespace glm;

int randomNum = 0;
mutex m_lock;
bool hasMovingObject = false;

vec3 backGround(double x, double y, int k, int l) {
	if (k == 0 && l == 0) {
		randomNum = rand() % 1000;
	}
	if (randomNum >= 997) {
		return vec3(1.0f, 1.0f, 1.0f);
	}
	return float(y) * vec3(0.0f, 0.51f, 0.65f) + float(1-y) * vec3(0.0f, 0.10f, 0.30f);
}



vec3 getColour(SceneNode * root, Ray r, glm::vec3 uv, int depth, glm::vec3 ambient, std::list<Light *> lights) {
	// vec3 colour = backGround(tX, tY, k, l);
	vec3 colour = vec3(0.0f);
	double t = 100000;
	Ray reflectedRay = Ray(vec3(0.0f), vec3(0.0f));
	Ray refractedRay = Ray(vec3(0.0f), vec3(0.0f));
	double reflectance = 0;
	double shininess = 0;
	for (auto child: root->children) {
		getNodeColour(*child, r, ambient, lights, t, colour, root, reflectedRay, reflectance, refractedRay, shininess);
	}
	if (depth == 0) {
		return colour;
	} else {
		if (reflectance > 0 && refractedRay.direction != vec3(0.0f)) {
			return
			(1 - reflectance) * getColour(root, refractedRay, uv, depth-1, ambient, lights) +
			reflectance * getColour(root, reflectedRay, uv, depth-1, ambient, lights);
		} else if (reflectance > 0) {
			if (shininess > 0) {
				return (1 - reflectance) * colour + reflectance * traceGlossyReflection(root, r, uv, depth, ambient, lights, shininess, reflectedRay);
			} else {
				return (1 - reflectance) * colour + reflectance * 	getColour(root, reflectedRay, uv, depth-1, ambient, lights);
			}
		} else {
			return colour;
		}
	}
}

vec3 traceGlossyReflection(SceneNode * root, Ray r, glm::vec3 uv, int depth, glm::vec3 ambient, std::list<Light *> lights, double shininess, Ray reflectedRay) {
	vec3 vec1 = cross(reflectedRay.direction, r.direction);
	if (vec1 == vec3(0.0f)) {
		return getColour(root, reflectedRay, uv, depth-1, ambient, lights);
	} else {
		vec3 u = normalize(vec1);
		vec3 v = normalize(cross(reflectedRay.direction, u));
		vec3 colour = vec3(0.0f);
		int numSample = 25;
		for (int i = 0; i < numSample ; i++) {
			float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float u_a = - shininess / 2 + r1 * shininess;
			float v_a = - shininess / 2 + r2 * shininess;
			Ray disturbedRay = Ray(vec3(0.0f), vec3(0.0f));
			disturbedRay.origin = reflectedRay.origin;
			disturbedRay.direction = reflectedRay.direction + u_a * u + v_a * v;
			colour += getColour(root, disturbedRay, uv, depth-1, ambient, lights);
		}
		return colour/numSample;
	}
}

void getNodeColour(SceneNode & node, Ray r, glm::vec3 ambient
	, std::list<Light *> lights, double &t, glm::vec3 &colour, SceneNode * root, Ray & reflectedRay, double & reflectance, Ray & refractedRay, double & shininess) {
		if (node.m_nodeType == NodeType::GeometryNode) {
			GeometryNode& gnode = static_cast<GeometryNode&>(node);
			gnode.setColour(root, r, t, colour, ambient, lights, reflectedRay, reflectance, refractedRay, shininess);
		}
		if (!node.children.empty()) {
			for (auto child: node.children) {
				getNodeColour(*child, r, ambient, lights, t, colour, root, reflectedRay, reflectance, refractedRay, shininess);
			}
		}
	}

void calculateGlobalTransform(SceneNode &node, mat4 parentTrans, mat4 parentScale) {
	mat4 curTransform = parentTrans * node.trans;
	mat4 curScale = parentScale * node.scaleM;
	node.set_globalTransform(curTransform);
	node.globalTransCopy = curTransform;
	node.set_globalScale(curScale);
	if (node.m_nodeType == NodeType::GeometryNode) {
		GeometryNode& gnode = static_cast<GeometryNode&>(node);
		gnode.m_primitive->initBoundingBox();
		if (gnode.isMoving) {
			hasMovingObject = true;
		}
	}
	if (!(node.children).empty()) {
		for (auto child: node.children) {
			calculateGlobalTransform(*child, curTransform, curScale);
		}
	}
}

void moveNodes(SceneNode &node, int t) {
	if (node.m_nodeType == NodeType::GeometryNode) {
		GeometryNode& gnode = static_cast<GeometryNode&>(node);
		if (gnode.isMoving) {
			vec3 disposition = float(t * shutterInterval) * gnode.m_initialVelocity + float(0.5 * pow(t * shutterInterval, 2)) * gnode.m_acceleration;
			gnode.globalTrans[3][0] = gnode.globalTransCopy[3][0] + disposition[0];
			gnode.globalTrans[3][1] = gnode.globalTransCopy[3][1] + disposition[1];
			gnode.globalTrans[3][2] = gnode.globalTransCopy[3][2] + disposition[2];
			gnode.invglobalTrans = inverse(gnode.globalTrans);
		}
	}
	if (!(node.children).empty()) {
		for (auto child: node.children) {
			moveNodes(*child, t);
		}
	}
}

void getSuperSampledColour(int x, int y, double nx, double ny,
	double w, double h, vec3 eye, vec3 viewNorm, vec3 upNorm, vec3 xNorm,
	SceneNode * root, const vec3 & ambient, const list<Light *> & lights,
	Image & image) {
		vec3 colourSum = vec3(0.0f);
		for (int k = 0; k < 3; k++) {
			for (int l = 0; l < 3; l++) {
				double imageX = -nx/2 + (double(x) + 0.3 *(k+1))*nx/w;
				double imageY = ny/2 - (double(y) + 0.3 *(l+1))*ny/h;

				// p is the pixel center in world coord.
				glm::vec3 p = eye + viewNorm + float(imageX)*xNorm + float(imageY) *upNorm;

				Ray* r = new Ray(eye, normalize(p - eye));

				vec3 pixelColor = getColour(root, *r, vec3(0.0f, 0.0f, 0.0f), 4,
				ambient, lights);
				colourSum += pixelColor;
			}
	}
	colourSum /= 9;
	lock_guard<mutex> lockGuard(m_lock);
	image(x, y, 0) += double(colourSum[0]);
	image(x, y, 1) += double(colourSum[1]);
	image(x, y, 2) += double(colourSum[2]);
	//colourMap[x*int(h)+y] = colourSum;
}

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
) {

	calculateGlobalTransform(*root, mat4(1.0f), mat4(1.0f));

	size_t h = image.height();
	size_t w = image.width();

	cout << "fov" << fovy << endl;
	double nx = asin(radians(fovy/2)) * 2;
	cout << "nx: " << nx << endl;
	double ny = nx;
	double n = 1;

	glm::vec3 viewNorm = glm::normalize(view);
	glm::vec3 upNorm = glm::normalize(up);
	glm::vec3 xNorm = glm::cross(viewNorm, upNorm);

  std::cout << "Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;

	if (hasMovingObject) {
		/////// HEREEEEEEEEEEEEEEEEEEE!!!!!!!!!!!!
		int numTimeIntervals = int(exposureTime/shutterInterval);
		for (int t = 0; t < numTimeIntervals; t++) {
			if (t != 0) {
				moveNodes(*root, t);
			}
			vector<thread> threads;
			for (int x = 0; x < w; x++) {
				for (int y = 0; y < h; y++) {
					threads.push_back(thread(getSuperSampledColour, x, y,
						nx, ny, w, h, eye, viewNorm, upNorm, xNorm, root, ref(ambient),
						ref(lights), ref(image)));
						//cout << to_string(pixelColor)<< endl;
					// colourMap[x][y] += pixelColor;
				}
				//cout << x << endl;
			}
			for(int i = 0; i < threads.size() ; i++)
		  {
		      threads.at(i).join();
		  }
		}
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				for (int i = 0; i < 3; i++) {
					image(x, y, i) /= numTimeIntervals;
				}
			}
		}
	} else {
		vector<thread> threads;
		for (int x = 0; x < w; x++) {
			for (int y = 0; y < h; y++) {
				threads.push_back(thread(getSuperSampledColour, x, y,
					nx, ny, w, h, eye, viewNorm, upNorm, xNorm, root, ref(ambient),
					ref(lights), ref(image)));
			}
			//cout << x << s;
		}
		for(int i = 0; i < threads.size() ; i++)
		{
				threads.at(i).join();
		}
	}
}
