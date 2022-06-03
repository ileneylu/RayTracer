// Winter 2019
#include <math.h>

#include "GeometryNode.hpp"
#include <glm/gtx/string_cast.hpp>
#include "Mesh.hpp"
#include <mutex>
using namespace std;
using namespace glm;

static mutex m_lock;

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
{
	m_nodeType = NodeType::GeometryNode;
	hasTexture = false;
	hasBump = false;
	hasPerlinNoise = false;
	isMoving = false;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

void GeometryNode::setColour(SceneNode * root, Ray r, double &t, glm::vec3 &colour, glm::vec3 ambient, std::list<Light *> lights, Ray & reflectedRay, double & reflectance, Ray & refractedRay, double & shininess) {
		vec3 normal = vec3(0.0f);
		// transform Ray to model coordinate;
		Ray r_m = Ray(vec3(invglobalScale * invglobalTrans * vec4(r.origin,1.0)), vec3(invglobalScale * invglobalTrans * vec4(r.direction, 0.0)));
		if (m_primitive->hit(r_m, T0, T1, t, normal)) {
			vec3 position = r.origin + float(t) * r.direction;
			vec3 normal_w = normalize(vec3(globalTrans * invglobalScale * vec4(normal, 0.0)));
			colour = getPhongColour(root, r.direction, position, normal_w, ambient, lights);
			reflectedRay.origin = position;
			reflectedRay.direction = getReflectedRay(r.direction, normal_w);
			refractedRay.origin = position;
			refractedRay.direction = getRefractedRay(r.direction, normal_w);
			if (m_material->getRefractiveIndex() != 0) {
				reflectance = getFresnelReflectance(r.direction, normal_w);
			} else {
				reflectance = m_material->getReflectance();
				shininess = m_material->getShininess();
			}
		}
}

// void GeometryNode::setColour(SceneNode * root, Ray r, double &t, glm::vec3 &colour, glm::vec3 ambient, std::list<Light *> lights, Ray & reflectedRay, double & reflectance, Ray & refractedRay, double & shininess) {
// 	if (isMoving) {
// 		mat4 globalTransCopy = globalTrans;
// 		vec3 aggregatedColour = vec3(0.0f);
// 		lock_guard<mutex> lockGuard(m_lock);
// 		for (int i = 0; i < exposureTime/shutterInterval; i++) {
// 			vec3 tempColour = vec3(0.0f);
// 			vec3 disposition = float(i * shutterInterval) * m_initialVelocity + float(0.5 * pow(i * shutterInterval, 2)) * m_acceleration;
// 			globalTrans[3][0] = globalTransCopy[3][0] + disposition[0];
// 			globalTrans[3][1] = globalTransCopy[3][1] + disposition[1];
// 			globalTrans[3][2] = globalTransCopy[3][2] + disposition[2];
// 			invglobalTrans = inverse(globalTrans);
// 			getStaticObjectColour(root, r, t, tempColour, ambient, lights, reflectedRay, reflectance, refractedRay, shininess);
// 			aggregatedColour += tempColour;
// 		}
// 		globalTrans = globalTransCopy;
// 		invglobalTrans = inverse(globalTrans);
// 		colour = aggregatedColour/float(exposureTime/shutterInterval);
// 	} else {
// 		getStaticObjectColour(root, r, t, colour, ambient, lights, reflectedRay, reflectance, refractedRay, shininess);
// 	}
// }

double GeometryNode::getFresnelReflectance(vec3 & in, vec3 & normal){
	float cosi = dot(in, normal);
	float etai = 1;
	float etat = m_material->getRefractiveIndex();
	if (cosi > 0) {
		swap(etai, etat);
	}
	// Compute sini using Snell's law
	float sint = etai / etat * sqrtf(std::max(0.0f, 1 - cosi * cosi));
	// Total internal reflection
	if (sint >= 1) {
			return 1;
	}
	else {
			float cost = sqrtf(std::max(0.f, 1 - sint * sint));
			cosi = fabsf(cosi);
			float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
			float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
			return (Rs * Rs + Rp * Rp) / 2;
	}
}

vec3 GeometryNode::getReflectedRay(vec3 & in, vec3 & normal) {
		return in - 2 * dot(in, normal) * normal;
}

vec3 GeometryNode::getRefractedRay(glm::vec3 & in, glm::vec3 & normal) {
		float cosi = dot(normalize(in), normal);
    float etai = 1;
		float etat = m_material->getRefractiveIndex();
    vec3 n = normal;
		// ray into the object
    if (cosi < 0) {
			cosi = -cosi;
		} else { // ray out of the object
			swap(etai, etat);
			n = -normal;
		}
    float eta = etai / etat;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0.0f ? vec3(0.0f) : eta * in + (eta * cosi - sqrt(k)) * n;
}

vec3 GeometryNode::getSingleLightSource(vec3 rayDirection, vec3 fragPosition, vec3 fragNormal, Light& light) {
		int numSampleX = light.diameter[0]/cellSize + 1;
		int numSampleY = light.diameter[1]/cellSize + 1;
		int numSampleZ = light.diameter[2]/cellSize + 1;
		fragNormal = getPointNormal(fragNormal, fragPosition);
		vec3 color = vec3(0.0f);
		for (int i = 0; i < numSampleX; i++) {
			float dispositionX = i * cellSize;
			for (int j = 0; j < numSampleY; j++) {
				float dispositionY = j * cellSize;
				for (int k = 0; k < numSampleZ; k++) {
					float dispositionZ = k * cellSize;
					vec3 curLightPos = light.position +
							vec3(dispositionX, dispositionY, dispositionZ);

					float lightDistance = length(curLightPos - fragPosition);
					float fallOff = light.falloff[0] + lightDistance * light.falloff[1] + light.falloff[2] * pow(lightDistance, 2);
					vec3 l = normalize(curLightPos - fragPosition);
			    vec3 v = normalize(-rayDirection);
			    float n_dot_l = fmax(dot(fragNormal, l), 0.0);
					vec3 diffuse;
					diffuse = m_material->getKd() * n_dot_l;
			    vec3 specular = vec3(0.0);
			    if (n_dot_l > 0.0) {
						vec3 h = normalize(v + l);
			      float n_dot_h = fmax(dot(fragNormal, h), 0.0);
			      specular = getPointColor(fragPosition) * float(pow(n_dot_h, m_material->getShininess()));
					}
					color += (diffuse + specular) / fallOff;
				}
			}
		}

		return light.colour * color / float(numSampleX * numSampleY * numSampleZ);
}

vec3 GeometryNode::getPointNormal(vec3 normal, vec3 position) {
	if (hasBump) {
		//vec3 color = vec3(0.0f);
		//vec3 coloru = vec3(0.0f);
		//vec3 colorv = vec3(0.0f);
		if (NonhierSphere* obj = dynamic_cast<NonhierSphere*>(m_primitive)) {
			vec3 nhSphereOrigin = vec3(globalTrans * globalScale * vec4(obj->m_pos, 1.0f));
			position -= nhSphereOrigin;
			float theta = degrees(atan2(position[2],position[0]));
			float phi = degrees(atan2(sqrt(pow(position[0], 2) + pow(position[2], 2)), position[1]));
			uint bumpMapW = m_bumpMap->width();
			uint bumpMapH = m_bumpMap->height();

			uint posW = (uint)((1 - (theta+180)/360) * bumpMapW);
			uint posH = (uint)(phi/180 * bumpMapH);
			float b = (*m_bumpMap)(posW, posH, 0);
			//color[1] = (*m_textureMap)(posW, posH, 1) - 0.1f;
			//color[2] = (*m_textureMap)(posW, posH, 2) - 0.1f;
			//cout << posW << "," << posH << endl;
			float bu = (*m_bumpMap)((posW+2)%bumpMapW, posH, 0) - b;
			//coloru[1] = (*m_textureMap)((posW+1)%textureMapW, posH, 1);
			//coloru[2] = (*m_textureMap)((posW+1)%textureMapW, posH, 2);
			float bv = (*m_bumpMap)(posW, (posH+2)%bumpMapH, 0) - b;
			//colorv[1] = (*m_textureMap)(posW, (posH+1)%textureMapH, 1);
			//colorv[2] = (*m_textureMap)(posW, (posH+1)%textureMapH, 2);

			//cout << posW << ", " << posH << endl;
			vec3 a = vec3(0.0f, 1.0f, 0.0f);
			vec3 t = normalize(cross(a, position));
			vec3 bt = cross(normal, t);
			return normal + bu * cross(normal, bt) + bv * cross(normal, t);
		}
		return normal;
	} else {
		return normal;
	}
}

vec3 GeometryNode::getPointColor(vec3 position) {
	if (hasTexture) {
		vec3 color = vec3(0.0f);
		if (NonhierSphere* obj = dynamic_cast<NonhierSphere*>(m_primitive)) {
			vec3 positionModelCoord = vec3(invglobalScale * invglobalTrans * vec4(position, 1.0f));
			//vec3 nhSphereOrigin = vec3(globalTrans * globalScale * vec4(obj->m_pos, 1.0f));
			positionModelCoord -= obj->m_pos;
			float theta = degrees(atan2(positionModelCoord[2],positionModelCoord[0]));
			float phi = degrees(atan2(sqrt(pow(positionModelCoord[0], 2) + pow(positionModelCoord[2], 2)), positionModelCoord[1]));
			uint textureMapW = m_textureMap->width();
			uint textureMapH = m_textureMap->height();
			uint posW = (uint)((1 - (theta + 180)/360) * textureMapW);
			uint posH = (uint)(phi/180 * textureMapH);
			color[0] = (*m_textureMap)(posW, posH, 0);
			color[1] = (*m_textureMap)(posW, posH, 1);
			color[2] = (*m_textureMap)(posW, posH, 2);
		} else if (NonhierBox* obj = dynamic_cast<NonhierBox*>(m_primitive)) {
			// lock_guard<mutex> lockGuard(m_lock);
			//cout << "here" << endl;
			vec3 posModelCoord = vec3(invglobalScale * invglobalTrans * vec4(position, 1.0f));
			vec3 disposition = posModelCoord - obj->m_pos;
			uint textureMapW = m_textureMap->width();
			uint textureMapH = m_textureMap->height();
			// cout << "disposition: " << to_string(disposition) << endl;
			// cout << "tm dimension" << textureMapW << "," << textureMapH << endl;
			if (abs(disposition[0] - 0) < T0 || abs(disposition[0] - obj->m_size) < T0) {
				uint posH = abs(disposition[1]) * textureMapH;
				uint posW = abs(disposition[2]) * textureMapW;
				color[0] = (*m_textureMap)(posW, posH, 0);
				color[1] = (*m_textureMap)(posW, posH, 1);
				color[2] = (*m_textureMap)(posW, posH, 2);
			} else if (abs(disposition[1] - 0) < T0 || abs(disposition[1] - obj->m_size) < T0) {
				// cout << to_string(disposition) << endl;
				uint posH = abs(disposition[2]) * textureMapH;
				uint posW = abs(disposition[0]) * textureMapW;
				// cout << "ph,pw : " << posH << ", " << posW << endl;
				color[0] = (*m_textureMap)(posW, posH, 0);
				color[1] = (*m_textureMap)(posW, posH, 1);
				color[2] = (*m_textureMap)(posW, posH, 2);
			} else if (abs(disposition[2] - 0) < T0 || abs(disposition[2] - obj->m_size) < T0) {
				uint posH = abs(disposition[0]) * textureMapH;
				uint posW = abs(disposition[1]) * textureMapW;
				color[0] = (*m_textureMap)(posW, posH, 0);
				color[1] = (*m_textureMap)(posW, posH, 1);
				color[2] = (*m_textureMap)(posW, posH, 2);
			}
		}
		//cout << to_string(color) << endl;
		return color;
	} else if (hasPerlinNoise) {
			vec3 baseColour = m_material->getKs();
			//cout << "gNode p "<<m_perlinNoise->p << endl;
			//cout << "gNode"<<(m_perlinNoise->p)[0] << endl;
			float noise = turbulance(position[0], position[1], position[2], 32);
			return noise * baseColour;
	} else {
		return m_material->getKs();
	}
}

float GeometryNode::turbulance(float x, float y, float z, int frequency){
	double value = 0;
	double size = 2;

  while(frequency >= 1)
  {
    value += abs(m_perlinNoise->noise(double(x)/ size, double(y)/ size, double(z)/ size)) * size/2;
    size /= 2.0;
		frequency -= 1;
  }
	return sin(2 * (x + 4 * float(value) + y));
}

vec3 GeometryNode::getPhongColour(SceneNode * root, vec3 rayDirection, vec3 fragPosition, vec3 fragNormal,
	vec3 ambient, list<Light *> lights) {
		vec3 colour = vec3(0.0f);
		for (auto light : lights) {
			int numSampleX = light->diameter[0]/cellSize + 1;
			int numSampleY = light->diameter[1]/cellSize + 1;
			int numSampleZ = light->diameter[2]/cellSize + 1;
			vec3 singleLightSourceColor = vec3(0.0f);
			for (int i = 0; i < numSampleX; i++) {
				float dispositionX = i * cellSize;
				for (int j = 0; j < numSampleY; j++) {
					float dispositionY = j * cellSize;
					for (int k = 0; k < numSampleZ; k++) {
						float dispositionZ = k * cellSize;
						vec3 curLightPos = light->position +
								vec3(dispositionX, dispositionY, dispositionZ);
						vec3 lightRayDirection = curLightPos - fragPosition;
						Ray lightRay = Ray(fragPosition, lightRayDirection);
						if (!isLightSourceBlocked(*root,lightRay)) {
							singleLightSourceColor += getSingleLightSource(rayDirection, fragPosition, fragNormal, *light);
						}
					}
				}
			}
			colour += singleLightSourceColor / float(numSampleX * numSampleY * numSampleZ);
		}
		return ambient + colour;
}

bool GeometryNode::isLightSourceBlocked(SceneNode & node, Ray r) {
	if (node.m_nodeType == NodeType::GeometryNode) {
		GeometryNode& gNode = static_cast<GeometryNode&>(node);
		if (gNode.isBlockedbyMe(r)) {
			return true;
		}
	}
	if (!node.children.empty()) {
		for (auto child : node.children) {
			bool isBlockedBySubtree = isLightSourceBlocked(*child, r);
			if (isBlockedBySubtree) {
				return true;
			}
		}
	}
	return false;
}

bool GeometryNode::isBlockedbyMe(Ray r) {
		double t = 1;
		vec3 normal = vec3(0.0);
		Ray r_m = Ray(vec3(invglobalScale * invglobalTrans * vec4(r.origin, 1.0)), vec3(invglobalScale * invglobalTrans * vec4(r.direction, 0.0)));
		//cout << to_string(invglobalTrans) << endl;
		return m_primitive->hit(r_m, T0, 1, t, normal);
}
