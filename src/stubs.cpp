#include <cstdlib>

extern "C" void __cxa_pure_virtual() {
	    abort();
}

void operator delete(void *, unsigned long) {}

void operator delete(void*, unsigned int) {}

void operator delete(void*, unsigned int, std::align_val_t) {}

extern "C" void __assert_func(const char *file, int line, const char *, const char *e) {}

extern "C" void __assert_fail(const char *__assertion, const char *__file, unsigned int __line,
		    const char *__function) {}

extern "C" [[noreturn]] void abort(void);

