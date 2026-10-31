#pragma once

#include <format>
#include <iostream>
#include <istream>
#include <stdexcept>
#include <string>

#if !defined(LAUNCH_NO_HEDGEHOG)
#include "hedgehog.h"
#endif

namespace launch {
	void fmtout_raw(const std::string& fmt, std::ostream& out = std::cout);

	void fmtin_line(std::string& str, std::istream& in = std::cin);

#if !defined(LAUNCH_NO_HEDGEHOG)
	void fmtout(const std::string& fmt, const hedgehog& cont = {}, std::ostream& out = std::cout);

	template <readable... Ts>
	hedgehog pack(Ts&... args) {
		hedgehog cont;
		(cont.push_back(std::ref(args)), ...);
		return cont;
	}

	void fmtin(const hedgehog& cont, std::istream& in = std::cin);

	template <readable T>
	void fmtin(T& x, std::istream& in = std::cin) {
		in >> x;
	}
#endif
}