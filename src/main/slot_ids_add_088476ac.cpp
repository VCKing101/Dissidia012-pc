extern "C" {
typedef struct Slots088476AC { char pairs[0x20]; unsigned char ids[16]; } Slots088476AC;
struct SlotIds {
    static unsigned char &at(Slots088476AC *s, int i) { return *((unsigned char *)s + i + 0x20); }
};
extern void func_08847280(Slots088476AC *s, void *actor, int id);
void func_088476AC(Slots088476AC *s, void *actor, int id)
{
    func_08847280(s, actor, id);
    for (int i = 0; i < 16; i++) {
        if (SlotIds::at(s, i) == id) {
            return;
        }
    }
    for (int i = 0; i < 16; i++) {
        if (SlotIds::at(s, i) == 0xFF) {
            SlotIds::at(s, i) = id;
            return;
        }
    }
}
}
