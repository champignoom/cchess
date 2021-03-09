#ifndef COLOR_HPP
#define COLOR_HPP

#include <string_view>

namespace TerminalColor {
	constexpr std::u32string_view restore{U"\033[0m"};

	constexpr std::u32string_view lightRed{U"\033[91m"};
	constexpr std::u32string_view lightBlue{U"\033[96m"};
}

#endif
