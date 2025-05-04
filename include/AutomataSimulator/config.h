#pragma once

// DLL import/export
#ifdef _WIN32
#ifdef AUTOMATASIMULATOR_EXPORTS
#define AUTOMATASIMULATOR_API __declspec(dllexport)
#else
#define AUTOMATASIMULATOR_API __declspec(dllimport)
#endif
#else
#define AUTOMATASIMULATOR_API
#endif

// C linkage macros
#ifdef __cplusplus
#define AUTOMATASIMULATOR_EXTERN_C extern "C"
#define AUTOMATASIMULATOR_EXTERN_C_BEGIN extern "C" {
#define AUTOMATASIMULATOR_EXTERN_C_END }
#else
#define AUTOMATASIMULATOR_EXTERN_C
#define AUTOMATASIMULATOR_EXTERN_C_BEGIN
#define AUTOMATASIMULATOR_EXTERN_C_END
#endif

// Combined macro
#define AUTOMATASIMULATOR_EXPORT AUTOMATASIMULATOR_EXTERN_C AUTOMATASIMULATOR_API
