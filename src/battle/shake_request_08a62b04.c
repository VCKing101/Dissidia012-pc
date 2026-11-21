/* Sets the global shake request (kind 1, no a/b): takes the matching slot (or a
 * free one, cleared), stores the ten shake values and starts it; -1 when
 * shaking is off, -2 when no slot is free. */
typedef struct ShakeSlot {
    unsigned char pad_00[0x20];
    int kind;
    int a;
    int b;
    float values[10];
    unsigned char once;
    unsigned char active;
    unsigned char pad_56[2];
    unsigned int state;
    float time;
} ShakeSlot;

extern int func_08A62EF8(void *shaker);
extern ShakeSlot *func_08A62FA0(void *shaker, int kind, int a, int b);
extern void func_08935828(void *dst, int value, int size);

int func_08A62B04(void *shaker, float v2, float v3, float v6, float v7, float v8, float v9)
{
    int kind;
    int any;
    ShakeSlot *slot;
    if (func_08A62EF8(shaker) == 0) {
        return -1;
    }
    kind = 1;
    any = -1;
    slot = func_08A62FA0(shaker, kind, any, any);
    if (slot == 0) {
        return -2;
    }
    if (!(slot->state < 3 && slot->a == any && slot->b == any && slot->kind == kind)) {
        func_08935828(slot, 0, sizeof(ShakeSlot));
    }
    slot->kind = kind;
    slot->a = any;
    slot->values[0] = 0.0f;
    slot->b = any;
    slot->values[1] = 0.0f;
    slot->values[2] = v2;
    slot->values[3] = v3;
    slot->values[4] = 0.0f;
    slot->values[5] = 0.0f;
    slot->values[6] = v6;
    slot->values[7] = v7;
    slot->values[8] = v8;
    slot->values[9] = v9;
    slot->once = 1;
    slot->active = 0;
    slot->state = 0;
    slot->time = 0.0f;
    return 0;
}
