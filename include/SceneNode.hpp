// Fall 2018

#pragma once

#include "Material.hpp"

#include <glm/glm.hpp>

#include <list>
#include <string>
#include <iostream>

enum class NodeType {
	SceneNode,
	GeometryNode,
	JointNode
};

class SceneNode {
public:
    SceneNode(const std::string & name);

	SceneNode(const SceneNode & other);

    virtual ~SceneNode();

	int totalSceneNodes() const;

    const glm::mat4& get_transform() const;
    const glm::mat4& get_inverse() const;

		const glm::mat4& get_globalTransform() const;
    const glm::mat4& get_invglobalTransform() const;

    void set_transform(const glm::mat4& m);
		void set_globalTransform(const glm::mat4& m);
		void set_globalScale(const glm::mat4& m);

    void add_child(SceneNode* child);

    void remove_child(SceneNode* child);

	//-- Transformations:
    void rotate(char axis, float angle);
    void scale(const glm::vec3& amount);
    void translate(const glm::vec3& amount);


	friend std::ostream & operator << (std::ostream & os, const SceneNode & node);

		glm::mat4 globalTransCopy;
		glm::mat4 globalTrans;
		glm::mat4 invglobalTrans;
		glm::mat4 globalScale;
		glm::mat4 invglobalScale;
    // Transformations
    glm::mat4 trans;
		glm::mat4 scaleM;
		glm::mat4 invScale;
    glm::mat4 invtrans;

    std::list<SceneNode*> children;

	NodeType m_nodeType;
	std::string m_name;
	unsigned int m_nodeId;

private:
	// The number of SceneNode instances.
	static unsigned int nodeInstanceCount;
};
