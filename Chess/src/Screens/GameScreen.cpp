#include "GameScreen.h"

#include "../Globals.h"
#include "../ScreenManagement/ScreenManager.h"
#include "../Utils/Logger.h"
#include "../Utils/Timer.h"
#include "../WindowManagement/WindowManager.h"
#include "MenuScreen.h"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <raylib.h>
#include <raymath.h>
#include <sstream>
#include <string>

Board::Board()
{
    isWhiteTurn = true;
    whiteRightCastlingStillPossible = true;
    whiteLeftCastlingStillPossible = true;
    blackRightCastlingStillPossible = true;
    blackLeftCastlingStillPossible = true;
    pawnMovedTwice = {-1, -1};

    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            board[{x, y}] = {PieceColor::NoColor, PieceType::None};
}

void GameScreen::Load()
{
    whitePiecesTexture = LoadTexture("Assets/Textures/WhitePieces.png");
    blackPiecesTexture = LoadTexture("Assets/Textures/BlackPieces.png");
    saveIconTexture = LoadTexture("Assets/Textures/icons-save.png");

    SetTextureFilter(whitePiecesTexture, TEXTURE_FILTER_POINT);
    SetTextureFilter(blackPiecesTexture, TEXTURE_FILTER_POINT);

    OnResize();

    if (Globals::BoardFilePath.empty())
        LoadBoard("Assets/Boards/basic_board.csv");
    else
        LoadBoard(Globals::BoardFilePath);
    Globals::BoardFilePath = "";
    LOG_INFO("Loaded board");

    board.whiteLegalMoves = GetLegalMoves(PieceColor::White, board);
    board.blackLegalMoves = GetLegalMoves(PieceColor::Black, board);

    Particle particle;
    particle.size = {8.0f, 8.0f};
    particle.velocity = {80.0f, -75.0f};
    particle.acceleration = {0.0f, 0.0f};
    particle.rotation = PI / 2;
    particle.rotationVelocity = 2.0f;
    particle.rotationAcceleration = -1.64f;
    particle.startColor = {177, 199, 206, 255};
    particle.endColor = {53, 99, 97, 1};

    eatParticleSystem = ParticleSystem(particle, {0.0f, 0.0f}, 2.0f, 0.04f, 0.41f, 2.0f * PI);
}

void GameScreen::Unload()
{
    UnloadTexture(whitePiecesTexture);
    UnloadTexture(blackPiecesTexture);
    UnloadTexture(saveIconTexture);

    board.board.clear();
    board.whiteLegalMoves.clear();
    board.blackLegalMoves.clear();
}

void GameScreen::Update(float dt)
{
    if (IsWindowResized())
        OnResize();

    eatParticleSystem.Update(dt);
    if (eatParticleSystem.IsEmitting())
    {
        particleTimer += dt;

        if (particleTimer > 0.4f)
        {
            eatParticleSystem.StopEmitting();
        }
    }
    
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetMousePosition();
        Vector2 boardMousePos = {(mousePos.x - boardX) / boardSquareSize, (mousePos.y - boardY) / boardSquareSize};
        if (CheckCollisionPointRec(mousePos, {(float)boardX, (float)boardY, 8.0f * boardSquareSize, 8.0f * boardSquareSize}))
        {
            Vector2i boardPosi = {(int)boardMousePos.x, (int)boardMousePos.y};
            if (selectedPiece.x == -1 && selectedPiece.y == -1 || board.board[boardPosi].first == (board.isWhiteTurn ? PieceColor::White : PieceColor::Black))
            {
                LOG_INFO("Selecting piece");
                selectedPiece = boardPosi;
                std::pair<PieceColor, PieceType> selected = board.board[selectedPiece];
                if (selected.second == PieceType::None || (board.isWhiteTurn && selected.first == PieceColor::Black) || (!board.isWhiteTurn && selected.first == PieceColor::White))
                    selectedPiece = {-1, -1};
            }
        }
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetMousePosition();
        Vector2 boardMousePos = {(mousePos.x - boardX) / boardSquareSize, (mousePos.y - boardY) / boardSquareSize};
        if (CheckCollisionPointRec(mousePos, {0, 0, 50, 56}))
        {
            ScreenManager::ChangeScreen<MenuScreen>();
        }
        else if (CheckCollisionPointRec(mousePos, {GetScreenWidth() - 50.0f, 0.0f, 50, 56}))
        {
            SaveBoard();
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
            if (board.board[movePos].first != (board.isWhiteTurn ? PieceColor::White : PieceColor::Black))
            {
                bool isEating = false;
                LOG_INFO("Changing piece positionq");
                if (board.board[movePos].first != PieceColor::NoColor)
                    isEating = true;
                if (MovePiece(selectedPiece, movePos, board))
                {
                    if (isEating)
                    {
                        particleTimer = 0.0f;
                        eatParticleSystem.SetSpawnPosition({boardX + movePos.x * boardSquareSize + boardSquareSize / 2.0f, boardY + movePos.y * boardSquareSize + boardSquareSize / 2.0f});
                        eatParticleSystem.StartEmitting();
                    }

                    board.isWhiteTurn = !board.isWhiteTurn;
                    board.whiteLegalMoves.clear();
                    board.blackLegalMoves.clear();
                    board.whiteLegalMoves = GetLegalMoves(PieceColor::White, board);
                    board.blackLegalMoves = GetLegalMoves(PieceColor::Black, board);
                    RemoveCheckMoves();
                }
                selectedPiece = {-1, -1};
            }
        }
    }
    if (saveBanner.IsDone())
        saveBanner = BannerAnimation(4.0f, "SAVED", {(float)GetScreenWidth() + 100.0f, 70.0f}, {-100.0f, 70.0f}, 20, WHITE, RED);
    saveBanner.Update(dt);
}

void GameScreen::Render()
{
    ClearBackground({51, 76, 76, 255});
    DrawBoard();
    eatParticleSystem.Render();
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++)
            DrawPiece(x, y, board.board[{x, y}].second, board.board[{x, y}].first);
    if (IsInCheck(PieceColor::White, board))
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
    if (IsInCheck(PieceColor::Black, board))
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

    // UI
    DrawRectangle(10, 25, 40, 6, WHITE);
    DrawRectanglePro({7, 28, 25, 6}, {0, 0}, -45.0f, WHITE);
    DrawRectanglePro({7, 28, 25, 6}, {0, 6}, 45.0f, WHITE);
    
    DrawTextureEx(saveIconTexture, {GetScreenWidth() - 45.0f, 5.0f}, 0.0f, 0.8f, WHITE);

    saveBanner.Render();
}


void GameScreen::RenderStartTransition(float time)
{
    float progress = time / 1.3f;

    DrawRectangle(0, 0, GetScreenWidth(), (int)Lerp((float)GetScreenHeight() / 2, 0, progress), BLACK);
    DrawRectangle(0, (int)Lerp((float)GetScreenHeight() / 2, GetScreenHeight(), progress), GetScreenWidth(), GetScreenHeight(), BLACK);
}

bool GameScreen::IsStartTransitionDone(float time)
{
    return time / 1.3f > 1.0f;
}

void GameScreen::RenderEndTransition(float time)
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

bool GameScreen::IsEndTransitionDone(float time)
{
    return time / 1.3f > 1.0f;
}

void GameScreen::OnResize()
{
    LOG_INFO("OnResize called");

    if (GetScreenHeight() < GetScreenWidth())
        boardSquareSize = (int)(GetScreenHeight() / 9.6f);
    else
        boardSquareSize = GetScreenWidth() / 16;

    boardX = (int)(GetScreenWidth() / 2 - 8 * boardSquareSize / 2);
    boardY = (int)(GetScreenHeight() / 2 - 8 * boardSquareSize / 2);

    saveBanner = BannerAnimation(4.0f, "SAVED", {(float)GetScreenWidth() + 100.0f, 70.0f}, {-100.0f, 70.0f}, 20, WHITE, RED);
}

void GameScreen::LoadBoard(std::string filename)
{
    std::ifstream fileStream(filename);
    if (!fileStream)
    {
        LOG_WARN("Failed to open " + filename);
        ScreenManager::ChangeScreen<MenuScreen>();
        return;
    }

    std::stringstream strStream;
    strStream << fileStream.rdbuf();

    fileStream.close();

    std::string tmpLine;
    int countX = 0, countY = 0;

    while (strStream >> tmpLine)
    {
        if (countY == 8)
        {
            LOG_WARN("Rows exceed 8");
            break;
        }

        std::string tmp;
        std::stringstream lineStream(tmpLine);
        while (std::getline(lineStream, tmp, ','))
        {
            if (countX == 8)
            {
                if (countY == 0)
                    board.isWhiteTurn = tmp == "0" ? false : true;
                else if (countY == 1)
                    board.whiteLeftCastlingStillPossible = tmp == "0" ? false : true;
                else if (countY == 2)
                    board.whiteRightCastlingStillPossible = tmp == "0" ? false : true;
                else if (countY == 3)
                    board.blackLeftCastlingStillPossible = tmp == "0" ? false : true;
                else if (countY == 4)
                    board.blackRightCastlingStillPossible = tmp == "0" ? false : true;
                else if (countY == 5)
                    board.pawnMovedTwice.x = std::stoi(tmp);
                else if (countY == 6)
                    board.pawnMovedTwice.y = std::stoi(tmp);
                break;
            }
            else if (countX > 8)
            {
                LOG_WARN("Columns exceed 9 " + std::to_string(countY));
                break;
            }

            int piece = std::stoi(tmp);
            if (piece == 0)
                board.board[{countX, countY}] = {PieceColor::NoColor, PieceType::None};
            else
            {
                PieceType type;
                PieceColor color;
                if (piece > 0)
                    color = PieceColor::White;
                else
                {
                    color = PieceColor::Black;
                    piece *= -1;
                }

                switch (piece)
                {
                case 1:
                    type = PieceType::Pawn;
                    break;
                case 2:
                    type = PieceType::Rook;
                    break;
                case 3:
                    type = PieceType::Knight;
                    break;
                case 4:
                    type = PieceType::Bishop;
                    break;
                case 5:
                    type = PieceType::Queen;
                    break;
                case 6:
                    type = PieceType::King;
                    break;

                default:
                    LOG_WARN("Piece " + tmp + " is invalid");
                    break;
                }
                board.board[{countX, countY}] = {color, type};
            }
            countX++;
        }
        countX = 0;
        countY++;
    }
}

void GameScreen::SaveBoard()
{
    std::stringstream ss;
    for (int y = 0; y < 8; y++)
    {
        if (y != 0)
            ss << "\n";

        for (int x = 0; x < 9; x++)
        {
            if (x != 0)
                ss << ",";

            if (x == 8)
            {
                if (y == 0)
                    ss << (board.isWhiteTurn ? 1 : 0);
                else if (y == 1)
                    ss << (board.whiteLeftCastlingStillPossible ? 1 : 0);
                else if (y == 2)
                    ss << (board.whiteRightCastlingStillPossible ? 1 : 0);
                else if (y == 3)
                    ss << (board.blackLeftCastlingStillPossible ? 1 : 0);
                else if (y == 4)
                    ss << (board.blackRightCastlingStillPossible ? 1 : 0);
                else if (y == 5)
                    ss << board.pawnMovedTwice.x;
                else if (y == 6)
                    ss << board.pawnMovedTwice.y;
                else
                 ss << 0;
            }
            else 
            {
                int mult = board.board[{x, y}].first == PieceColor::Black ? -1 : 1;
                int piece = 0;
    
                switch (board.board[{x, y}].second)
                {
                case PieceType::None:
                    ss << "0";
                    break;
                case PieceType::Pawn:
                    piece = 1 * mult;
                    ss << piece;
                    break;
                case PieceType::Rook:
                    piece = 2 * mult;
                    ss << piece;
                    break;
                case PieceType::Knight:
                    piece = 3 * mult;
                    ss << piece;
                    break;
                case PieceType::Bishop:
                    piece = 4 * mult;
                    ss << piece;
                    break;
                case PieceType::Queen:
                    piece = 5 * mult;
                    ss << piece;
                    break;
                case PieceType::King:
                    piece = 6 * mult;
                    ss << piece;
                    break;

                default:
                    ss << "0";
               }
            }
        }
    }

    auto time = std::time(nullptr);
    auto localtime = *std::localtime(&time);

    std::ostringstream oss;
    oss << "Assets/Saves/" << std::put_time(&localtime, "%d-%m-%Y_%H-%M-%S") << ".csv";

    std::ofstream outfile;
    outfile.open(oss.str());
    outfile << ss.str();
    outfile.close();
}

void GameScreen::DrawBoard()
{
    DrawRectangle(boardX - 1, boardY - 1, boardSquareSize * 8 + 2, boardSquareSize * 8 + 2, {54, 93, 201, 255});

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            Color squareColor;
            if (x == selectedPiece.x && y == selectedPiece.y)
                squareColor = {188, 188, 75, 255};
            else if (selectedPiece.x != -1 && selectedPiece.y != -1 && IsLegalMove(board.isWhiteTurn ? board.whiteLegalMoves : board.blackLegalMoves, selectedPiece, {x, y}))
                squareColor = {83, 175, 29, 255};
            else if ((y * 9 + x) % 2 == 0)
                squareColor = {232, 232, 232, 255};
            else
                squareColor = {13, 13, 13, 255};
            DrawRectangle(boardX + x * boardSquareSize, boardY + y * boardSquareSize, boardSquareSize, boardSquareSize, squareColor);
        }
    }
}

void GameScreen::DrawPiece(int x, int y, PieceType type, PieceColor color)
{
    Rectangle sourceRect = {(int)type * 16.0f, 0.0f, 16.0f, 16.0f};
    Rectangle destRect = {(float)(boardX + x * boardSquareSize) + 2.0f, (float)(boardY + y * boardSquareSize), boardSquareSize - 4.0f, boardSquareSize - 4.0f};
    if (type != PieceType::None)
    {
        if (color == PieceColor::White)
        {
            DrawTexturePro(whitePiecesTexture, sourceRect, destRect, {0.0f, 0.0f}, 0.0f, WHITE);
        }
        else if (color == PieceColor::Black)
        {
            DrawTexturePro(blackPiecesTexture, sourceRect, destRect, {0.0f, 0.0f}, 0.0f, WHITE);
        }
    }
}

bool GameScreen::MovePiece(Vector2i from, Vector2i to, Board& _board)
{
    PieceColor color = _board.board[from].first;
    PieceType type = _board.board[from].second;
    if (IsLegalMove(color == PieceColor::White ? _board.whiteLegalMoves : _board.blackLegalMoves, from, to))
    {
        _board.board[from] = {PieceColor::NoColor, PieceType::None};
        _board.board[to] = {color, type};

        if (type != PieceType::Pawn)
            _board.pawnMovedTwice = {-1, -1};
        if (type == PieceType::King)
        {
            if (color == PieceColor::White)
            {
                _board.whiteRightCastlingStillPossible = false;
                _board.whiteLeftCastlingStillPossible = false;
            }
            else if (color == PieceColor::Black)
            {
                _board.blackRightCastlingStillPossible = false;
                _board.blackLeftCastlingStillPossible = false;
            }

            if (from.x - to.x > 1)
            {
                _board.board[{0, to.y}] = {PieceColor::NoColor, PieceType::None};
                _board.board[{3, to.y}] = {color, PieceType::Rook};
            }
            else if (to.x - from.x > 1)
            {
                _board.board[{7, to.y}] = {PieceColor::NoColor, PieceType::None};
                _board.board[{5, to.y}] = {color, PieceType::Rook};
            }
        }
        if (type == PieceType::Rook)
        {
            if (color == PieceColor::White)
            {
                if (from.x == 0)
                    _board.whiteLeftCastlingStillPossible = false;
                else if (from.x == 7)
                    _board.whiteRightCastlingStillPossible = false;
            }
            else if (color == PieceColor::Black)
            {
                if (from.x == 0)
                    _board.blackLeftCastlingStillPossible = false;
                else if (from.x == 7)
                    _board.blackRightCastlingStillPossible = false;
            }
        }

        else if (type == PieceType::Pawn)
        {
            if (_board.pawnMovedTwice.x != -1 && _board.pawnMovedTwice.y != -1)
            {
                if (from.y == _board.pawnMovedTwice.y && to.x == _board.pawnMovedTwice.x && _board.board[_board.pawnMovedTwice].first != color)
                    _board.board[_board.pawnMovedTwice] = {PieceColor::NoColor, PieceType::None};
                _board.pawnMovedTwice = {-1, -1};
            }
            if (from.y - to.y > 1 || to.y - from.y > 1)
                _board.pawnMovedTwice = to;
            else
                _board.pawnMovedTwice = {-1, -1};
            if (to.y == 0 || to.y == 7)
                _board.board[to].second = PieceType::Queen;
        }

        return true;
    }
    return false;
}

std::vector<LegalMove> GameScreen::GetLegalMoves(PieceColor color, Board _board)
{
    std::vector<LegalMove> legalMoves;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            std::pair<PieceColor, PieceType> piece = _board.board[{x, y}];
            if (piece.first != color)
                continue;

            for (int toY = 0; toY < 8; toY++)
            {
                for (int toX = 0; toX < 8; toX++)
                {
                    if (IsPossibleLegalMove(piece.second, color, {x, y}, {toX, toY}, _board))
                        legalMoves.push_back({{x, y}, {toX, toY}});
                }
            }
        }
    }
    return legalMoves;
}

bool GameScreen::IsLegalMove(std::vector<LegalMove> legalMoves, Vector2i from, Vector2i to)
{
    for (int i = 0; i < legalMoves.size(); i++)
    {
        if (legalMoves[i].from.x == from.x && legalMoves[i].from.y == from.y && legalMoves[i].to.x == to.x && legalMoves[i].to.y == to.y)
            return true;
    }

    return false;
}

bool GameScreen::IsPossibleLegalMove(PieceType type, PieceColor color, Vector2i from, Vector2i to, Board _board)
{
    if (_board.board[to].first == color)
        return false;

    if (type == PieceType::Rook)
    {
        if (from.x == to.x && from.y != to.y) // || from.x != to.x && from.y == to.y)
        {
            if (to.y - from.y > 0)
            {
                for (int y = from.y + 1; y < to.y; y++)
                    if (_board.board[{from.x, y}].second != PieceType::None)
                        return false;
            }
            else
            {
                for (int y = from.y - 1; y > to.y; y--)
                    if (_board.board[{from.x, y}].second != PieceType::None)
                        return false;
            }
            return true;
        }
        else if (from.x != to.x && from.y == to.y)
        {
            if (to.x - from.x > 0)
            {
                for (int x = from.x + 1; x < to.x; x++)
                    if (_board.board[{x, from.y}].second != PieceType::None)
                        return false;
            }
            else
            {
                for (int x = from.x - 1; x > to.x; x--)
                {
                    if (_board.board[{x, from.y}].second != PieceType::None)
                        return false;
                }
            }
            return true;
        }
    }
    else if (type == PieceType::Bishop)
    {
        Vector2i distance = {to.x - from.x, to.y - from.y};
        if (distance.x == distance.y || distance.x == -distance.y)
        {
            if (distance.x > 0 && distance.y > 0)
            {
                for (int y = from.y + 1, x = from.x + 1; y < to.y && x < to.x; y++, x++)
                    if (_board.board[{x, y}].second != PieceType::None)
                        return false;
            }
            else if (distance.x < 0 && distance.y > 0)
            {

                for (int y = from.y + 1, x = from.x - 1; y < to.y && x > to.x; y++, x--)
                    if (_board.board[{x, y}].second != PieceType::None)
                        return false;
            }
            else if (distance.x < 0 && distance.y < 0)
            {

                for (int y = from.y - 1, x = from.x - 1; y > to.y && x > to.x; y--, x--)
                    if (_board.board[{x, y}].second != PieceType::None)
                        return false;
            }
            else if (distance.x > 0 && distance.y < 0)
            {
                for (int y = from.y - 1, x = from.x + 1; y > to.y && x < to.x; y--, x++)
                    if (_board.board[{x, y}].second != PieceType::None)
                        return false;
            }
            return true;
        }
    }
    else if (type == PieceType::Queen)
    {
        if (IsPossibleLegalMove(PieceType::Rook, color, from, to, _board) || IsPossibleLegalMove(PieceType::Bishop, color, from, to, _board))
            return true;
    }
    else if (type == PieceType::King)
    {
        Vector2i distance = {to.x - from.x, to.y - from.y};
        if (distance.x < 0)
            distance.x *= -1;
        if (distance.y < 0)
            distance.y *= -1;
        if (distance.x <= 1 && distance.y <= 1)
            return true;

        if (color == PieceColor::White && !_board.whiteRightCastlingStillPossible && !_board.whiteLeftCastlingStillPossible)
            return false;
        if (color == PieceColor::Black && !_board.blackRightCastlingStillPossible && !_board.blackLeftCastlingStillPossible)
            return false;

        if (from.y != to.y)
            return false;

        if (to.x == 6 && _board.board[{5, from.y}].second == PieceType::None)
        {
            if (color == PieceColor::White && _board.whiteRightCastlingStillPossible)
                return true;
            if (color == PieceColor::Black && _board.blackRightCastlingStillPossible)
                return true;
        }

        if (to.x == 2 && _board.board[{1, from.y}].second == PieceType::None && _board.board[{3, from.y}].second == PieceType::None)
        {
            if (color == PieceColor::White && _board.whiteLeftCastlingStillPossible)
                return true;
            if (color == PieceColor::Black && _board.blackLeftCastlingStillPossible)
                return true;
        }
    }
    else if (type == PieceType::Knight)
    {
        Vector2i distance = {to.x - from.x, to.y - from.y};
        if (distance.x < 0)
            distance.x *= -1;
        if (distance.y < 0)
            distance.y *= -1;

        if (distance.x == 2 && distance.y == 1 || distance.x == 1 && distance.y == 2)
            return true;
    }
    else if (type == PieceType::Pawn)
    {
        if (from.x == to.x)
        {
            if (color == PieceColor::White)
            {
                if (_board.board[to].first == PieceColor::Black)
                    return false;

                if (to.y + 1 == from.y || from.y == 6 && to.y == 4)
                    return true;
            }
            else if (color == PieceColor::Black)
            {
                if (_board.board[to].first == PieceColor::White)
                    return false;

                if (to.y - 1 == from.y || from.y == 1 && to.y == 3)
                    return true;
            }
        }
        else if (to.x == from.x - 1 || to.x == from.x + 1)
        {
            if (color == PieceColor::White)
            {
                if (to.y + 1 == from.y)
                {
                    if (_board.board[to].first == PieceColor::Black)
                        return true;

                    // En-Passant
                    if (from.y == _board.pawnMovedTwice.y && to.x == _board.pawnMovedTwice.x && _board.board[_board.pawnMovedTwice].first == PieceColor::Black)
                        return true;
                }
            }
            else if (color == PieceColor::Black)
            {
                if (to.y - 1 == from.y)
                {
                    if (_board.board[to].first == PieceColor::White)
                        return true;

                    // En-Passant
                    if (from.y == _board.pawnMovedTwice.y && to.x == _board.pawnMovedTwice.x && _board.board[_board.pawnMovedTwice].first == PieceColor::White)
                        return true;
                }
            }
        }
    }

    return false;
}

bool GameScreen::IsInCheck(PieceColor color, Board _board)
{
    if (color == PieceColor::NoColor)
        return false;

    Vector2i kingPos = {-1, -1};
    bool kingFound = false;
    for (int y = 0; y < 8 && !kingFound; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (_board.board[{x, y}].first == color && _board.board[{x, y}].second == PieceType::King)
            {
                kingPos = {x, y};
                kingFound = true;
                break;
            }
        }
    }
    if (kingPos.x == -1 && kingPos.y == -1)
        return false;

    PieceColor checkColor = color == PieceColor::White ? PieceColor::Black : PieceColor::White;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (_board.board[{x, y}].first == checkColor)
            {
                if (IsPossibleLegalMove(_board.board[{x, y}].second, checkColor, {x, y}, kingPos, _board))
                    return true;
            }
        }
    }

    return false;
}

void GameScreen::RemoveCheckMoves()
{
    PieceColor color = board.isWhiteTurn ? PieceColor::White : PieceColor::Black;

    Board tempBoard;
    tempBoard.isWhiteTurn = board.isWhiteTurn;
    tempBoard.whiteRightCastlingStillPossible = board.whiteRightCastlingStillPossible;
    tempBoard.whiteLeftCastlingStillPossible = board.whiteLeftCastlingStillPossible;
    tempBoard.blackRightCastlingStillPossible = board.blackRightCastlingStillPossible;
    tempBoard.blackLeftCastlingStillPossible = board.blackLeftCastlingStillPossible;
    tempBoard.pawnMovedTwice = board.pawnMovedTwice;

    auto legalMoves = color == PieceColor::White ? &board.whiteLegalMoves : &board.blackLegalMoves;
    for (int i = legalMoves->size() - 1; i >= 0; i--)
    {
        tempBoard.whiteLegalMoves = board.whiteLegalMoves;
        tempBoard.blackLegalMoves = board.blackLegalMoves;

        tempBoard.board = board.board;

        MovePiece((*legalMoves)[i].from, (*legalMoves)[i].to, tempBoard);
        if (IsInCheck(color, tempBoard))
            legalMoves->erase(legalMoves->begin() + i);
    }
}
