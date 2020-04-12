#pragma once

#include <QtCore>

#include "Renderable.h"

class SceneNode
{
protected:
  SceneNode *parent;
public:
  SceneNode();
  virtual ~SceneNode();

  void init(Renderable *renderable, float rotationSpeed_);
  void setLocalTransform(QMatrix4x4 localTransform_);

  // Add a child to this node
  void addChild(SceneNode *child);
  // Update this node and all its children
  void update(float msec);
  // Draw this node's object and all its children
  void draw(const QMatrix4x4 &view, const QMatrix4x4 &projection);
private:
  // Children holds all a pointer to all of the descendents
  // of a particular SceneNode. A pointer is used because
  // we do not want to hold or make actual copies.
  std::vector<SceneNode *> children;
  // The object stored in the scene graph
  Renderable *object;
  // Rotation speed of the object
  float rotationSpeed;
  // Each SceneNode nodes locals transform
  QMatrix4x4 localTransform;
  // We additionally can store the world transform
  QMatrix4x4 worldTransform;
};