Globals
{
	Textures
	{
		WHITE_PIECES : string : "Assets/Textures/WhitePieces.png";	
		BLACK_PIECES : string : "Assets/Textures/BlackPieces.png";
		SAVE_ICON    : string : "Assets/Textures/icons-save.png";
	}

	BASIC_BOARD_PATH : string : "Assets/Boards/basic_board.csv";
	BACKGROUND_COLOR : color  : #FF327064; 

	EatParticle
	{
		LIFETIME       : float : 1.5f;
		VEL_X          : float : 130.0f;
		VEL_Y          : float : -125.0f;
		ROTATION       : float : PI / 2.0f;
		ROTATION_VEL   : float : 2.0f;
		ACCEL_X        : float : 0.0f;
		ACCEL_Y        : float : 0.0f;
		ROTATION_ACCEL : float : -1.64f;
		BEGIN_COLOR    : color : #FFB1C7CE;
		END_COLOR      : color : #01356361;
	}
    = eatParticleEmitter = ParticleEmitter(particle, {0.0f, 0.0f}, -1.64f, {177, 199, 206, 255}, {53, 99, 97, 1}, {1.0f, 1.0f}, 2.0f, 10.0f, {0.0f, 0.0f}, 0.04f, 0.41f, 2.0f * PI);

}

