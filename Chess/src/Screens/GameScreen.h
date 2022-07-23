#pragma once

#include "../ScreenManagement/Screen.h"
#include "../Utils/BannerAnimation.h"
#include "../Particles/ParticleEmitter.h"
#include <iostream>
#include <raylib.h>
#include <unordered_map>
#include <vector>

// TODO: Polish 


struct Vector2i
{
    int x, y;
};

struct Vector2i_equal_to
{
    bool operator()(const Vector2i& left, const Vector2i& right) const
    {
        return left.x == right.x && left.y == right.y;
    }
};

struct Vector2i_hash
{
    std::size_t operator()(const Vector2i& vec) const
    {
        return std::hash<int>()(vec.x) ^ std::hash<int>()(vec.y);
    }
};

struct LegalMove
{
    Vector2i from;
    Vector2i to;
};

enum PieceType
{
    Pawn,
    Knight,
    Rook,
    Bishop,
    Queen,
    King,
    None
};

enum PieceColor
{
    White,
    Black,
    NoColor
};

struct Board
{
    std::unordered_map<Vector2i, std::pair<PieceColor, PieceType>, Vector2i_hash, Vector2i_equal_to> board;
    bool isWhiteTurn;

    bool whiteRightCastlingStillPossible;
    bool whiteLeftCastlingStillPossible;
    bool blackRightCastlingStillPossible;
    bool blackLeftCastlingStillPossible;

    Vector2i pawnMovedTwice;

    std::vector<LegalMove> whiteLegalMoves;
    std::vector<LegalMove> blackLegalMoves;

    Board();
};

namespace GameScreen
{

    Screen GetScreen();


    void Load();
    void Unload();

    void Update(float dt);
    void Render();

    void RenderStartTransition(float time);
    bool IsStartTransitionDone(float time);

    void RenderEndTransition(float time);
    bool IsEndTransitionDone(float time);

    void OnResize();

    // Load a board from a .csv file
    void LoadBoard(std::string filename);
    void SaveBoard();

    void DrawBoard();
    void DrawPiece(int x, int y, PieceType type, PieceColor color);

    bool MovePiece(Vector2i from, Vector2i to, Board& _board);
    bool IsLegalMove(std::vector<LegalMove> legalMoves, Vector2i from, Vector2i to);
    bool IsPossibleLegalMove(PieceType type, PieceColor color, Vector2i from, Vector2i to, Board _board);
    bool IsInCheck(PieceColor color, Board _board);

    // Remove all moves that would lead to the king being eaten in the next move
    void RemoveCheckMoves();

    std::vector<LegalMove> GetLegalMoves(PieceColor color, Board _board);


} // namespace GameScreen
