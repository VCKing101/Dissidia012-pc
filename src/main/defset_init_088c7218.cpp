/* Initialises a definition set: header, the three definitions and the extra count. */
struct Def088C7218 { char data[0x3C]; };
struct Set088C7218 { char pad[0x80]; Def088C7218 defs[3]; char pad2[0x134 - 0x80 - 3 * 0x3C]; int extra; };
extern "C" {
void func_088C6FD4(Set088C7218 *set);
void func_088C6F68(Def088C7218 *def);
void func_088C7218(Set088C7218 *set)
{
    func_088C6FD4(set);
    for (int i = 0; i < 3; i++) {
        func_088C6F68(&set->defs[i]);
    }
    set->extra = 0;
}
}
