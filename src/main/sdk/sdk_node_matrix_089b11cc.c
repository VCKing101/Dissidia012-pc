typedef struct SdkMat4 {
    float m[16];
} SdkMat4;

typedef struct SdkMatNode80 {
    char pad0[0x80];
    SdkMat4 mtx;
} SdkMatNode80;

/* Identity matrix in .rodata. */
extern const SdkMat4 D_089D2650;

void func_089B11CC(SdkMatNode80 *node, const SdkMat4 *m)
{
    if (node != 0) {
        if (m == 0) {
            m = &D_089D2650;
        }
        node->mtx = *m;
    }
}
