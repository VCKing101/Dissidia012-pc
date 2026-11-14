typedef struct SdkPair {
    int f0;
    void *first;
    void *second;
} SdkPair;

extern void func_089BB0C8(int pool, void *p);

SdkPair *func_089BB328(SdkPair *pair)
{
    if (pair != 0) {
        func_089BB0C8(0, pair->first);
        func_089BB0C8(0, pair->second);
    }
    return pair;
}
