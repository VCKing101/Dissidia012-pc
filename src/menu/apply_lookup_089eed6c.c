typedef struct LookupBuffer {
    char data[0x40];
} LookupBuffer;

extern void func_088DA0A8(int key, int kind, LookupBuffer *out, int *extra);
extern void func_0887B778(void *target, LookupBuffer *buffer);
extern void func_0887B820(void *target, int extra);

void func_089EED6C(void *target, int key, int kind)
{
    if (target != 0) {
        LookupBuffer buffer;
        int extra;
        int value;
        func_088DA0A8(key, kind, &buffer, &extra);
        value = extra;
        func_0887B778(target, &buffer);
        func_0887B820(target, value);
    }
}
