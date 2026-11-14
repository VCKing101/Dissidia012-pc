typedef struct SdkRefHeader {
    unsigned short ref;
} SdkRefHeader;

extern void func_089BB3F8(SdkRefHeader *obj);
extern void func_089BB0E0(SdkRefHeader *obj);

void func_089BB460(SdkRefHeader *obj)
{
    if (obj != 0) {
        unsigned short ref = obj->ref;
        func_089BB3F8(obj);
        func_089BB0E0(obj);
        obj->ref = ref;
    }
}
