struct Agent4Pair {
    int field_00;
    int field_04;
};

struct Agent4Pool {
    int count;
    unsigned char pool[0x20];
};

extern "C" {
extern Agent4Pair D_089D2800;
extern Agent4Pool D_089D2808;
void func_08809194(void);
void func_08993560(void *pool, int count, int size, void (*callback)(void));
}

struct Agent4PoolReset {
    static void reset(Agent4Pair *pair, Agent4Pool *pool, void (*callback)(void)) {
        pair->field_04 = 0;
        pair->field_00 = 0;
        pool->count = 0;
        func_08993560(pool->pool, 0xFA, 0x20, callback);
    }
};

extern "C" {
void func_0880A9A0(void) {
    Agent4PoolReset::reset(&D_089D2800, &D_089D2808, func_08809194);
}
}
