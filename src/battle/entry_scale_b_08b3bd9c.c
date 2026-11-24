/* Sets a sub-object of the idx-th 0xB0-byte entry and stores a scale (1.0 when the value is zero) with the raw value. */
extern void func_089A3868(void *dst, int src);
typedef struct { char pad0[0x50]; char sub[0x10]; char pad1[0x88 - 0x50 - 0x10]; float scale; float raw; } Ent;
void func_08B3BD9C(float v, char *base, int idx, int src) {
    Ent *e = (Ent *)(base + idx * 0xB0 + 0x110);
    float t;
    func_089A3868(e->sub, src);
    t = v;
    if (v == 0.0f) {
        t = 1.0f;
    }
    e->scale = t;
    e->raw = v;
}
