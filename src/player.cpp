#include "player.h"

Player::Player(scene::ISceneNode* parent,scene::ISceneManager* smgr):
  scene::ISceneNode(parent,smgr),
  speed(0,0,0),
  touching_ground(false),
  m_position(0,0,0)
{
  m_box=core::aabbox3d<f32>(-BS,-BS,-BS,BS,BS,BS);
  video::IVideoDriver* driver=SceneManager->getVideoDriver();
  avatar=smgr->getMesh("../resource/character.b3d");
  avatar_node=smgr->addAnimatedMeshSceneNode(avatar,this);
  if(avatar_node){
    avatar_node->setMaterialFlag(video::EMF_LIGHTING,false);
    avatar_node->setMaterialTexture(0,driver->getTexture("../daresourceta/character.png"));
    avatar_node->setScale(v3f(1,1,1));
    avatar_node->setPosition(v3f(0,0,0));
    avatar_node->setRotation(v3f(0,0,0));
    animateStand();
  }
  updateSceneNodePosition();
}

Player::~Player(){
  if (SceneManager!=NULL)
    ISceneNode::remove();
}

void Player::animateStand(){
  avatar_node->setFrameLoop(0,80);
  avatar_node->setAnimationSpeed(32);
}

void Player::animateMove(){
  avatar_node->setFrameLoop(168,188);
  avatar_node->setAnimationSpeed(32);
}

void Player::move(f32 dtime){
  setRotation(m_rotation);
  v3f position=getPosition();
  position+=speed*dtime;
  // The frame length is limited to the player going 0.1*BS per call
  f32 d=(float)BS*0.15;
  setPosition(position);
}

