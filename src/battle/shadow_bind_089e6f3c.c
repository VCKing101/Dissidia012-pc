/* Binds a model to the small 0x08-byte holder and queries it. */
typedef struct BattleShadowModel {
    unsigned char padding_00[0xC];
    short field_0C;
    short field_0E;
} BattleShadowModel;

typedef struct BattleShadowHolder {
    unsigned char field_00;
    unsigned char padding_01[3];
    BattleShadowModel *field_04;
} BattleShadowHolder;

typedef struct BattleShadowSpec {
    int field_00;
    int field_04;
} BattleShadowSpec;

extern BattleShadowSpec D_089E70E8;
extern void func_089E6EFC(BattleShadowHolder *);
extern BattleShadowModel *func_088DAA40(void *, int, int);

int func_089E6F3C(BattleShadowHolder *holder, void *source)
{
    func_089E6EFC(holder);
    if (!source) return -1;
    holder->field_04 = func_088DAA40(source, D_089E70E8.field_00, 0);
    if (!holder->field_04) return -2;
    holder->field_04->field_0E = D_089E70E8.field_04;
    holder->field_00 = 1;
    return 0;
}

int func_089E6FBC(BattleShadowHolder *holder)
{
    if (!holder->field_00) return -1;
    return 0;
}

int func_089E6FDC(BattleShadowHolder *holder)
{
    if (!holder->field_00) return 0;
    if (!holder->field_04) return 0;
    return holder->field_04->field_0C == 0x1D;
}
