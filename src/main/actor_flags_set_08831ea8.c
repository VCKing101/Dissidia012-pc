typedef struct ActorFlags { char pad_000[0x388]; unsigned long long flags; } ActorFlags;
extern unsigned long long D_089C7F18;
extern unsigned long long D_089C7F20;
void func_08831EA8(ActorFlags *a, int unused, unsigned char extra)
{
    a->flags |= D_089C7F18;
    if (extra == 1) {
        a->flags |= D_089C7F20;
    }
}
