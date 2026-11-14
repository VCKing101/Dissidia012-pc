/* Polls the active battle setup root, if one exists. */
typedef struct BattleGlobalRoot {
    unsigned char padding_000[0x7A4];
    unsigned char *field_7A4;
} BattleGlobalRoot;

extern BattleGlobalRoot *D_089D6A10;
extern int func_089DD854(unsigned char *);

void func_089DD80C(void)
{
    unsigned char *root = D_089D6A10->field_7A4;

    if (root == 0) {
        return;
    }
    if (func_089DD854(root) != 0) return;
}
