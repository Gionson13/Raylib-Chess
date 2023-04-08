#include "GameScreen.h"

#include "Globals.hpp"
#include "Variables.h"
#include "MenuScreen.h"
#include "Layers/SettingsLayer.h"

#include <Difu/ScreenManagement/ScreenManager.h>
#include <Difu/Utils/Logger.h>
#include <Difu/Utils/Timer.h>
#include <Difu/WindowManagement/WindowManager.h>
#include <Difu/Animation/KeyFrameAnimation.h>

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <raylib.h>
#include <raymath.h>
#include <sstream>
#include <string>




namespace GameScreen
{
    static Texture2D whitePiecesTexture;
    static Texture2D blackPiecesTexture;
    static Texture2D saveIconTexture;

    static int boardX, boardY;
    static Board board;

    static Vector2i selectedPiece = {-1, -1};

    static BannerAnimation saveBanner;

    static ParticleEmitter eatParticleEmitter;

    static int boardSquareSize = 50; 

	static bool askQueening;
	static Vector2i queeningMovePos;

	static Layer settingsLayer;
	static bool settings = false;
	static Animation::KeyFrameAnimation settingsAnim;
	static const int SETTING_X_ANIMATION = 1;

    Screen GetScreen()
    {
        Screen screen;
        screen.LoadFunction = &Load;
        screen.UnloadFunction = &Unload;
        screen.UpdateFunction = &Update;
        screen.RenderFunction = &Render;
        screen.IsEndTransitionDoneFunction = &IsEndTransitionDone;
        screen.IsStartTransitionDoneFunction = &IsStartTransitionDone;
        screen.RenderStartTransitionFunction = &RenderStartTransition;
        screen.RenderEndTransitionFunction = &RenderEndTransition;
		screen.OnResize = &OnResize;

        return screen;
    }

void Load()
{
    whitePiecesTexture = LoadTexture(Globals::Textures::WHITE_PIECES.c_str());
    blackPiecesTexture = LoadTexture(Globals::Textures::BLACK_PIECES.c_str());
    saveIconTexture = LoadTexture(Globals::Textures::SAVE_ICON.c_str());

    // SetTextureFilter(whitePiecesTexture, TEXTURE_FILTER_POINT);
    // SetTextureFilter(blackPiecesTexture, TEXTURE_FILTER_POINT);

	std::string boardPath = "";
    if (Variables::BoardFilePath.empty())
        boardPath = Globals::BASIC_BOARD_PATH;
    else
		boardPath = Variables::BoardFilePath;

    if (!LoadBoard(board, boardPath))
        ScreenManager::ChangeScreen(MenuScreen::GetScreen());

    Variables::BoardFilePath = "";

    board.whiteLegalMoves = GetLegalMoves(board, PieceColor::White);
    board.blackLegalMoves = GetLegalMoves(board, PieceColor::Black);

	askQueening = false;
	queeningMovePos = {-1, -1};

    eatParticleEmitter = ParticleEmitter({0.0f, 0.0f}, Globals::EatParticle::VELOCITY, Globals::EatParticle::ACCELERATION, Globals::EatParticle::CENTRIPETAL_ACCEL, Globals::EatParticle::ROTATION, Globals::EatParticle::ROTATION_VEL, Globals::EatParticle::ROTATION_ACCEL, Globals::EatParticle::BEGIN_COLOR, Globals::EatParticle::END_COLOR, Globals::EatParticle::ASPECT_RATIO, Globals::EatParticle::MIN_SIZE_FACTOR, Globals::EatParticle::MAX_SIZE_FACTOR, Globals::EatParticle::LIFETIME, Globals::EatParticle::INTERVAL, Globals::EatParticle::RANDOMNESS, Globals::EatParticle::SPREAD);

	settingsLayer = SettingsLayer::GetLayer();
	settingsLayer.y = 0;
	settingsLayer.Load();

	settingsAnim = Animation::KeyFrameAnimation(0.2f, Animation::AnimationType::SINGLE_SHOT);
	settingsAnim.AddKey(SETTING_X_ANIMATION, GetScreenWidth(), GetScreenWidth() - 500.0f);
}

void Unload()
{
    UnloadTexture(whitePiecesTexture);
    UnloadTexture(blackPiecesTexture);
    UnloadTexture(saveIconTexture);

    board.cells.clear();
    board.whiteLegalMoves.clear();
    board.blackLegalMoves.clear();

	settingsLayer.Unload();
}

void Update(float dt)
{
	eatParticleEmitter.Update(dt);

	if (IsKeyPressed(KEY_H))
	{
		settings = !settings;
		settingsAnim.SetReversed(!settings);
		settingsAnim.SetPlaying(true);
	}
        
	Vector2 mousePos = GetMousePosition();
	if (askQueening)
	{
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
		{
			Vector2i askPos = {(GetScreenWidth() - 4 * boardSquareSize) / 2, (GetScreenHeight() - boardSquareSize) / 2};

			if (CheckCollisionPointRec(mousePos, {(float)askPos.x, (float)askPos.y, 4.0f * boardSquareSize, (float)boardSquareSize}))
			{
				Vector2 normalizedMousePos =  {mousePos.x - askPos.x, mousePos.y - askPos.y};

				int index = (int)(normalizedMousePos.x / boardSquareSize);

				board.cells[queeningMovePos].second = (PieceType)(index + 1);

				board.isWhiteTurn = !board.isWhiteTurn;
				board.whiteLegalMoves.clear();
				board.blackLegalMoves.clear();
				board.whiteLegalMoves = GetLegalMoves(board, PieceColor::White);
				board.blackLegalMoves = GetLegalMoves(board, PieceColor::Black);
				RemoveCheckMoves(board);

				queeningMovePos = {-1, -1};
				askQueening = false;
			}

		}
	}
	else
	{
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (!settings || mousePos.x < GetScreenWidth() - 500))
		{ 
			Vector2 boardMousePos = {(mousePos.x - boardX) / boardSquareSize, (mousePos.y - boardY) / boardSquareSize};
			if (CheckCollisionPointRec(mousePos, {(float)boardX, (float)boardY, 8.0f * boardSquareSize, 8.0f * boardSquareSize}))
			{
				Vector2i boardPosi = {(int)boardMousePos.x, (int)boardMousePos.y};
				if ((selectedPiece.x == -1 && selectedPiece.y == -1) || board.cells[boardPosi].first == (board.isWhiteTurn ? PieceColor::White : PieceColor::Black))
				{
					LOG_INFO("Selecting piece");
					selectedPiece = boardPosi;
					std::pair<PieceColor, PieceType> selected = board.cells[selectedPiece];
					if (selected.second == PieceType::None || (board.isWhiteTurn && selected.first == PieceColor::Black) || (!board.isWhiteTurn && selected.first == PieceColor::White))
						selectedPiece = {-1, -1};
				}
			}
		}

		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && (!settings || mousePos.x < GetScreenWidth() - 500))
		{
			Vector2 boardMousePos = {(mousePos.x - boardX) / boardSquareSize, (mousePos.y - boardY) / boardSquareSize};
			if (CheckCollisionPointRec(mousePos, {GetScreenWidth() - 50.0f, 0.0f, 50, 56})) // Save button
			{
				SaveBoard(board);
				saveBanner.Start();
			}
			else if (!CheckCollisionPointRec(mousePos, {(float)boardX, (float)boardY, 8.0f * boardSquareSize, 8.0f * boardSquareSize}))
			{
				LOG_INFO("Out of board");
				selectedPiece = {-1, -1};
			}
			else if (selectedPiece.x != -1 || selectedPiece.y != -1)
			{
				Vector2i movePos = {(int)boardMousePos.x, (int)boardMousePos.y};
				if (board.cells[movePos].first != (board.isWhiteTurn ? PieceColor::White : PieceColor::Black))
				{
					bool isEating = IsEating(board, selectedPiece, movePos);
					bool enPassantEating = board.cells[movePos].first == PieceColor::NoColor;
					bool isQueening = IsQueening(board, selectedPiece, movePos);
					LOG_INFO("Changing piece position");
					if (MovePiece(board, selectedPiece, movePos))
					{
						if (isEating)
						{
							Vector2i particleSpawnPos = movePos;
							if (enPassantEating)
							{
								if (movePos.y == 2)
									particleSpawnPos.y += 1;
								else
									particleSpawnPos.y -= 1;
							}

							eatParticleEmitter.SetSpawnPosition({boardX + particleSpawnPos.x * boardSquareSize + boardSquareSize / 2.0f, boardY + particleSpawnPos.y * boardSquareSize + boardSquareSize / 2.0f});
							eatParticleEmitter.EmitNow(16);
						}

						if (isQueening)
						{
							askQueening = true;
							queeningMovePos = movePos;
						}
						else
						{
							board.isWhiteTurn = !board.isWhiteTurn;
							board.whiteLegalMoves.clear();
							board.blackLegalMoves.clear();
							board.whiteLegalMoves = GetLegalMoves(board, PieceColor::White);
							board.blackLegalMoves = GetLegalMoves(board, PieceColor::Black);
							RemoveCheckMoves(board);
						}
					}
					selectedPiece = {-1, -1};
				}
			}
		}
    } 

	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
	{
		if (CheckCollisionPointRec(mousePos, {0, 0, 50, 56})) // Go back to main menu
			ScreenManager::ChangeScreen(MenuScreen::GetScreen());
	}

    if (saveBanner.IsDone())
        saveBanner = BannerAnimation(4.0f, "SAVED", {(float)GetScreenWidth() + 100.0f, 70.0f}, {-100.0f, 70.0f}, 20, WHITE, RED);
    saveBanner.Update(dt);

	settingsAnim.Update(dt);
	settingsLayer.x = settingsAnim.GetKey(SETTING_X_ANIMATION);
	if (settings)
	{
		if(settingsLayer.Update(dt))
		{
			settings = false;
			settingsAnim.SetReversed(true);
			settingsAnim.SetPlaying(true);
		}
	}
	else 
	{
		if (IsKeyPressed(KEY_ESCAPE))
			WindowManager::CloseWindow();
	}

}

void Render()
{
    ClearBackground(Globals::Colors::BACKGROUND);
    DrawBoard();
    eatParticleEmitter.Render();
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            DrawPiece(x, y, board.cells[{x, y}].second, board.cells[{x, y}].first);
    if (IsInCheck(board, PieceColor::White))
    {
        if (board.whiteLegalMoves.size() == 0)
            DrawText("Checkmate", 20, 200, 32, {234, 234, 234, 255});
        else
            DrawText("Check", 50, 200, 32, {234, 234, 234, 255});
    }
    else
    {
        if (board.whiteLegalMoves.size() == 0)
            DrawText("Stalemate", 20, 200, 32, {234, 234, 234, 255});
    }
    if (IsInCheck(board, PieceColor::Black))
    {
        if (board.blackLegalMoves.size() == 0)
            DrawText("Checkmate", 20, 200, 32, {32, 32, 32, 255});
        else
            DrawText("Check", 50, 200, 32, {32, 32, 32, 255});
    }
    else
    {
        if (board.blackLegalMoves.size() == 0)
            DrawText("Stalemate", 20, 200, 32, {32, 32, 32, 255});
    }

	if (askQueening)
	{
		DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Globals::Colors::DIALOG_SHADE);

		Rectangle dialogRec = {(GetScreenWidth() - 4.0f * boardSquareSize) / 2.0f, (GetScreenHeight() - boardSquareSize) / 2.0f, 4.0f * boardSquareSize, (float)boardSquareSize };

		DrawRectangleRec(dialogRec, Globals::Colors::DIALOG_BACKGROUND);
		Vector2 mousePos = GetMousePosition();
		if (CheckCollisionPointRec(mousePos, dialogRec))
		{
				Vector2 normalizedMousePos =  {mousePos.x - dialogRec.x, mousePos.y - dialogRec.y};
				int index = (int)(normalizedMousePos.x / boardSquareSize);

				DrawRectangleRec({dialogRec.x + index * boardSquareSize, dialogRec.y, (float)boardSquareSize, dialogRec.height}, Globals::Colors::DIALOG_HOVER);
		}

		if (board.isWhiteTurn)
		{
			Rectangle sourceRect = {whitePiecesTexture.width / 6.0f, 0.0f, whitePiecesTexture.width / 6.0f * 4.0f, (float)whitePiecesTexture.height};
			DrawTexturePro(whitePiecesTexture, sourceRect, dialogRec, {0.0f, 0.0f}, 0.0f, WHITE);
		}
		else
		{
			Rectangle sourceRect = {blackPiecesTexture.width / 6.0f, 0.0f, blackPiecesTexture.width / 6.0f * 4.0f, (float)blackPiecesTexture.height};
			DrawTexturePro(blackPiecesTexture, sourceRect, dialogRec, {0.0f, 0.0f}, 0.0f, WHITE);
		}
		DrawRectangleLinesEx(dialogRec, 3.0f, Globals::Colors::DIALOG_OUTLINE);
	}

    // UI
	// if (!settings)
	// {
	DrawRectangle(10, 25, 40, 6, WHITE);
	DrawRectanglePro({7, 28, 25, 6}, {0, 0}, -45.0f, WHITE);
	DrawRectanglePro({7, 28, 25, 6}, {0, 6}, 45.0f, WHITE);
	// }
    
    DrawTextureEx(saveIconTexture, {GetScreenWidth() - 45.0f, 5.0f}, 0.0f, 0.8f, WHITE);

    saveBanner.Render();

	settingsLayer.Render();

    if (Variables::RenderFPS)
        DrawFPS(0, 0);
}


void RenderStartTransition(float time)
{
    float progress = time / 1.3f;

    DrawRectangle(0, 0, GetScreenWidth(), (int)Lerp((float)GetScreenHeight() / 2, 0, progress), BLACK);
    DrawRectangle(0, (int)Lerp((float)GetScreenHeight() / 2, GetScreenHeight(), progress), GetScreenWidth(), GetScreenHeight(), BLACK);
}

bool IsStartTransitionDone(float time)
{
    return time / 1.3f > 1.0f;
}

void RenderEndTransition(float time)
{
    float progress = time / 1.3f;

    if (progress * 2 < 1.1f)
        DrawRectangleGradientH(0, 0, (int)Lerp(0, GetScreenWidth(), progress * 2), GetScreenHeight(), BLACK, {1, 95, 108, 255});
    else
    {
        DrawRectangleGradientH((int)Lerp(0, GetScreenWidth(), (progress - 0.5f)* 2), 0, GetScreenWidth(), GetScreenHeight(), BLACK, {1, 95, 108, 255});
        DrawRectangle(0, 0,(int)Lerp(0, GetScreenWidth(), (progress - 0.5f) * 2), GetScreenHeight(), BLACK);
    }
    

}

bool IsEndTransitionDone(float time)
{
    return time / 1.3f > 1.0f;
}

void OnResize(int width, int height)
{
    LOG_INFO("OnResize called");

    if (height < width)
        boardSquareSize = (int)(height / 9.6f);
    else
        boardSquareSize = width / 16;

    boardX = (int)(width / 2 - 8 * boardSquareSize / 2);
    boardY = (int)(height / 2 - 8 * boardSquareSize / 2);

    saveBanner = BannerAnimation(4.0f, "SAVED", {(float)width + 100.0f, 70.0f}, {-100.0f, 70.0f}, 20, WHITE, RED);

	settingsLayer.Resize(500, height);

	settingsAnim.RemoveKey(SETTING_X_ANIMATION);
	settingsAnim.AddKey(SETTING_X_ANIMATION, width, width - 500.0f);
}


void DrawBoard()
{
    DrawRectangle(boardX - 1, boardY - 1, boardSquareSize * 8 + 2, boardSquareSize * 8 + 2, Globals::Colors::BOARD_OUTLINE);

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            Color squareColor;
            if (x == selectedPiece.x && y == selectedPiece.y)
                squareColor = Globals::Colors::BOARD_SELECTED;
            else if (selectedPiece.x != -1 && selectedPiece.y != -1 && IsLegalMove(board.isWhiteTurn ? board.whiteLegalMoves : board.blackLegalMoves, selectedPiece, {x, y}))
                squareColor = Globals::Colors::BOARD_LEGAL_MOVE;
            else if ((y * 9 + x) % 2 == 0)
                squareColor = Globals::Colors::BOARD_WHITE;
            else
                squareColor = Globals::Colors::BOARD_BLACK;
            DrawRectangle(boardX + x * boardSquareSize, boardY + y * boardSquareSize, boardSquareSize, boardSquareSize, squareColor);
        }
    }
}

void DrawPiece(int x, int y, PieceType type, PieceColor color)
{
    Rectangle destRect = {(float)(boardX + x * boardSquareSize) + 2.0f, (float)(boardY + y * boardSquareSize), boardSquareSize - 4.0f, boardSquareSize - 4.0f};
    if (type != PieceType::None)
    {
        if (color == PieceColor::White)
		{
			Rectangle sourceRect = {(int)type * (whitePiecesTexture.width / 6.0f), 0.0f, whitePiecesTexture.width / 6.0f, (float)whitePiecesTexture.height};
            DrawTexturePro(whitePiecesTexture, sourceRect, destRect, {0.0f, 0.0f}, 0.0f, WHITE);
        }
        else if (color == PieceColor::Black)
        {
			Rectangle sourceRect = {(int)type * (blackPiecesTexture.width / 6.0f), 0.0f, blackPiecesTexture.width / 6.0f, (float)blackPiecesTexture.height};
            DrawTexturePro(blackPiecesTexture, sourceRect, destRect, {0.0f, 0.0f}, 0.0f, WHITE);
        }
    }
}
} // namespace GameScreen
