// This file is auto generated
#pragma once

#include <string>

#ifndef RL_COLOR_TYPE
	// Color, 4 components, R8G8B8A8 (32bit)
	typedef struct Color {
		unsigned char r;        // Color red value
		unsigned char g;        // Color green value
		unsigned char b;        // Color blue value
		unsigned char a;        // Color alpha value
	} Color;
	#define RL_COLOR_TYPE
#endif

#ifndef PI
	#define PI 3.14159265358979323846f
#endif
#ifndef E
	#define E 2.71828182845904523536f
#endif

namespace Globals
{
	namespace Textures
	{
		const std::string WHITE_PIECES = "Assets/Textures/WhitePieces.png";
		const std::string BLACK_PIECES = "Assets/Textures/BlackPieces.png";
		const std::string SAVE_ICON = "Assets/Textures/icons-save.png";
	} // namespace Textures

	namespace EatParticle
	{
		const float LIFETIME = 1.5f;
		const float VEL_X = 130.0f;
		const float VEL_Y = -125.0f;
		const float ROTATION = PI/2.0f;
		const float ROTATION_VEL = 2.0f;
		const float ACCEL_X = 0.0f;
		const float ACCEL_Y = 0.0f;
		const float ROTATION_ACCEL = -1.64f;
		const Color BEGIN_COLOR = {177, 199, 206, 255};
		const Color END_COLOR = {53, 99, 97, 1};
	} // namespace EatParticle

	const std::string BASIC_BOARD_PATH = "Assets/Boards/basic_board.csv";
} // namespace Globals

