#ifdef _MSC_VER
// Define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
// #include "driverChoice.h"
#include "utils.h"
#include "player.h"
using namespace irr;
#include "eventreceiverwarper.h"

int main(){
	video::E_DRIVER_TYPE driverType=video::EDT_OPENGL;
	// create device
	EventReceiverWarper  receiver;

	u16 screenW=800;
	u16 screenH=600;
	IrrlichtDevice* device=createDevice(driverType,core::dimension2d<u32>(screenW, screenH),16,false,false,false,&receiver);

	if (device==0) return 1; // could not create selected driver.

	video::IVideoDriver* driver=device->getVideoDriver();
	scene::ISceneManager* smgr=device->getSceneManager();

	/*
	Create the player_node which will be moved with the WSAD keys. As there is no dynamic lights in this scene
	lighting for each model is disabled (otherwise the models would be black).
	*/
	Player *player=new Player(smgr->getRootSceneNode(),smgr);
	if (player){
		player->setPosition(v3f(0,0,30));
		player->setMaterialFlag(video::EMF_LIGHTING,false);
	}
	receiver.setControlPlayer(player);
	Player *dummy_player=new Player(smgr->getRootSceneNode(),smgr);
	if (dummy_player){
		dummy_player->setPosition(v3f(0,0,130));
		dummy_player->setMaterialFlag(video::EMF_LIGHTING,false);
	}
	// player_node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../resource/character.b3d"));
	// if (player_node)
	// {
	// 	player_node->setPosition(v3f(0,0,30));
	// 	player_node->setRotation(v3f(0, 0, 0));
	// 	player_node->setMaterialTexture(0, driver->getTexture("../resource/character.png"));
	// 	player_node->setMaterialFlag(video::EMF_LIGHTING, false);
	// 	player_node->setFrameLoop(0, 80);
	// 	player_node->setAnimationSpeed(15);
	// 	player_node->setScale(v3f(1, 1, 1));
	// }
	// scene::IAnimatedMeshSceneNode * node1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("../resource/character.b3d"));
	// if (node1)
	// {
	// 	node1->setPosition(v3f(0, 0, 130));
	// 	node1->setRotation(v3f(0, 0, 0));
	// 	node1->setMaterialTexture(0, driver->getTexture("../resource/character.png"));
	// 	node1->setMaterialFlag(video::EMF_LIGHTING, false);
	// 	node1->setFrameLoop(0, 80);
	// 	node1->setAnimationSpeed(15);
	// 	node1->setScale(v3f(1, 1, 1));
	// };
	scene::ICameraSceneNode* camera=smgr->addCameraSceneNode(
		0, // Camera parent
		v3f(BS*100,BS*2,BS*100), // Look from
		v3f(BS*100+1,BS*2,BS*100), // Look to
		-1 // Camera ID
	);
	if (camera==NULL) return 1;
	camera->setFOV(PI/2.5);
	// Just so big a value that everything rendered is visible
	camera->setFarValue(BS*1000);

	f32 camera_yaw=0; // "right/left"
	f32 camera_pitch=0; // "up/down"

	// camera zoom distance control
	f32 zoom_max=ZOOM_MAX;
	f32 zoom_min=ZOOM_MIN;
	f32 zoom_speed=ZOOM_SPEED;
	f32 camera_zoom=0;

	// camera rotate control
	f64 rotate_speed=ROTATE_SPEED;
	f64 camera_rotate=0;

	f32 walk_acceleration=WALK_ACCELERATION;
	f32 walkspeed_max=WALKSPEED_MAX;

	device->getCursorControl()->setVisible(false);

	int lastFPS=-1;

	// For frame rate independent movement,
	// Time is in milliseconds
	u32 lasttime=device->getTimer()->getTime();

	// This is the movemen speed in units per second.
	const f32 MOVEMENT_SPEED=5.f;

	while(device->run()){
		// Work out a frame delta time.
		u32 time=device->getTimer()->getTime();
		f32 dtime; // in seconds
		if (time>lasttime)
			dtime=(time-lasttime)/1000.0;
		else
			dtime=0;
		lasttime=time;

		if (receiver.IsKeyDown(irr::KEY_ESCAPE)){
			break;
		}
		core::vector3df nodePosition=player->getPosition();
		v3f zoom_direction=v3f(0,0,1);
		zoom_direction.rotateXZBy(camera_yaw);
		/*Camera zoom*/
		if (receiver.IsKeyDown(irr::KEY_UP)){
			camera_zoom+=zoom_speed;
		}

		if (receiver.IsKeyDown(irr::KEY_DOWN)){
			camera_zoom-=zoom_speed;
		}
		if (camera_zoom<zoom_min){
			camera_zoom=zoom_min;
		}
		else if (camera_zoom>zoom_max){
			camera_zoom=zoom_max;
		}

		/*Camera rotate*/
		if (receiver.IsKeyDown(irr::KEY_LEFT)){
			camera_rotate-=rotate_speed;
		}

		if (receiver.IsKeyDown(irr::KEY_RIGHT)){
			camera_rotate+=rotate_speed;
		}
		/*
			Player speed control
		*/

		v3f move_direction=v3f(0,0,1);
		move_direction.rotateXZBy(camera_yaw);

		v3f speed=v3f(0,0,0);
		if (receiver.IsKeyDown(irr::KEY_KEY_W)){
			speed+=move_direction;
		}
		if (receiver.IsKeyDown(irr::KEY_KEY_S)){
			speed-=move_direction;
		}
		if (receiver.IsKeyDown(irr::KEY_KEY_A)){
			speed+=move_direction.crossProduct(v3f(0, 1, 0));
		}
		if (receiver.IsKeyDown(irr::KEY_KEY_D)){
			speed+=move_direction.crossProduct(v3f(0,-1,0));
		}

		speed=speed.normalize()*walkspeed_max;

		//if (speed.X>0||speed.Z>0)
		//{
		//	player->setFrameLoop(168, 188);
		//	player->setAnimationSpeed(32);
		//	player->setLoopMode(false);
		//}
		//else
		//{
		//	player->setFrameLoop(0, 80);
		//	player->setLoopMode(true);
		//}
		nodePosition+=speed*dtime;

		player->setPosition(nodePosition);

		s32 dx=device->getCursorControl()->getPosition().X-screenW/2;
		s32 dy=device->getCursorControl()->getPosition().Y-screenH/2;
		camera_yaw-=dx*0.2;
		camera_pitch+=dy*0.2;
		if (camera_pitch<-89.9)camera_pitch=-89.9; // look up
		if (camera_pitch>89.9)camera_pitch=89.9; // look down
		device->getCursorControl()->setPosition(screenW/2,screenH/2);

		v3f camera_direction = v3f(0,0,1);
		camera_direction.rotateYZBy(camera_pitch);
		camera_direction.rotateXZBy(camera_yaw);
		zoom_direction.rotateXZBy(camera_rotate);
		camera_direction.rotateXZBy(camera_rotate);
		player->setRotation(v3f(0,-1*camera_yaw,0));
		player->setPosition(nodePosition);
		v3f camera_position=player->getPosition()+v3f(0,PLAYER_EYE_HEIGHT,zoom_max)+zoom_direction*camera_zoom;

		camera->setPosition(camera_position);
		camera->setTarget(camera_position+camera_direction);
		driver->beginScene(true,true,video::SColor(255,113,113,133));

		smgr->drawAll(); // draw the 3d scene
		device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

		driver->endScene();

		int fps=driver->getFPS();

		if (lastFPS!=fps){
			core::stringw tmp(L"Movement Example - Irrlicht Engine [");
			tmp+=driver->getName();
			tmp+=L"] fps: ";
			tmp+=fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS=fps;
		}
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	device->drop();

	return 0;
}