typedef struct BattleGlobalRoot {
    unsigned char padding_000[0x7A4];
    unsigned char *field_7A4;
} BattleGlobalRoot;

extern BattleGlobalRoot *D_089D6A10;
extern unsigned char *func_089DD8D4(unsigned char *);
extern void func_089E09C4(unsigned char *, short);

void func_089E0974(void *self, short value)
{
    unsigned char *root = D_089D6A10->field_7A4;

    if (!root) return;
    func_089E09C4(func_089DD8D4(root), value);
}
