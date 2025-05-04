#include "AutomataSimulator/c_api/c_util.h"

void freeStringArray(StringArray array) {
	if (array.data) {
		for (size_t i = 0; i < array.length; ++i) {
			if (array.data[i]) {
				free(array.data[i]);
			}
		}
		delete[] array.data;
	}
}

const char *convertToCString(const std::string &str) {
	return strdup(str.c_str());
}

inline void free_c_string(char *str) {
	if (str) {
		free(str);
	}
}