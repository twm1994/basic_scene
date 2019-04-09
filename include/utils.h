#ifndef UTILS_HEADER
#define UTILS_HEADER

#include <irrlicht.h>
using namespace irr;
typedef core::vector3df v3f;
typedef core::vector3d<s16> v3s16;
typedef core::vector2d<s16> v2s16;
typedef core::vector2d<f32> v2f32;
typedef core::vector3d<s32> v3s32;
#define BS 10.0
#define ZOOM_MAX 1.5
#define ZOOM_MIN (-5.0*BS)
#define ZOOM_SPEED (float)(0.02*BS)
#define ROTATE_SPEED 1
#define WALK_ACCELERATION (4.0 * BS)
#define WALKSPEED_MAX (4.0 * BS)
#define PI 3.14159
#define FOV_ANGLE (PI/2.5)
#define PLAYER_RADIUS (BS*0.3)
#define PLAYER_HEIGHT (BS*1.7)
#define PLAYER_EYE_HEIGHT (BS*1.65)
#endif
