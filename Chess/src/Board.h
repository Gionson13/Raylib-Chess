#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
#include <string>

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
    Bishop,
	Rook,
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
	Board();

    std::unordered_map<Vector2i, std::pair<PieceColor, PieceType>, Vector2i_hash, Vector2i_equal_to> cells;
    bool isWhiteTurn;

    bool whiteRightCastlingStillPossible;
    bool whiteLeftCastlingStillPossible;
    bool blackRightCastlingStillPossible;
    bool blackLeftCastlingStillPossible;

    Vector2i pawnMovedTwice;

    std::vector<LegalMove> whiteLegalMoves;
    std::vector<LegalMove> blackLegalMoves;
};

bool MovePiece(Board& board, Vector2i from, Vector2i to);
bool IsLegalMove(std::vector<LegalMove> legalMoves, Vector2i from, Vector2i to);
bool IsPossibleLegalMove(Board& board, PieceType type, PieceColor color, Vector2i from, Vector2i to);
bool IsInCheck(Board& board, PieceColor color);
bool IsEating(Board& board, Vector2i from, Vector2i to);
bool IsQueening(Board& board, Vector2i from, Vector2i to);

// Remove all moves that would lead to the king being eaten in the next move
void RemoveCheckMoves(Board& board);

std::vector<LegalMove> GetLegalMoves(Board& board, PieceColor color);

// Load a board from a .csv file
bool LoadBoard(Board& board, std::string filename);
void SaveBoard(Board& board);
