extern "C" {
typedef struct Slots08847758 { char pairs[0x40]; unsigned char ids[16]; } Slots08847758;
struct SlotIds {
    static unsigned char &at(Slots08847758 *s, int i) { return *((unsigned char *)s + i + 0x40); }
};
extern void func_08847320(Slots08847758 *s, void *actor, int id);
void func_08847758(Slots08847758 *s, void *actor, int id)
{
    func_08847320(s, actor, id);
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
