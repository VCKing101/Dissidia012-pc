extern "C" {
typedef struct SlotPair08847280 { unsigned char id; unsigned char value; } SlotPair08847280;
typedef struct Slots08847280 { SlotPair08847280 pairs[16]; } Slots08847280;
typedef struct Actor08847280 { char pad_000[0x394]; void *body; } Actor08847280;
extern unsigned char func_0886CC38(void *body);
void func_08847280(Slots08847280 *s, Actor08847280 *a, int id)
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
