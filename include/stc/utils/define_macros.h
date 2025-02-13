#ifndef STC_MACROS_DEFINED
#define STC_MACROS_DEFINED

/// Version

#if __cplusplus >= 202002L
#define IF_CPP20(code) code
#else
#define IF_CPP20(code)
#endif

#if __cplusplus >= 201703L
#define IF_CPP17(code) code
#else
#define IF_CPP17(code)
#endif

#if __cplusplus >= 201402L
#define IF_CPP14(code) code
#else
#define IF_CPP14(code)
#endif

/// Utils

#define CONSTEXPR20 IF_CPP20(constexpr)
#define NODISCARD IF_CPP17([[nodiscard]])
#define NOEXCEPT17 IF_CPP17(noexcept)

#endif // !STC_MACROS_DEFINED
