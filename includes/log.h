#ifdef DEBUG
//Use it the same As printf
#define LOG(fmt,...) printf(fmt,##__VA_ARGS__)
#else
#define LOG(...)
#endif