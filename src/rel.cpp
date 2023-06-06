// rel.cpp - REL init

extern "C" {

typedef void (*PFN_voidfunc)();
__attribute__((section(".ctors"))) extern PFN_voidfunc _ctors[];
__attribute__((section(".ctors"))) extern PFN_voidfunc _ctors_end[];

void _prolog();
void _epilog();
void _unresolved();

}

void codeLoaded();

__declspec(noinline) void _prolog()
{
    // Run global constructors
    for (PFN_voidfunc* ctor = _ctors; ctor != _ctors_end && *ctor; ++ctor) {
        (*ctor)();
    }

    // Run our main function
    codeLoaded();
}

__declspec(noinline) void _epilog()
{
    // Infinite loop
    _epilog();
}

__declspec(noinline) void _unresolved()
{
    // Infinite loop
    _unresolved();
}

#ifdef __clang__
int __dso_handle;
extern "C" void __cxa_atexit()
{
}
#endif