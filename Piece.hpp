#ifndef PIECE_HPP
#define PIECE_HPP

#include <cassert>
#include <string>
#include <vector>

#include "Team.hpp"
#include "Vector2d.hpp"

class Board;

namespace PieceNS {
	using std::u32string_view;
	using std::string_view;

	class Base {
		protected:
			std::optional<Vector2d> destOfDirectionStraight(int direction, Vector2d from, char c) const;
			std::optional<Vector2d> destOfDirectionsByCandidates(const std::vector<Vector2d> &dirCandidates, int direction, Vector2d from, char c) const;
		public:
			const Team team;

			Base(Team team_): team(team_) { }

			virtual u32string_view getNameRed() const = 0;
			virtual u32string_view getNameBlack() const = 0;
			virtual string_view getNameEn() const = 0;

			virtual u32string_view getName() const {
				switch (team) {
					case Team::red: return getNameRed();
					case Team::black: return getNameBlack();
					default: assert(false); // impossible
				}
			}

			virtual bool isMoveCandidate(const Board &board, const Vector2d from, const Vector2d to) const = 0;
			virtual std::optional<Vector2d> destOfDirection(int direction, Vector2d from, char) const = 0;
	};

	class Ju: public Base {
		public:
			static constexpr u32string_view nameRed{U"俥"}, nameBlack{U"車"};
			static constexpr string_view nameEn{"j"};
			u32string_view getNameRed() const { return nameRed; }
			u32string_view getNameBlack() const { return nameBlack; }
			string_view getNameEn() const { return nameEn; }

			using Base::Base;
			bool isMoveCandidate(const Board &board, const Vector2d from, const Vector2d to) const override;
			std::optional<Vector2d> destOfDirection(int direction, Vector2d from, char) const override;
	};

	class Ma: public Base {
		public:
			static constexpr u32string_view nameRed{U"傌"}, nameBlack{U"馬"};
			static constexpr string_view nameEn{"m"};
			u32string_view getNameRed() const { return nameRed; }
			u32string_view getNameBlack() const { return nameBlack; }
			string_view getNameEn() const { return nameEn; }

			using Base::Base;
			bool isMoveCandidate(const Board &board, const Vector2d from, const Vector2d to) const override;
			std::optional<Vector2d> destOfDirection(int direction, Vector2d from, char) const override;
	};

	class Pao: public Base {
		public:
			static constexpr u32string_view nameRed{U"炮"}, nameBlack{U"砲"};
			static constexpr string_view nameEn{"p"};
			u32string_view getNameRed() const { return nameRed; }
			u32string_view getNameBlack() const { return nameBlack; }
			string_view getNameEn() const { return nameEn; }

			using Base::Base;
			bool isMoveCandidate(const Board &board, const Vector2d from, const Vector2d to) const override;
			std::optional<Vector2d> destOfDirection(int direction, Vector2d from, char) const override;
	};

	class Shi: public Base {
		public:
			static constexpr u32string_view nameRed{U"仕"}, nameBlack{U"士"};
			static constexpr string_view nameEn{"s"};
			u32string_view getNameRed() const { return nameRed; }
			u32string_view getNameBlack() const { return nameBlack; }
			string_view getNameEn() const { return nameEn; }

			bool isShiPosition(const Vector2d p) const;

			using Base::Base;
			bool isMoveCandidate(const Board &board, const Vector2d from, const Vector2d to) const override;
			std::optional<Vector2d> destOfDirection(int direction, Vector2d from, char) const override;
	};

	class Xiang: public Base {
		public:
			static constexpr u32string_view nameRed{U"相"}, nameBlack{U"象"};
			static constexpr string_view nameEn{"x"};
			u32string_view getNameRed() const { return nameRed; }
			u32string_view getNameBlack() const { return nameBlack; }
			string_view getNameEn() const { return nameEn; }

			bool isXiangPosition(const Vector2d p) const;

			using Base::Base;
			bool isMoveCandidate(const Board &board, const Vector2d from, const Vector2d to) const override;
			std::optional<Vector2d> destOfDirection(int direction, Vector2d from, char) const override;
	};

	class Jiang: public Base {
		public:
			static constexpr u32string_view nameRed{U"帥"}, nameBlack{U"將"};
			static constexpr string_view nameEn{"w"};
			u32string_view getNameRed() const { return nameRed; }
			u32string_view getNameBlack() const { return nameBlack; }
			string_view getNameEn() const { return nameEn; }

			using Base::Base;
			bool isMoveCandidate(const Board &board, const Vector2d from, const Vector2d to) const override;
			std::optional<Vector2d> destOfDirection(int direction, Vector2d from, char) const override;
	};

	class Zu: public Base {
		public:
			u32string_view getNameRed() const { return nameRed; }
			u32string_view getNameBlack() const { return nameBlack; }
			string_view getNameEn() const { return nameEn; }

			bool isZuPosition(const Vector2d p) const;

			static constexpr u32string_view nameRed{U"兵"}, nameBlack{U"卒"};
			static constexpr string_view nameEn{"z"};
			using Base::Base;
			bool isMoveCandidate(const Board &board, const Vector2d from, const Vector2d to) const override;
			std::optional<Vector2d> destOfDirection(int direction, Vector2d from, char) const override;
	};
}

using Piece = PieceNS::Base;

#endif
