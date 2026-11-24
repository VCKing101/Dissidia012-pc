/* Stores a float and a scale (defaulting to 1.0 when zero) into the idx-th 0xB0-byte entry. */
typedef struct { char pad[0x70]; float v; char pad2[0x24]; float scale; float raw; } Ent;
void func_08B3BE84(float v, float s, char *base, int idx) {
    Ent *e = (Ent *)(base + idx * 0xB0 + 0x110);
    float t = s;
    e->v = v;
    if (s == 0.0f) {
        t = 1.0f;
    }
    e->scale = t;
    e->raw = s;
}
