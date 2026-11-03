#include "../include/launch_config.h"

#include "../include/fmtio_impl.h"

namespace launch {
	void fmtout_raw(const std::string& fmt, std::ostream& out) {
		out << fmt;
	}

	void fmtin_line(std::string& str, std::istream& in) {
		std::getline(in >> std::ws, str);
	}

#if !defined(LAUNCH_NO_HEDGEHOG)
	void fmtout(const std::string& fmt, const hedgehog& cont, std::ostream& out) {
		if (fmt.find_first_of("{}[]|") == std::string::npos) {
			out << fmt;
			return;
		}
		std::stringstream ss;
		ss.flags(out.flags());
		const size_t fmt_length = fmt.length();
		const size_t cont_size = cont.size();
		bool manual_indexing = false;
		bool auto_indexing = false;
		size_t curr_index = 0;
		for (size_t i = 0; i < fmt_length; ++i) {
			switch (fmt[i]) {
			case '{':
				if (i + 1 == fmt_length) {
					throw std::format_error("fmtout: unmatched { at index " + std::to_string(i));
				}
				if (fmt[i + 1] == '{') {
					++i;
					ss << '{';
					break;
				}
				++i;
				{
					size_t index;
					if (fmt[i] == '}') {
						if (manual_indexing) {
							throw std::format_error("fmtout: unexpected } at index " + std::to_string(i));
						}
						auto_indexing = true;
						index = curr_index++;
					}
					else {
						if (auto_indexing) {
							throw std::format_error(
								std::string("fmtout: expected } but got ") +
								fmt[i] +
								" at index " +
								std::to_string(i)
							);
						}
						manual_indexing = true;
						std::string index_str;
						for (; fmt[i] != '}'; ++i) {
							if (i + 1 == fmt_length) {
								throw std::format_error("fmtout: unmatched { at index " + std::to_string(i));
							}
							if (fmt[i] < '0' || fmt[i] > '9') {
								throw std::format_error(
									std::string("fmtout: expected a digit but got ") +
									fmt[i] +
									" at index " +
									std::to_string(i)
								);
							}
							index_str += fmt[i];
						}
						index = std::stoull(index_str);
					}
					if (index >= cont_size) {
						throw std::format_error(
							"fmtout: index (which is " +
							std::to_string(index) +
							") >= cont.size() (which is " +
							std::to_string(cont_size) +
							")"
						);
					}
					ss << cont[index];
				}
				break;
			case '}':
				if (i + 1 != fmt_length && fmt[i + 1] == '}') {
					++i;
					ss << '}';
					break;
				}
				throw std::format_error("fmtout: unmatched } at index " + std::to_string(i));
			case '[':
				if (i + 1 == fmt_length) {
					throw std::format_error("fmtout: unmatched [ at index " + std::to_string(i));
				}
				if (fmt[i + 1] == '[') {
					++i;
					ss << '[';
					break;
				}
				++i;
				{
					std::string sep;
					for (; fmt[i] != ']' || i + 1 != fmt_length && fmt[i] == ']' && fmt[i + 1] == ']'; ++i) {
						if (i + 1 == fmt_length) {
							throw std::format_error("fmtout: unmatched [ at index " + std::to_string(i));
						}
						switch (fmt[i]) {
						case '[':
							if (fmt[i + 1] != '[') {
								throw std::format_error("fmtout: unexpected [ at index " + std::to_string(i));
							}
							++i;
							sep += '[';
							break;
						case ']':
							if (fmt[i + 1] == ']') {
								++i;
								sep += ']';
							}
							break;
						case '{':
							if (fmt[i + 1] != '{') {
								throw std::format_error("fmtout: unexpected { at index " + std::to_string(i));
							}
							++i;
							sep += '{';
							break;
						case '}':
							if (fmt[i + 1] != '}') {
								throw std::format_error("fmtout: unexpected } at index " + std::to_string(i));
							}
							++i;
							sep += '}';
							break;
						case '|':
							if (i + 1 != fmt_length && fmt[i + 1] == '|') {
								++i;
								sep += '|';
							}
							break;
						default:
							sep += fmt[i];
						}
					}
					for (size_t index = 0; index < cont_size; ++index) {
						ss << cont[index];
						if (index + 1 != cont_size) {
							ss << sep;
						}
					}
				}
				break;
			case ']':
				if (i + 1 != fmt_length && fmt[i + 1] == ']') {
					ss << ']';
					++i;
					break;
				}
				throw std::format_error("fmtout: unmatched ] at index " + std::to_string(i));
			case '|':
				if (i + 1 != fmt_length && fmt[i + 1] == '|') {
					++i;
					ss << '|';
				}
				break;
			default:
				ss << fmt[i];
			}
		}
		out << ss.rdbuf();
	}

	void fmtin(const std::string& fmt, const hedgehog& cont, std::istream& in) {
		const size_t fmt_length = fmt.length();
		const size_t cont_size = cont.size();
		bool manual_indexing = false;
		bool auto_indexing = false;
		size_t curr_index = 0;
		std::vector<bool> consumed(cont_size, false);
		const auto to_readable = [](int ch) -> std::string {
			std::string str;
			if (std::isgraph(ch)) {
				str = ch;
			}
			else if (ch != EOF) {
				str = "code point " + std::to_string(ch);
			}
			else {
				str = "EOF";
			}
			return str;
		};
		const auto expect = [&in, &to_readable](char expected) -> void {
			const int peeked = in.peek();
			if (peeked != expected) {
				throw std::runtime_error(
					std::string("fmtin: expected ") +
					to_readable(expected) +
					" but read " +
					to_readable(peeked)
				);
			}
			in.get();
		};
		const auto read = [&cont, &in, &consumed](size_t index) -> void {
			if (consumed[index]) {
				throw std::format_error(
					"fmtin: attempted to read an element (at index " +
					std::to_string(index) +
					") multiple times"
				);
			}
			in >> cont[index];
			if (in.fail()) {
				throw std::runtime_error("fmtin: failed to parse the element at index " + std::to_string(index));
			}
			consumed[index] = true;
		};
		in >> std::ws;
		for (size_t i = 0; i < fmt_length; ++i) {
			switch (fmt[i]) {
			case '{':
				if (i + 1 == fmt_length) {
					throw std::format_error("fmtin: unmatched { at index " + std::to_string(i));
				}
				if (fmt[i + 1] == '{') {
					++i;
					expect('{');
					break;
				}
				++i;
				{
					size_t index;
					if (fmt[i] == '}') {
						if (manual_indexing) {
							throw std::format_error("fmtin: unexpected } at index " + std::to_string(i));
						}
						auto_indexing = true;
						index = curr_index++;
					}
					else {
						if (auto_indexing) {
							throw std::format_error(
								std::string("fmtin: expected } but got ") +
								fmt[i] +
								" at index " +
								std::to_string(i)
							);
						}
						manual_indexing = true;
						std::string index_str;
						for (; fmt[i] != '}'; ++i) {
							if (i + 1 == fmt_length) {
								throw std::format_error("fmtin: unmatched { at index " + std::to_string(i));
							}
							if (fmt[i] < '0' || fmt[i] > '9') {
								throw std::format_error(
									std::string("fmtin: expected a digit but got ") +
									fmt[i] +
									" at index " +
									std::to_string(i)
								);
							}
							index_str += fmt[i];
						}
						index = std::stoull(index_str);
					}
					if (index >= cont_size) {
						throw std::format_error(
							"fmtin: index (which is " +
							std::to_string(index) +
							") >= cont.size() (which is " +
							std::to_string(cont_size) +
							")"
						);
					}
					read(index);
				}
				break;
			case '}':
				if (i + 1 != fmt_length && fmt[i + 1] == '}') {
					++i;
					expect('}');
					break;
				}
				throw std::format_error("fmtin: unmatched } at index " + std::to_string(i));
			case '[':
				if (i + 1 == fmt_length) {
					throw std::format_error("fmtin: unmatched [ at index " + std::to_string(i));
				}
				if (fmt[i + 1] == '[') {
					++i;
					expect('[');
					break;
				}
				++i;
				{
					std::string sep;
					for (; fmt[i] != ']' || i + 1 != fmt_length && fmt[i] == ']' && fmt[i + 1] == ']'; ++i) {
						if (i + 1 == fmt_length) {
							throw std::format_error("fmtin: unmatched [ at index " + std::to_string(i));
						}
						switch (fmt[i]) {
						case '[':
							if (fmt[i + 1] != '[') {
								throw std::format_error("fmtin: unexpected [ at index " + std::to_string(i));
							}
							++i;
							sep += '[';
							break;
						case ']':
							if (fmt[i + 1] == ']') {
								++i;
								sep += ']';
							}
							break;
						case '{':
							if (fmt[i + 1] != '{') {
								throw std::format_error("fmtin: unexpected { at index " + std::to_string(i));
							}
							++i;
							sep += '{';
							break;
						case '}':
							if (fmt[i + 1] != '}') {
								throw std::format_error("fmtin: unexpected } at index " + std::to_string(i));
							}
							++i;
							sep += '}';
							break;
						case '|':
							if (i + 1 != fmt_length && fmt[i + 1] == '|') {
								++i;
								sep += '|';
							}
							break;
						default:
							sep += fmt[i];
						}
					}
					if (sep.empty()) {
						throw std::format_error("fmtin: empty separator at index " + std::to_string(i));
					}
					for (size_t index = 0; index < cont_size; ++index) {
						read(index);
						if (index + 1 != cont_size) {
							const size_t sep_length = sep.length();
							for (size_t j = 0; j < sep_length; ++j) {
								expect(sep[j]);
							}
						}
					}
				}
				break;
			case ']':
				if (i + 1 != fmt_length && fmt[i + 1] == ']') {
					expect(']');
					++i;
					break;
				}
				throw std::format_error("fmtin: unmatched ] at index " + std::to_string(i));
			case '|':
				if (i + 1 != fmt_length && fmt[i + 1] == '|') {
					++i;
					expect('|');
				}
				break;
			default:
				expect(fmt[i]);
			}
		}
	}
#endif
}