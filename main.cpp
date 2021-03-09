#include "Board.hpp"

#include <iostream>
#include <optional>
#include <regex>
#include <cctype>
#include <cstdlib>

using std::cin;
using std::cout;
using std::endl;
using std::optional;
using std::nullopt;
using std::regex;
using std::smatch;
using std::string;

// enum PieceType                                 { JU,       MA,       XIANG,    SHI,      JIANG,    PAO,      ZU,       NUM_PIECE };
// array<string_view, NUM_PIECE> red_piece_name   { u8"俥"sv, u8"傌"sv, u8"相"sv, u8"士"sv, u8"帥"sv, u8"炮"sv, u8"兵"sv  };
// array<string_view, NUM_PIECE> black_piece_name { u8"車"sv, u8"馬"sv, u8"象"sv, u8"士"sv, u8"將"sv, u8"砲"sv, u8"卒"sv  };

struct Command {
	Vector2d from, to;

	Command(Vector2d from_, Vector2d to_): from(from_), to(to_) { }

	static optional<Command> readFromStdin() {
		int x0, y0, x1, y1;
		cin >>x0 >>y0 >>x1 >>y1;
		Command c{ {x0, y0}, {x1, y1} };

		if (!Board::inBound(c.from) || !Board::inBound(c.to)) return nullopt;
		if (c.from == c.to) return nullopt;
		return c;
	}

	static optional<Command> quadFromStdin(const Board &board, const Team currentPlayer) {
		string s;
		if (!(cin >>s)) {
			cout <<endl;
			exit(0);
		}

		static const regex e{R"((?:([jmpxswbz])[1-9]|[qh2-5][jmpxswbz1-9])[jtp][1-9])"};
		smatch m;
		if (!regex_match(s, m, e)) return nullopt;

		optional<Vector2d> from{[&]{
			if (m[1].matched) {
				return board.parseSinglePiece(currentPlayer, s[0], s[1]);
			} else {
				return isdigit(s[1]) ?board.parseZuAtCol(currentPlayer, s[0], s[1]) :board.parseMultiple(currentPlayer, s[0], s[1]);
			}
		}()};

		if (!from.has_value()) return nullopt;

		optional<Vector2d> to = board.parseDestByDirection(*from, s[2], s[3]);
		if (!to.has_value()) return nullopt;

		return Command{*from, *to};
	}
};

int main() {
	Board board = Board::makeStandardBoard();
	Team currentPlayer = Team::red;

	board.print();

	while (true) {
		cout <<cnName(currentPlayer) <<"> ";
		optional<Command> c = Command::quadFromStdin(board, currentPlayer);
		if (!c.has_value()) {
			cout <<"invalid command" <<endl;
			continue;
		}

		if (!Board::inBound(c->from)) {
			cout <<"Out of bound: " <<c->from <<endl;
			continue;
		}

		if (!Board::inBound(c->to)) {
			cout <<"Out of bound: " <<c->to <<endl;
			continue;
		}

		if (c->from == c->to) {
			cout <<"Not moving" <<endl;
			continue;
		}

		if (!board.pieceExist(c->from)) {
			cout <<"No piece at " <<c->from <<endl;
			continue;
		}

		if (board.pieceAt(c->from)->team != currentPlayer) {
			cout <<"Not your piece" <<endl;
			continue;
		}

		if (!board.isMoveable(c->from, c->to)) {
			cout <<"Cannot move the piece at " <<c->from <<" to " <<c->to <<endl;
			continue;
		}

		board.makeMove(c->from, c->to);
		board.print();
		currentPlayer = otherTeam(currentPlayer);
	}
}
