#include "SceneNode.h"

SceneNode::SceneNode()
{
  parent = NULL;
  object = NULL;
  rotationSpeed = 0.0f;
  localTransform.setToIdentity();
  worldTransform.setToIdentity();
}

SceneNode::~SceneNode()
{
  for (unsigned int i = 0; i < children.size(); ++i) {
    delete children[i];
  }
}

void SceneNode::init(Renderable *renderable, float rotationSpeed_) {
  object = renderable;
  rotationSpeed = rotationSpeed_;
}

void SceneNode::setLocalTransform(QMatrix4x4 localTransform_) {
  localTransform = localTransform_;
}

// Add a child to this node
void SceneNode::addChild(SceneNode *child) {
  children.push_back(child);
  child->parent = this;
}

// Update this node and all its children
void SceneNode::update(float msec) {
  // Apply rotation
  QMatrix4x4 rotMatrix;
  rotMatrix.rotate(msec / 100 * rotationSpeed, QVector3D(0, 1, 0));
  localTransform = localTransform * rotMatrix;

  worldTransform = parent ? parent->worldTransform * localTransform : localTransform;
  for (std::vector<SceneNode *>::iterator i = children.begin(); i != children.end(); ++i) {
    (*i)->update(msec);
  }
}

// Draw this node's object and all its children
void SceneNode::draw(const QMatrix4x4 &view, const QMatrix4x4 &projection) {
  if (object != NULL) {
    object->setModelMatrix(worldTransform);
    object->draw(view, projection);
  }
  for (std::vector<SceneNode *>::iterator i = children.begin(); i != children.end(); ++i) {
    (*i)->draw(view, projection);
  }
}