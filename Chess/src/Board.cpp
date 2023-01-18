#include "Board.h"
#include "Utils/Logger.h"

#include <cstring>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cerrno>

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
			cells[{x, y}] = {PieceColor::NoColor, PieceType::None};
}


bool MovePiece(Board& board, Vector2i from, Vector2i to)
{
	PieceColor color = board.cells[from].first;
	PieceType type = board.cells[from].second;
	if (IsLegalMove(color == PieceColor::White ? board.whiteLegalMoves : board.blackLegalMoves, from, to))
	{
		board.cells[from] = {PieceColor::NoColor, PieceType::None};
		board.cells[to] = {color, type};

		if (type != PieceType::Pawn)
			board.pawnMovedTwice = {-1, -1};
		if (type == PieceType::King)
		{
			if (color == PieceColor::White)
			{
				board.whiteRightCastlingStillPossible = false;
				board.whiteLeftCastlingStillPossible = false;
			}
			else if (color == PieceColor::Black)
			{
				board.blackRightCastlingStillPossible = false;
				board.blackLeftCastlingStillPossible = false;
			}

			if (from.x - to.x > 1)
			{
				board.cells[{0, to.y}] = {PieceColor::NoColor, PieceType::None};
				board.cells[{3, to.y}] = {color, PieceType::Rook};
			}
			else if (to.x - from.x > 1)
			{
				board.cells[{7, to.y}] = {PieceColor::NoColor, PieceType::None};
				board.cells[{5, to.y}] = {color, PieceType::Rook};
			}
		}
		if (type == PieceType::Rook)
		{
			if (color == PieceColor::White)
			{
				if (from.x == 0)
					board.whiteLeftCastlingStillPossible = false;
				else if (from.x == 7)
					board.whiteRightCastlingStillPossible = false;
			}
			else if (color == PieceColor::Black)
			{
				if (from.x == 0)
					board.blackLeftCastlingStillPossible = false;
				else if (from.x == 7)
					board.blackRightCastlingStillPossible = false;
			}
		}

		else if (type == PieceType::Pawn)
		{
			if (board.pawnMovedTwice.x != -1 && board.pawnMovedTwice.y != -1)
			{
				if (from.y == board.pawnMovedTwice.y && to.x == board.pawnMovedTwice.x && board.cells[board.pawnMovedTwice].first != color)
					board.cells[board.pawnMovedTwice] = {PieceColor::NoColor, PieceType::None};
				board.pawnMovedTwice = {-1, -1};
			}
			if (from.y - to.y > 1 || to.y - from.y > 1)
				board.pawnMovedTwice = to;
			else
				board.pawnMovedTwice = {-1, -1};
			if (to.y == 0 || to.y == 7)
				board.cells[to].second = PieceType::Queen;
		}

		return true;
	}
	return false;
}


std::vector<LegalMove> GetLegalMoves(Board& board, PieceColor color)
{
    std::vector<LegalMove> legalMoves;
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            std::pair<PieceColor, PieceType> piece = board.cells[{x, y}];
            if (piece.first != color)
                continue;

            for (int toY = 0; toY < 8; toY++)
            {
                for (int toX = 0; toX < 8; toX++)
                {
                    if (IsPossibleLegalMove(board, piece.second, color, {x, y}, {toX, toY}))
                        legalMoves.push_back({{x, y}, {toX, toY}});
                }
            }
        }
    }
    return legalMoves;
}


bool IsLegalMove(std::vector<LegalMove> legalMoves, Vector2i from, Vector2i to)
{
    for (size_t i = 0; i < legalMoves.size(); i++)
    {
        if (legalMoves[i].from.x == from.x && legalMoves[i].from.y == from.y && legalMoves[i].to.x == to.x && legalMoves[i].to.y == to.y)
            return true;
    }

    return false;
}


bool IsPossibleLegalMove(Board& board, PieceType type, PieceColor color, Vector2i from, Vector2i to)
{
    if (board.cells[to].first == color)
        return false;

    if (type == PieceType::Rook)
    {
        if (from.x == to.x && from.y != to.y) // || from.x != to.x && from.y == to.y)
        {
            if (to.y - from.y > 0)
            {
                for (int y = from.y + 1; y < to.y; y++)
                    if (board.cells[{from.x, y}].second != PieceType::None)
                        return false;
            }
            else
            {
                for (int y = from.y - 1; y > to.y; y--)
                    if (board.cells[{from.x, y}].second != PieceType::None)
                        return false;
            }
            return true;
        }
        else if (from.x != to.x && from.y == to.y)
        {
            if (to.x - from.x > 0)
            {
                for (int x = from.x + 1; x < to.x; x++)
                    if (board.cells[{x, from.y}].second != PieceType::None)
                        return false;
            }
            else
            {
                for (int x = from.x - 1; x > to.x; x--)
                {
                    if (board.cells[{x, from.y}].second != PieceType::None)
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
                    if (board.cells[{x, y}].second != PieceType::None)
                        return false;
            }
            else if (distance.x < 0 && distance.y > 0)
            {

                for (int y = from.y + 1, x = from.x - 1; y < to.y && x > to.x; y++, x--)
                    if (board.cells[{x, y}].second != PieceType::None)
                        return false;
            }
            else if (distance.x < 0 && distance.y < 0)
            {

                for (int y = from.y - 1, x = from.x - 1; y > to.y && x > to.x; y--, x--)
                    if (board.cells[{x, y}].second != PieceType::None)
                        return false;
            }
            else if (distance.x > 0 && distance.y < 0)
            {
                for (int y = from.y - 1, x = from.x + 1; y > to.y && x < to.x; y--, x++)
                    if (board.cells[{x, y}].second != PieceType::None)
                        return false;
            }
            return true;
        }
    }
    else if (type == PieceType::Queen)
    {
        if (IsPossibleLegalMove(board, PieceType::Rook, color, from, to) || IsPossibleLegalMove(board, PieceType::Bishop, color, from, to))
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

        if (color == PieceColor::White && !board.whiteRightCastlingStillPossible && !board.whiteLeftCastlingStillPossible)
            return false;
        if (color == PieceColor::Black && !board.blackRightCastlingStillPossible && !board.blackLeftCastlingStillPossible)
            return false;

        if (from.y != to.y)
            return false;

        if (to.x == 6 && board.cells[{5, from.y}].second == PieceType::None)
        {
            if (color == PieceColor::White && board.whiteRightCastlingStillPossible)
                return true;
            if (color == PieceColor::Black && board.blackRightCastlingStillPossible)
                return true;
        }

        if (to.x == 2 && board.cells[{1, from.y}].second == PieceType::None && board.cells[{3, from.y}].second == PieceType::None)
        {
            if (color == PieceColor::White && board.whiteLeftCastlingStillPossible)
                return true;
            if (color == PieceColor::Black && board.blackLeftCastlingStillPossible)
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

        if ((distance.x == 2 && distance.y == 1) || (distance.x == 1 && distance.y == 2))
            return true;
    }
    else if (type == PieceType::Pawn)
    {
        if (from.x == to.x)
        {
            if (color == PieceColor::White)
            {
                if (board.cells[to].first == PieceColor::Black)
                    return false;

                if (to.y + 1 == from.y)
                    return true;

                if ((from.y == 6 && to.y == 4) && board.cells[{from.x, 5}].first == PieceColor::NoColor)
                    return true;
            }
            else if (color == PieceColor::Black)
            {
                if (board.cells[to].first == PieceColor::White)
                    return false;

                if (to.y - 1 == from.y)
                    return true;

                if ((from.y == 1 && to.y == 3) && board.cells[{from.x, 2}].first == PieceColor::NoColor)
                    return true;
            }
        }
        else if (to.x == from.x - 1 || to.x == from.x + 1)
        {
            if (color == PieceColor::White)
            {
                if (to.y + 1 == from.y)
                {
                    if (board.cells[to].first == PieceColor::Black)
                        return true;

                    // En-Passant
                    if (from.y == board.pawnMovedTwice.y && to.x == board.pawnMovedTwice.x && board.cells[board.pawnMovedTwice].first == PieceColor::Black)
                        return true;
                }
            }
            else if (color == PieceColor::Black)
            {
                if (to.y - 1 == from.y)
                {
                    if (board.cells[to].first == PieceColor::White)
                        return true;

                    // En-Passant
                    if (from.y == board.pawnMovedTwice.y && to.x == board.pawnMovedTwice.x && board.cells[board.pawnMovedTwice].first == PieceColor::White)
                        return true;
                }
            }
        }
    }

    return false;
}


bool IsInCheck(Board& board, PieceColor color)
{
    if (color == PieceColor::NoColor)
        return false;

    Vector2i kingPos = {-1, -1};
    bool kingFound = false;
    for (int y = 0; y < 8 && !kingFound; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (board.cells[{x, y}].first == color && board.cells[{x, y}].second == PieceType::King)
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
            if (board.cells[{x, y}].first == checkColor)
            {
                if (IsPossibleLegalMove(board, board.cells[{x, y}].second, checkColor, {x, y}, kingPos))
                    return true;
            }
        }
    }

    return false;
}

// Doesn't check for colors
bool IsEating(Board& board, Vector2i from, Vector2i to)
{
    if (board.cells[to].first != PieceColor::NoColor)
        return true;

    if (board.cells[from].second != PieceType::Pawn)
        return false;

    if (to.y + 1 == from.y)
    {
        // En-Passant
        if (from.y == board.pawnMovedTwice.y && to.x == board.pawnMovedTwice.x)
            return true;
    }
    
    if (to.y - 1 == from.y)
    {
        // En-Passant
        if (from.y == board.pawnMovedTwice.y && to.x == board.pawnMovedTwice.x)
            return true;
    }

    return false;

}


void RemoveCheckMoves(Board& board)
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

        tempBoard.cells = board.cells;

        MovePiece(tempBoard, (*legalMoves)[i].from, (*legalMoves)[i].to);
        if (IsInCheck(tempBoard, color))
            legalMoves->erase(legalMoves->begin() + i);
    }
}



void SaveBoard(Board& board)
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
                int mult = board.cells[{x, y}].first == PieceColor::Black ? -1 : 1;
                int piece = 0;
    
                switch (board.cells[{x, y}].second)
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


bool LoadBoard(Board& board, std::string filename)
{
    std::ifstream fileStream(filename);
    if (!fileStream)
    {
        LOG_WARN("Failed to open {}: {}", filename, std::strerror(errno));
        return false;
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
                board.cells[{countX, countY}] = {PieceColor::NoColor, PieceType::None};
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
                board.cells[{countX, countY}] = {color, type};
            }
            countX++;
        }
        countX = 0;
        countY++;
    }

	LOG_INFO("Board loaded successfully");

	return true;
}
