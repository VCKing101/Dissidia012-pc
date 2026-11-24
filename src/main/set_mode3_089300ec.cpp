/* Sets the word at 0x28 to 3 when the sub-object at 0x30 is missing or has bit 5 of its halfword at 0x20 set. */
struct Sub089300EC {
    char pad[0x20];
    unsigned short flags;
    static bool test(Sub089300EC *s) { return (s->flags & 0x20) != 0; }
};
struct Obj089300EC {
    char pad[0x28];
    int mode;
    char pad2[4];
    Sub089300EC *sub;
};
extern "C" void func_089300EC(Obj089300EC *self) {
    Sub089300EC *s = self->sub;
    if (s) { if (!Sub089300EC::test(s)) { return; } }
    self->mode = 3;
}
