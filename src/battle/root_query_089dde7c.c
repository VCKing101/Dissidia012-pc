typedef struct BattleRootOwner {
    int field_00;
    unsigned char *field_04;
} BattleRootOwner;

extern int func_089DD7A8(unsigned char *);

void func_089DDE7C(BattleRootOwner *owner)
{
    unsigned char *root;

    if (owner == 0) {
        return;
    }
    root = owner->field_04;
    if (root == 0) {
        return;
    }
    if (func_089DD7A8(root) != 0) return;
}
