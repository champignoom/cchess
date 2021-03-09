#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>
#include <optional>
#include <memory>
#include <vector>

#include "Piece.hpp"

class Board {
	public:
		static constexpr int N_COL = 9;
		static constexpr int N_ROW = 10;
	private:
		using BoardArray = std::array<std::array<std::unique_ptr<Piece>, N_COL>, N_ROW>;
		BoardArray board;
		Board(BoardArray &&board_): board(std::move(board_)) { }

		std::vector<Vector2d> getPiecesOfCol(Team team, char enPieceName, char col) const;
		std::optional<Vector2d> parseKthPieceAtCol(Team team, char enPieceName, char kthInCol, char col) const;

	public:
		static Board makeStandardBoard();

		static bool inBound(Vector2d);
		static bool inTeam(Team, Vector2d);
		static bool inBase(Team, Vector2d);
		static Vector2d toTeam(Team, Vector2d);
		static int colToTeam(Team, int);

		std::unique_ptr<Piece> &pieceAt(Vector2d);
		const std::unique_ptr<Piece> &pieceAt(Vector2d) const;
		bool pieceExist(Vector2d) const;
		std::optional<int> countPiecesBetween(Vector2d from, Vector2d to) const;
		bool isMoveable(Vector2d from, Vector2d to) const;
		void makeMove(Vector2d from, Vector2d to);
		void print() const;

		std::optional<Vector2d> parseSinglePiece(Team team, char enPieceName, char col) const;
		std::optional<Vector2d> parseZuAtCol(Team team, char kthInCol, char col) const;
		std::optional<Vector2d> parseMultiple(Team team, char kthInCol, char enPieceName) const;
		std::optional<Vector2d> parseDestByDirection(Vector2d from, char direction, char destCol) const;
};

#endif
