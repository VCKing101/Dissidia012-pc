/* Sets the player-only shake request (kind 2, no a/b; nothing for a CPU fighter): takes the matching slot (or a
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

class ShakeActor {
public:
    unsigned char pad_000[0x544];
    virtual void slot_000();
    virtual void slot_001();
    virtual void slot_002();
    virtual void slot_003();
    virtual void slot_004();
    virtual void slot_005();
    virtual void slot_006();
    virtual void slot_007();
    virtual void slot_008();
    virtual void slot_009();
    virtual void slot_010();
    virtual void slot_011();
    virtual void slot_012();
    virtual void slot_013();
    virtual void slot_014();
    virtual void slot_015();
    virtual void slot_016();
    virtual void slot_017();
    virtual void slot_018();
    virtual void slot_019();
    virtual void slot_020();
    virtual void slot_021();
    virtual void slot_022();
    virtual void slot_023();
    virtual void slot_024();
    virtual void slot_025();
    virtual void slot_026();
    virtual void slot_027();
    virtual void slot_028();
    virtual void slot_029();
    virtual void slot_030();
    virtual void slot_031();
    virtual void slot_032();
    virtual void slot_033();
    virtual void slot_034();
    virtual int is_player();
};

struct Shaker {
    unsigned char pad_00[4];
    ShakeActor *actor;
};

extern "C" {

extern int func_08A62EF8(Shaker *shaker);
extern ShakeSlot *func_08A62FA0(Shaker *shaker, int kind, int a, int b);
extern void func_08935828(void *dst, int value, int size);

int func_08A62C58(Shaker *shaker, float v2, float v3, float v6, float v7, float v8, float v9)
{
    int kind;
    int any;
    ShakeSlot *slot;
    if (func_08A62EF8(shaker) == 0) {
        return -1;
    }
    if (shaker->actor->is_player() == 0) {
        return 0;
    }
    kind = 2;
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

}
