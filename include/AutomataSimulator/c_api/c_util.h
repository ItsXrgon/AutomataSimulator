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

AUTOMATASIMULATOR_EXTERN_C_BEGIN

AUTOMATASIMULATOR_EXPORT inline void free_c_string(char *str);

AUTOMATASIMULATOR_EXPORT inline void free_automaton_error(AutomatonError *error) {
	if (error) {
		if (error->message) {
			free(error->message);
			error->message = nullptr;
		}
	}
}

AUTOMATASIMULATOR_EXTERN_C_END

void freeStringArray(StringArray array);

const char *convertToCString(const std::string &str);

template <typename Func> inline void wrap_result(Func func, AutomatonError *error = nullptr) {
	AutomatonError local_error = create_success_error();

	try {
		func();
	} catch (const std::exception &ex) {
		local_error = handle_exception_with_message(ex);
	} catch (...) {
		local_error = create_error(AUTOMATON_ERR_UNKNOWN, "Unknown error occurred");
	}

	if (error) {
		try {
			free_automaton_error(error);
			copy_automaton_error(error, &local_error);
		} catch (...) {
			error->code = local_error.code;
			error->message = nullptr;
		}
	}
}

inline const StringArray convertToStringArray(const std::vector<std::string> &vec) {
	StringArray result;
	result.length = vec.size();
	result.data = new char *[result.length];

	for (size_t i = 0; i < result.length; ++i) {
		result.data[i] = new char[vec[i].size() + 1];
		strcpy_s(result.data[i], vec[i].size() + 1, vec[i].c_str());
	}

	return result;
}

template <typename T, typename Func>
inline T wrap_result(Func func, AutomatonError *error = nullptr, T default_value = T()) {
	AutomatonError local_error = create_success_error();

	try {
		copy_automaton_error(error, &local_error);
		return func();
	} catch (const std::exception &ex) {
		local_error = handle_exception_with_message(ex);
		if (error) {
			try {
				free_automaton_error(error);
				copy_automaton_error(error, &local_error);
			} catch (...) {
				error->code = local_error.code;
				error->message = nullptr;
			}
		}
		return default_value;
	} catch (...) {
		local_error = create_error(AUTOMATON_ERR_UNKNOWN, "Unknown error occurred");

		if (error) {
			try {
				free_automaton_error(error);
				copy_automaton_error(error, &local_error);
			} catch (...) {
				error->code = local_error.code;
				error->message = nullptr;
			}
		}
		return default_value;
	}
}
