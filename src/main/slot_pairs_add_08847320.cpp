extern "C" {
typedef struct SlotPair08847320 { unsigned char id; unsigned char value; } SlotPair08847320;
typedef struct Slots08847320 { SlotPair08847320 pairs[16]; } Slots08847320;
typedef struct Actor08847320 { char pad_000[0x394]; void *body; } Actor08847320;
extern unsigned char func_0886CC38(void *body);
void func_08847320(Slots08847320 *s, Actor08847320 *a, int id)
{
    for (int i = 0; i < 16; i++) {
        if (s->pairs[i].id == id) {
            return;
        }
    }
    for (int i = 0; i < 16; i++) {
        if (s->pairs[i].id == 0xFF) {
            s->pairs[i].id = id;
            s->pairs[i].value = func_0886CC38(a->body);
            return;
        }
    }
}
}
