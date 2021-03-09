#include <cassert>
#include <memory>
#include <vector>

#include "Board.hpp"
#include "Piece.hpp"
#include "Vector2d.hpp"

using std::optional;
using std::nullopt;
using std::vector;

namespace PieceNS {
		optional<Vector2d> Base::destOfDirectionStraight(int direction, Vector2d from, char c) const {
			assert (-1 <= direction && direction <= 1);
			assert('1' <= c && c-'1' < (direction==0 ?Board::N_COL :Board::N_ROW));

			if (direction==0) {
				const int k = c - '1';
				if (k==from.y) return nullopt;
				return Vector2d{from.x, Board::colToTeam(team, k)};
			} else {
				const int k = c - '0';
				Vector2d p{from.x+direction*k, from.y};
				if (Board::inBound(p)) return p;
				return nullopt;
			}
		}

		optional<Vector2d> Base::destOfDirectionsByCandidates(const vector<Vector2d> &dirCandidates, int direction, Vector2d from, char c) const {
			assert (-1 <= direction && direction <= 1);

			if (direction==0) return nullopt;

			int k = Board::colToTeam(team, c-'1');
			for (Vector2d d: dirCandidates) {
				Vector2d p = from + d;
				if (!Board::inBound(p) || p.y != k) continue;
				if ((p.x - from.x) * direction > 0) return p;
			}

			return nullopt;
		}

	bool Ju::isMoveCandidate(const Board &board, const Vector2d from, const Vector2d to) const {
		assert(Board::inBound(from));
		assert(Board::inBound(to));
		assert(from != to);

		return board.countPiecesBetween(from, to).value_or(-1) == 0;
	}

	optional<Vector2d> Ju::destOfDirection(int direction, Vector2d from, char c) const {
		return destOfDirectionStraight(direction, from, c);
	}

	bool Ma::isMoveCandidate(const Board &board, const Vector2d from, const Vector2d to) const {
		assert(Board::inBound(from));
		assert(Board::inBound(to));
		assert(from != to);

		Vector2d d{to - from};
		if (abs(d.x * d.y) != 2) {
			return false;
		}

		d -= d.quandrant();
		if (board.pieceExist(from+d)) {
			return false;
		}

		return true;
	}

	optional<Vector2d> Ma::destOfDirection(int direction, Vector2d from, char c) const {
		static const vector<Vector2d> dirs{{{2,1}, {1,2}, {-1,2}, {-2,1}, {-2,-1}, {-1,-2}, {1,-2}, {2,-1}}};
		return destOfDirectionsByCandidates(dirs, direction, from, c);
	}

	bool Pao::isMoveCandidate(const Board &board, const Vector2d from, const Vector2d to) const {
		assert(Board::inBound(from));
		assert(Board::inBound(to));
		assert(from != to);

		const optional<int> v{board.countPiecesBetween(from, to)};
		if (!v.has_value()) return false;
		if (*v == 0) return !board.pieceExist(to);
		return *v==1 && board.pieceExist(to);
	}

	optional<Vector2d> Pao::destOfDirection(int direction, Vector2d from, char c) const {
		return destOfDirectionStraight(direction, from, c);
	}

	bool Shi::isShiPosition(const Vector2d p) const {
		if (!Board::inBase(team, p)) return false;
		const Vector2d pt = Board::toTeam(team, p);
		return (pt.x + pt.y)%2 == 1;
	}

	bool Shi::isMoveCandidate(const Board &, const Vector2d from, const Vector2d to) const {
		assert(Board::inBound(from));
		assert(Board::inBound(to));
		assert(from != to);
		assert(isShiPosition(from));

		if (!Board::inBase(team, to)) return false;

		const Vector2d d{to - from};
		return abs(d.x)==1 && abs(d.y)==1;
	}

	optional<Vector2d> Shi::destOfDirection(int direction, Vector2d from, char c) const {
		static const vector<Vector2d> dirs{{{1,1}, {1,-1}, {-1,-1}, {-1,1}}};
		return destOfDirectionsByCandidates(dirs, direction, from, c);
	}

	bool Xiang::isXiangPosition(const Vector2d p) const {
		if (!Board::inTeam(team, p)) return false;
		const Vector2d pt = Board::toTeam(team, p);
		return pt.x%2==0 && pt.y%2==0 && (pt.x/2 + pt.y/2)%2 == 1;
	}

	bool Xiang::isMoveCandidate(const Board &board, const Vector2d from, const Vector2d to) const {
		assert(Board::inBound(from));
		assert(Board::inBound(to));
		assert(from != to);
		assert(isXiangPosition(from));

		if (!Board::inTeam(team, to)) return false;

		const Vector2d d{to - from};
		return abs(d.x)==2 && abs(d.y)==2 && !board.pieceExist(from + d.quandrant());
	}

	optional<Vector2d> Xiang::destOfDirection(int direction, Vector2d from, char c) const {
		static const vector<Vector2d> dirs{{{2,2}, {2,-2}, {-2,-2}, {-2,2}}};
		return destOfDirectionsByCandidates(dirs, direction, from, c);
	}

	bool Jiang::isMoveCandidate(const Board &, const Vector2d from, const Vector2d to) const {
		assert(Board::inBound(from));
		assert(Board::inBound(to));
		assert(from != to);
		assert(Board::inBase(team, from));

		if (!Board::inBase(team, to)) return false;

		const Vector2d d{to - from};
		return abs(d.x) + abs(d.y) == 1;
	}

	optional<Vector2d> Jiang::destOfDirection(int direction, Vector2d from, char c) const {
		return destOfDirectionStraight(direction, from, c);
	}

	bool Zu::isZuPosition(const Vector2d p) const {
		if (!Board::inTeam(team, p)) return true;
		const Vector2d pt = Board::toTeam(team, p);
		return pt.x>=3 && pt.y%2==0;
	}

	bool Zu::isMoveCandidate(const Board &, const Vector2d from, const Vector2d to) const {
		assert(Board::inBound(from));
		assert(Board::inBound(to));
		assert(from != to);
		assert(isZuPosition(from));

		const Vector2d d{Board::toTeam(team, to) - Board::toTeam(team, from)};
		return Board::inTeam(team, from) ?(d.x==1 && d.y==0) :(d.x>=0 && d.x+abs(d.y)==1);
	}

	optional<Vector2d> Zu::destOfDirection(int direction, Vector2d from, char c) const {
		return destOfDirectionStraight(direction, from, c);
	}

}
