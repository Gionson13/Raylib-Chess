Globals
{
	Textures
	{
		WHITE_PIECES : string : "Assets/Textures/WhitePieces.png";	
		BLACK_PIECES : string : "Assets/Textures/BlackPieces.png";
		SAVE_ICON    : string : "Assets/Textures/icons-save.png";
	}

	BASIC_BOARD_PATH : string : "Assets/Boards/basic_board.csv";

	Colors
	{
		BACKGROUND       : color : #FF143425;
		 
		BOARD_OUTLINE    : color : #FF006697;
		BOARD_SELECTED   : color : #FFc0cf6d;
		BOARD_LEGAL_MOVE : color : #FF528350;
		BOARD_BLACK      : color : #FF222222;
		BOARD_WHITE      : color : #FFF2F2F2;
		
		BUTTON           : color : #FFaa0115;
	}

	EatParticle
	{
		LIFETIME        : float    : 1.5f;
		VELOCITY        : vector2f : {130.0f, -125.0f};
		ROTATION        : float    : PI / 2.0f;
		ROTATION_VEL    : float    : 2.0f;
		ACCELERATION    : vector2f : {0.0f, 0.0f};
		ROTATION_ACCEL  : float    : -1.64f;
		BEGIN_COLOR     : color    : #FFB1C7CE;
		END_COLOR       : color    : #01356361;
		ASPECT_RATIO    : vector2f : {1.0f, 1.0f};
		MIN_SIZE_FACTOR : float    : 2.0f;
		MAX_SIZE_FACTOR : float    : 10.0f;
		INTERVAL        : float    : 0.04f;
		RANDOMNESS      : float    : 0.41f;
		SPREAD          : float    : 2.0f * PI;
	}
}

