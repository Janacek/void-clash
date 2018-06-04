/********************************************************************
**
**              GraphicDefines.hpp
**              Created by : Vial Joris
**
********************************************************************/

#ifndef VOIDCLASH_GRAPHIC_DEFINES
#define VOIDCLASH_GRAPHIC_DEFINES

// Define some common variable for the graphic engine

#define COEFF_ZOOM_MULT	1700.0f

#define LIFE_SMOOTHING_DELAY 0.25f
#define LIFE_SMOOTHING_SPEED 7500.0f
#define ROTATION_SMOOTH_SPEED 2.5f
#define ROTATION_MAX_Y 60.0f

// Define Z positions

#define Z_SPEEDFIELD 0.0f
#define Z_CAPTURE 1.0f
#define Z_RESPAWN 1.0f
#define Z_WALL 2.0f
#define Z_GRAVITYFIELD 3.0f
#define Z_TURRET 4.0f
#define Z_FLASH 5.0f
#define Z_HALO 5.0f
#define Z_FLAG 6.0f
#define Z_BOMB 8.0f
#define Z_BULLET 9.0f
#define Z_PLAYER 10.0f

// Wall size
#define WALL_SIZE 350.0f

#endif /* end of include guard: VOIDCLASH_GRAPHIC_DEFINES */
