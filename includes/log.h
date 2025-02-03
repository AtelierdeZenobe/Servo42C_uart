#ifdef DEBUG
//Use it the same As printf
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif