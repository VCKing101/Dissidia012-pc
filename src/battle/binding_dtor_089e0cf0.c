/* Destructor of the 0x88 binding subobject of the 0x1C6C slot owner. */
typedef struct BattleBinding {
    unsigned char field_00;
    unsigned char padding_01[3];
    void *field_04;
    void *vtable;
} BattleBinding;

extern unsigned char D_089E1060[];
extern void func_089E0D54(BattleBinding *);
extern void func_08992F04(void *);

void func_089E0CF0(BattleBinding *binding, int flags)
{
    if (binding) {
        binding->vtable = D_089E1060;
        func_089E0D54(binding);
        if (flags & 1) func_08992F04(binding);
    }
}
