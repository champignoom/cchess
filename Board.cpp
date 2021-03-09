#include "Board.hpp"
#include "Piece.hpp"
#include "Vector2d.hpp"

#include <optional>
#include <array>
#include <memory>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <string_view>
#include <vector>

using std::optional;
using std::nullopt;
using std::unique_ptr;
using std::make_unique;
using std::cout;
using std::endl;
using std::array;
using std::vector;
using std::u32string;
using std::string;
using std::string_view;
using namespace std::literals;

bool Board::inBound(const Vector2d p) {
	return 0<=p.x && p.x<N_ROW && 0<=p.y && p.y<N_COL;
}

Vector2d Board::toTeam(const Team team, const Vector2d p) {
	assert(inBound(p));
	return team==Team::black ?p :Vector2d{N_ROW-1-p.x, N_COL-1-p.y};
}

bool Board::inTeam(const Team team, const Vector2d p) {
	assert(inBound(p));
	return team==Team::black ?(p.x<N_ROW/2) :(p.x>=N_ROW/2);
}

bool Board::inBase(const Team team, const Vector2d p) {
	assert(inBound(p));
	const Vector2d pt{toTeam(team, p)};
	return pt.x<=2 && 3<=pt.y && pt.y<=5;
}

unique_ptr<Piece> &Board::pieceAt(const Vector2d p) {
	assert(inBound(p));
	return board[p.x][p.y];
}

const unique_ptr<Piece> &Board::pieceAt(const Vector2d p) const {
	assert(inBound(p));
	return board[p.x][p.y];
}

bool Board::pieceExist(const Vector2d p) const {
	assert(inBound(p));
	return pieceAt(p).operator bool();
}

optional<int> Board::countPiecesBetween(const Vector2d from, const Vector2d to) const {
	assert(inBound(from));
	assert(inBound(to));

	Vector2d diff = to - from;
	if (diff.isZero()) return nullopt;
	if (!diff.isOnAxis()) return nullopt;

	const Vector2d delta = diff.quandrant();
	int result = 0;
	for (Vector2d p{from+delta}; p!=to; p+=delta) {
		if (pieceExist(p)) {
			++result;
		}
	}

	return result;
}

bool Board::isMoveable(const Vector2d from, const Vector2d to) const {
	assert(inBound(from));
	assert(inBound(to));
	assert(from != to);
	assert(pieceExist(from));

	const unique_ptr<Piece> &pFrom = pieceAt(from);
	const unique_ptr<Piece> &pTo = pieceAt(to);
	if (!pFrom->isMoveCandidate(*this, from, to)) return false;
	return !pieceExist(to) || pTo->team != pFrom->team;
}

void Board::makeMove(const Vector2d from, const Vector2d to) {
	assert(isMoveable(from, to));

	pieceAt(to) = std::move(pieceAt(from));
}

void Board::print() const {
	array<u32string, 2*N_ROW+1> strs {
		{   U"一  二  三  四  五  六  七  八  九",
			U"┌───┬───┬───┬───┬───┬───┬───┬───┐    -0",
			U"│   │   │   │ ╲ │ ╱ │   │   │   │     ",
			U"├───┼───┼───┼───┼───┼───┼───┼───┤    -1",
			U"│   │   │   │ ╱ │ ╲ │   │   │   │     ",
			U"├───╬───┼───┼───┼───┼───┼───╬───┤    -2",
			U"│   │   │   │   │   │   │   │   │     ",
			U"╠───┼───╬───┼───╬───┼───╬───┼───╣    -3",
			U"│   │   │   │   │   │   │   │   │     ",
			U"├───┴───┴───┴───┴───┴───┴───┴───┤    -4",
			U"│      楚河            漢界     │     ",
			U"├───┬───┬───┬───┬───┬───┬───┬───┤    -5",
			U"│   │   │   │   │   │   │   │   │     ",
			U"╠───┼───╬───┼───╬───┼───╬───┼───╣    -6",
			U"│   │   │   │   │   │   │   │   │     ",
			U"├───╬───┼───┼───┼───┼───┼───╬───┤    -7",
			U"│   │   │   │ ╲ │ ╱ │   │   │   │     ",
			U"├───┼───┼───┼───┼───┼───┼───┼───┤    -8",
			U"│   │   │   │ ╱ │ ╲ │   │   │   │     ",
			U"└───┴───┴───┴───┴───┴───┴───┴───┘    -9",
			U"９  ８  ７  ６  ５  ４  ３  ２  １",
		}};

	for (int i=0; i<N_ROW; ++i) {
		for (int j=N_COL-1; j>=0; --j) {
			if (!pieceExist({i,j})) continue;
			const Piece &p=*pieceAt({i,j});
			const u32string coloredName = u32string(getTeamColor(p.team)) + u32string(p.getName()) + u32string(TerminalColor::restore);
			strs[1+i*2].replace(j*4, 2, coloredName);
		}
	}

	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> cvt;

	for (const u32string &s: strs) {
		cout <<cvt.to_bytes(s) <<endl;
	}
}

namespace {
	int parseKthInCol(char kthInCol) {
		assert(kthInCol=='q' || kthInCol=='h' || '2'<=kthInCol || kthInCol<='5');

		return kthInCol=='q' ?0 :kthInCol=='h' ?-1 :kthInCol-'1';
	}

	int parseDirection(Team team, char direction) {
		assert("jtp"sv.find(direction) != string_view::npos);
		return (team==Team::black ?1 :-1) * (direction=='j' ?1 :direction=='t' ?-1 :0);
	}
}

int Board::colToTeam(Team team, int col) {
	return Board::toTeam(team, {0,col}).y;
}

vector<Vector2d> Board::getPiecesOfCol(Team team, char enPieceName, char col) const {
	int c = col-'1';
	vector<Vector2d> result;
	for (int i=N_ROW-1; i>=0; --i) {
		Vector2d p{toTeam(team, {i,c})};
		if (pieceExist(p) && pieceAt(p)->team==team && pieceAt(p)->getNameEn()[0]==enPieceName) {
			result.push_back(p);
		}
	}

	return result;
}

optional<Vector2d> Board::parseKthPieceAtCol(Team team, char enPieceName, char kthInCol, char col) const {
	int k = parseKthInCol(kthInCol);
	vector<Vector2d> candidates{getPiecesOfCol(team, enPieceName, col)};
	if (candidates.size()<2 || (int)candidates.size()<=k) return nullopt;
	return k==-1 ?candidates.back() :candidates[k];
}

optional<Vector2d> Board::parseSinglePiece(Team team, char enPieceName, char col) const {
	assert("jmpxswbz"sv.find(enPieceName) != string_view::npos);
	assert('1' <= col);
	assert(col <= '9');

	vector<Vector2d> candidates{getPiecesOfCol(team, enPieceName, col)};
	if (candidates.size()==1) return candidates[0];
	return nullopt;
}

optional<Vector2d> Board::parseZuAtCol(Team team, char kthInCol, char col) const {
	return parseKthPieceAtCol(team, PieceNS::Zu::nameEn[0], kthInCol, col);
}

optional<Vector2d> Board::parseMultiple(Team team, char kthInCol, char enPieceName) const {
	optional<Vector2d> result;
	for (char col='1'; col<='9'; ++col) {
		if (optional<Vector2d> p=parseKthPieceAtCol(team, enPieceName, kthInCol, col)) {
			if (!result.has_value()) {
				result = p;
			} else {
				return nullopt;
			}
		}
	}

	assert(result.has_value());
	return result;
}

optional<Vector2d> Board::parseDestByDirection(Vector2d from, char direction, char c) const {
	assert(pieceExist(from));

	const unique_ptr<Piece> &p{pieceAt(from)};
	assert(p.operator bool());
	return p->destOfDirection(parseDirection(p->team, direction), from, c);
}

namespace {
	template <typename T> inline unique_ptr<T> m(Team t) { return make_unique<T>(t); }
}

Board Board::makeStandardBoard() {
	using namespace PieceNS;
	constexpr Team r=Team::red;
	constexpr Team b=Team::black;

	return Board{{{
		{   m<Ju>(b),    m<Ma>(b), m<Xiang>(b),   m<Shi>(b), m<Jiang>(b),   m<Shi>(b), m<Xiang>(b),    m<Ma>(b),    m<Ju>(b)}
		,{   nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr}
		,{   nullptr,   m<Pao>(b),     nullptr,     nullptr,     nullptr,     nullptr,     nullptr,   m<Pao>(b),     nullptr}
		,{  m<Zu>(b),     nullptr,    m<Zu>(b),     nullptr,    m<Zu>(b),     nullptr,    m<Zu>(b),     nullptr,    m<Zu>(b)}
		,{   nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr}
		,{   nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr}
		,{  m<Zu>(r),     nullptr,    m<Zu>(r),     nullptr,    m<Zu>(r),     nullptr,    m<Zu>(r),     nullptr,    m<Zu>(r)}
		,{   nullptr,   m<Pao>(r),     nullptr,     nullptr,     nullptr,     nullptr,     nullptr,   m<Pao>(r),     nullptr}
		,{   nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr,     nullptr}
		,{  m<Ju>(r),    m<Ma>(r), m<Xiang>(r),   m<Shi>(r), m<Jiang>(r),   m<Shi>(r), m<Xiang>(r),    m<Ma>(r),    m<Ju>(r)}
	}}};
}
