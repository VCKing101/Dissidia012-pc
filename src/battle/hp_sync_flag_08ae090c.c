// Syncs the owner's stored HP, refreshes it, and flags whether any of the target's first four attachments has kind 0x1F.
struct Stats { char pad[0x14]; short hp; };
struct Core { char pad[0x52C]; struct Stats *stats; };
struct Att { int pad; int kind; };
struct Target { char pad[0x30]; char list[1]; };
struct Self {
    char pad0[0x10]; struct Core *core; struct Target *target; int pad18; int hp;
    char pad1[0x30 - 0x20]; char sub[0x29A - 0x30]; char flag;
};
extern void func_08AE3710(float f, struct Self *self, int a, int b, int c, int d);
extern void func_08AE9710(void *sub);
extern int func_08AE9D90(void *list, struct Att **out, int max);

void func_08AE090C(struct Self *self)
{
    struct Att *found[4];
    int n;
    int i;
    int hp = self->core->stats->hp;
    if (hp != self->hp) {
        int diff = hp - self->hp;
        func_08AE3710(0.0f, self, 6, diff, diff, 0);
    }
    func_08AE9710(self->sub);
    n = func_08AE9D90(self->target->list, found, 4);
    self->flag = 0;
    i = 0;
    if (i < n) {
        do {
            if (found[i]->kind == 0x1F) {
                self->flag = 1;
                break;
            }
            i++;
        } while (i < n);
    }
}
