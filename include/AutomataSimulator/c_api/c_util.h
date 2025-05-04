#pragma once
#include "../AutomatonException.h"
#include "../config.h"
#include "AutomatonException_c.h"
#include <exception>
#include <functional>
#include <string>

typedef struct {
	char **data;
	size_t length;
} StringArray;

void freeStringArray(StringArray array);

const char *convertToCString(const std::string &str);

template <typename Func> inline void wrap_result(Func func, AutomatonError *error = nullptr) {
	AutomatonError local_error = create_success_error();

	try {
		func();
	} catch (const std::exception &ex) {
		local_error = handle_exception_with_message(ex);
		if (error) {
			*error = local_error;
		} else {
			free_automaton_error(&local_error);
		}
		return;
	}

	if (error) {
		*error = local_error;
	}
}

inline StringArray convertToStringArray(const std::vector<std::string> &vec) {
	StringArray result;
	result.length = vec.size();
	result.data = new char *[result.length];

	for (size_t i = 0; i < result.length; ++i) {
		result.data[i] = new char[vec[i].size() + 1];
		std::strcpy(result.data[i], vec[i].c_str());
	}

	return result;
}

template <typename T, typename Func>
inline T wrap_result(Func func, AutomatonError *error = nullptr, T default_value = T()) {
	AutomatonError local_error = create_success_error();
	T result;

	try {
		result = func();
	} catch (const std::exception &ex) {
		local_error = handle_exception_with_message(ex);
		if (error) {
			*error = local_error;
		} else {
			free_automaton_error(&local_error);
		}
		return default_value;
	}

	if (error) {
		*error = local_error;
	}
	return result;
}

AUTOMATASIMULATOR_EXTERN_C_BEGIN

AUTOMATASIMULATOR_EXPORT inline void free_c_string(char *str);

AUTOMATASIMULATOR_EXTERN_C_END
