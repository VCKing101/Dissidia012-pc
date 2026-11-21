/* Sets a shake request of kind 0 for (a, b): takes the matching slot (or a
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

int func_08A62660(void *shaker, int a, int b, unsigned char once, float v0, float v1, float v2, float v3, float v4,
                  float v5, float v6, float v7, float v8, float v9)
{
    int kind;
    ShakeSlot *slot;
    if (func_08A62EF8(shaker) == 0) {
        return -1;
    }
    kind = 0;
    slot = func_08A62FA0(shaker, kind, a, b);
    if (slot == 0) {
        return -2;
    }
    if (!(slot->state < 3 && slot->a == a && slot->b == b && slot->kind == kind)) {
        func_08935828(slot, 0, sizeof(ShakeSlot));
    }
    slot->kind = kind;
    slot->a = a;
    slot->values[0] = v0;
    slot->b = b;
    slot->values[1] = v1;
    slot->values[2] = v2;
    slot->values[3] = v3;
    slot->values[4] = v4;
    slot->values[5] = v5;
    slot->values[6] = v6;
    slot->values[7] = v7;
    slot->values[8] = v8;
    slot->values[9] = v9;
    slot->once = once;
    slot->active = 1;
    slot->state = 0;
    slot->time = 0.0f;
    return 0;
}
