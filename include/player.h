#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include <irrlicht.h>
#include "utils.h"
using namespace irr;

class Player : public scene::ISceneNode
{
public:

  Player(scene::ISceneNode* parent,scene::ISceneManager* smgr);

  ~Player();

  void animateStand();

  void animateMove();

  void move(f32 dtime);

  /*
    ISceneNode methods
  */

  virtual void OnRegisterSceneNode()
  {
    if (IsVisible)
      SceneManager->registerNodeForRendering(this);

    ISceneNode::OnRegisterSceneNode();
  }

  virtual void render()
  {
    // Do nothing
  }

  virtual const core::aabbox3d<f32>& getBoundingBox() const
  {
    return m_box;
  }

  v3f getPosition()
  {
    return m_position;
  }

  void setPosition(v3f position)
  {
    m_position=position;
    updateSceneNodePosition();
  }

  v3f getRotation()
  {
    return m_rotation;
  }

  void setRotation(v3f rotation)
  {
    m_rotation=rotation;
    updateSceneNodeRotation();
  }

  void updateSceneNodePosition()
  {
    ISceneNode::setPosition(m_position);
  }

  void updateSceneNodeRotation()
  {
    ISceneNode::setRotation(m_rotation);
  }

  v3f speed;
  bool touching_ground;

private:
  v3f m_position;
  v3f m_rotation;
  scene::IAnimatedMesh*           avatar;
  scene::IAnimatedMeshSceneNode*  avatar_node;
  core::aabbox3d<f32> m_box;
};

#endif