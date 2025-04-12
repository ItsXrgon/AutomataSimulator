#pragma once

#ifdef _WIN32
#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif
#else
#define AUTOMATASIMULATOR_API
#endif