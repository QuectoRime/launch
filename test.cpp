#include <fstream>

#include "include/launch.h"

const char* artwork =
R"( ___       ________  ___  ___  ________   ________  ___  ___     )""\n"
R"(|\  \     |\   __  \|\  \|\  \|\   ___  \|\   ____\|\  \|\  \    )""\n"
R"(\ \  \    \ \  \|\  \ \  \\\  \ \  \\ \  \ \  \___|\ \  \\\  \   )""\n"
R"( \ \  \    \ \   __  \ \  \\\  \ \  \\ \  \ \  \    \ \   __  \  )""\n"
R"(  \ \  \____\ \  \ \  \ \  \\\  \ \  \\ \  \ \  \____\ \  \ \  \ )""\n"
R"(   \ \_______\ \__\ \__\ \_______\ \__\\ \__\ \_______\ \__\ \__\)""\n"
R"(    \|_______|\|__|\|__|\|_______|\|__| \|__|\|_______|\|__|\|__|)";

long long hedgehog_test0() {
	l::hedgehog cont;
	for (int i = 0; i < 256; ++i) {
		cont.push_back(i);
		cont.push_back(i / 10.0);
		cont.push_back(i % 2 == 0);
		cont.push_back(std::to_string(i));
	}
	const size_t cont_size = cont.size();
	std::string fmt = "[ ]";
	l::stopwatch watch;
	watch.start();
	l::fmtout(fmt, cont, l::nullout);
	watch.stop();
	return watch.get_dur().microseconds();
}

long long hedgehog_test1() {
	l::hedgehog cont;
	for (int i = 0; i < 256; ++i) {
		cont.push_back(0);
		cont.push_back(0ll);
		cont.push_back(0.0f);
		cont.push_back(0.0);
	}
	const size_t cont_size = cont.size();
	l::stopwatch watch;
	watch.start();
	for (int i = 0; i < cont_size; ++i) {
		cont[i] += i / 4;
	}
	watch.stop();
	return watch.get_dur().microseconds();
}

void hedgehog_test2() {
	l::hedgehog cont = { 42, 3.14, std::string("Hello World!") };
	l::fmtout("int: {}, double: {}, std::string: {}\n", cont);
	for (l::hedgehog_elemproxy& elem : cont) {
		try {
			elem += 1;
		}
		catch (const std::runtime_error& e) {
			l::fmtout("Oops: {} (Please don't worry, this is intentional.)\n", { e.what() });
		}
	}
	l::fmtout("int: {}, double: {}, std::string: {}\n", cont);
}

int main(int argc, char* argv[]) {
	try {
		l::clap parser(argc, argv, {
			{ "e", "escseq" },
			{ "h", "hedgehog" },
			{ "m", "goodmath" },
			{ "s", "goodstr" },
			{ "a", "arithing" },
			{ "f", "exfmtio" },
			{ "g", "genev" }
			});
		std::cout << std::boolalpha;
		std::cin >> std::boolalpha;
		std::ifstream file("splashes.txt");
		std::vector<std::string> splashes;
		if (file.is_open()) {
			std::string line;
			while (std::getline(file, line)) {
				splashes.push_back(line);
			}
		}
		file.close();
		if (!splashes.size()) {
			splashes.push_back("Unable to load splashes!");
		}
		l::randgen_int<size_t> rand(0, splashes.size() - 1);
		const size_t splash_index = rand.next();
		l::fmtout("[]\n", {
			artwork,
			l::foreground_color(255, 255, 0),
			l::cursor_left(65),
			l::VERSION,
			l::cursor_right(65 - std::strlen(l::VERSION) - splashes[splash_index].length()),
			splashes[splash_index],
			l::rendl_fast
		});
		size_t cnt = 0;
		l::fmtout("=== START OF PROGRAM ===\n");
		if (parser.get_flag("escseq") || parser.get_flag("all")) {
			++cnt;
			l::fmtout("=== START OF TEST - ESCSEQ ===\n");
			l::fmtout("[]Hyperlink\n", {
				l::gen_style(l::escseq_style::italic | l::escseq_style::underline),
				l::foreground_color(0, 0, 255),
				l::background_color(255, 255, 255)
			});
			l::fmtout("[Hyperlink (clicked)]", {
				l::foreground_color(128, 0, 255),
				l::rendl_fast
			});
			l::fmtout("{0}int {1}var {2}= {3}42{2}; {4}// This is the palette of One Dark Pro{5}", {
				l::foreground_color(198, 120, 221),
				l::foreground_color(224, 108, 117),
				l::foreground_color(97, 175, 239),
				l::foreground_color(209, 154, 102),
				l::foreground_color(127, 132, 142),
				l::rendl_fast
			});
			l::fmtout("=== END OF TEST - ESCSEQ ===\n");
		}
		if (parser.get_flag("hedgehog") || parser.get_flag("all")) {
			++cnt;
			l::fmtout("=== START OF TEST - HEDGEHOG ===\n");
			l::fmtout("Test 1, printing 1024 elements: {} us\n", { hedgehog_test0() });
			l::fmtout("Test 2, calculating 1024 times: {} us\n", { hedgehog_test1() });
			l::fmtout("Test 3:\n");
			hedgehog_test2();
			l::fmtout("=== END OF TEST - HEDGEHOG ===\n");
		}
		if (parser.get_flag("goodmath") || parser.get_flag("all")) {
			++cnt;
			l::fmtout("=== START OF TEST - GOODMATH ===\n");
			double deg;
			l::fmtout("Enter an angle (in degrees) please: ");
			l::fmtin(deg);
			const double
				rad = l::dtor(deg),
				cos_ = l::cos(rad),
				sin_ = l::sin(rad);
			l::fmtout(
				"If O is at (0, 0), A is at (1, 0), both OA and OP are 1, "
				"and angle AOP is {} degrees, then P is at ({}, {}).\n"
				"By the way, this should be 1: {}.\n",
				{ deg, cos_, sin_, l::pow(cos_, 2) + l::pow(sin_, 2) }
			);
			l::fmtout("=== END OF TEST - GOODMATH ===\n");
		}
		if (parser.get_flag("goodstr") || parser.get_flag("all")) {
			++cnt;
			l::fmtout("=== START OF TEST - GOODSTR ===\n");
			std::string str0, str1, str2;
			l::fmtout("Enter three strings:\n");
			l::fmtin_line(str0);
			l::fmtin_line(str1);
			l::fmtin_line(str2);
			l::fmtout(
				"After replacing \"{}\"s in \"{}\" with \"{}\": {}\n",
				{ str1, str0, str2, l::replace(str0, str1, str2) }
			);
			l::fmtout("=== END OF TEST - GOODSTR ===\n");
		}
		if (parser.get_flag("arithing") || parser.get_flag("all")) {
			++cnt;
			l::fmtout("=== START OF TEST - ARITHING ===\n");
			l::arrat lhs, rhs, res;
			char op;
			l::fmtout("Enter a math expression please (e.g. 1 + 2, -1/2 * 3/4): ");
			l::fmtin("[ ]", l::pack(lhs, op, rhs));
			switch (op) {
			case '+':
				res = lhs + rhs;
				break;
			case '-':
				res = lhs - rhs;
				break;
			case '*':
				res = lhs * rhs;
				break;
			case '/':
				res = lhs / rhs;
				break;
			case '%':
				res = lhs % rhs;
				break;
			default:
				res = l::arint(0, l::arint_specval::nan);
			}
			if (l::denominator(res) == 1) {
				l::fmtout("{} {} {} = {}\n", { lhs, op, rhs, res });
			}
			else {
				l::fmtout("{} {} {} = {} = {}\n", { lhs, op, rhs, res, l::to_decimal(res) });
			}
			l::fmtout("=== END OF TEST - ARITHING ===\n");
		}
		if (parser.get_flag("exfmtio") || parser.get_flag("all")) {
			++cnt;
			l::fmtout("=== START OF TEST - EXFMTIO ===\n");
			size_t cnt;
			l::hedgehog cont;
			std::string fmt = "----------------------------------------\nYou entered:\n";
			l::fmtout("How many elements you're about to enter? ");
			l::fmtin(cnt);
			l::fmtout("----------------------------------------\nPlese enter {} element(s):\n", { cnt });
			l::fmtin_deduce_auto(cont, cnt);
			const size_t cont_size = cont.size();
			for (size_t i = 0; i < cont_size; ++i) {
				fmt = fmt + "{" + std::to_string(i) + "}: {" + std::to_string(i + cont_size) + "}\n";
				cont.push_back(cont[i].type().name());
			}
			l::fmtout(fmt, cont);
			l::fmtout("=== END OF TEST - EXFMTIO ===\n");
		}
		if (parser.get_flag("genev") || parser.get_flag("all")) {
			++cnt;
			l::fmtout("=== START OF TEST - GENEV ===\n");
			l::genev mass, accel;
			l::fmtout("Mass: ");
			l::fmtin(mass);
			l::fmtout("Acceleration: ");
			l::fmtin(accel);
			l::fmtout("F = ma = {} * {} = {}\n", { mass, accel, mass * accel });
			l::fmtout("=== END OF TEST - GENEV ===\n");
		}
		l::fmtout("=== END OF PROGRAM - {} TEST(S) EXECUTED ===\n", { cnt });
		if (!cnt) {
			l::fmtout(
				"\nOh, it seems like that none of our wonderful tests have been executed!\n"
				"Please consider running this program with one or more options.\n"
				"The available options are:\n"
				"--escseq (-e)\n"
				"--hedgehog (-h)\n"
				"--goodmath (-m)\n"
				"--goodstr (-s)\n"
				"--arithing (-a)\n"
				"--exfmtio (-f)\n"
				"--genev (-g)\n"
				"--all\n"
			);
		}
	}
	catch (const std::exception& e) {
		std::cerr << "ERROR: " << e.what() << "\n";
		std::cerr << "Program exited abnormally.\n";
		return -1;
	}
	return 0;
}