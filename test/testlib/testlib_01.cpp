
#if defined(_WIN32)
extern "C" __declspec(dllexport) int f42(){
    return 42;
}
#else
extern "C" int f42(){
    return 42;
}
#endif
