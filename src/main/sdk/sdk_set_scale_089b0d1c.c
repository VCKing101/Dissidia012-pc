typedef struct SdkNodeF {
    char pad0[0x1C];
    float f1c;
} SdkNodeF;

void func_089B0D1C(SdkNodeF *node, float value)
{
    if (node != 0) {
        node->f1c = value;
    }
}
