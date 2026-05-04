#include "htable.h"

int char_to_int(char c) {
    if (c >= 'a' && c <= 'z') return c - 'a' + 1;
    if (c >= 'A' && c <= 'Z') return c - 'A' + 27;
    if (c == '_') return 53;
    return 0;
}

size_t hash(long long item) {
	return item;
}

size_t hash(std::string item) {
	size_t ret = 0;
	for (char ch: item) ret = ret * 57 + char_to_int(ch);
	return ret;
}
