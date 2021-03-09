#ifndef TEAM_HPP
#define TEAM_HPP

#include "color.hpp"

#include <string_view>

enum class Team { red, black };

inline Team otherTeam(const Team &t) { return t==Team::red ?Team::black :Team::red; }
inline std::u32string_view getTeamColor(const Team &t) { return t==Team::red ?TerminalColor::lightRed :TerminalColor::lightBlue; }
inline std::string_view cnName(Team t) { return t==Team::red ?"红" :"黑"; }

#endif
