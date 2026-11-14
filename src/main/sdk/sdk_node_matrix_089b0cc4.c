typedef struct SdkMat4 {
    float m[16];
} SdkMat4;

typedef struct SdkMatNode {
    char pad0[0x90];
    SdkMat4 mtx;
} SdkMatNode;

/* Identity matrix in .rodata. */
extern const SdkMat4 D_089D2650;

void func_089B0CC4(SdkMatNode *node, const SdkMat4 *m)
{
    if (node != 0) {
        if (m == 0) {
            m = &D_089D2650;
        }
        node->mtx = *m;
    }
}
