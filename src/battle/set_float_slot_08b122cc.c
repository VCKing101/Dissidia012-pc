/* Marks an entry active and stores a float value with a zero float beside it. */
typedef struct { char pad[0x18]; int on; float zero; float v; } Entry;
void func_08B122CC(float v, Entry *e) {
    e->on = 1;
    e->zero = 0.0f;
    e->v = v;
}
