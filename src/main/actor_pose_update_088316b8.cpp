extern "C" {
typedef struct Actor088316B8 {
    char pad_000[0x164];
    char pose[0x330 - 0x164];
    char motion[0x388 - 0x330];
    unsigned long long flags;   /* 0x388 */
    char pad_390[4];
    void *body;                 /* 0x394 */
} Actor088316B8;
extern unsigned long long D_089C7F00;
extern int func_08837990(Actor088316B8 *a);
extern void func_0886ACCC(void *body, void *motion, void *pose);
struct ActorFlagOps {
    static bool has(Actor088316B8 *a) { return (a->flags & D_089C7F00) != 0; }
};
void func_088316B8(Actor088316B8 *a)
{
    if (func_08837990(a) != 0) {
        return;
    }
    if (a->body == 0) {
        return;
    }
    if (ActorFlagOps::has(a)) {
        return;
    }
    func_0886ACCC(a->body, a->motion, a->pose);
}
}
