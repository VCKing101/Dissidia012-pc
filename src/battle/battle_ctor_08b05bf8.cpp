typedef struct BattleCounter {
    int field_00;
    int field_04;
    int field_08;

    static void init(BattleCounter *counter)
    {
        counter->field_00 = 0;
        counter->field_04 = 1;
        counter->field_08 = 0;
    }
} BattleCounter;

typedef struct BattleObject {
    BattleCounter counter;
    unsigned char field_0C;
    unsigned char field_0D;
    unsigned char field_0E;
    char pad_0F[0x1];
    int field_10;
    char pad_14[0x4C];
    char field_60[0x34];
    unsigned char field_94;
    char pad_95[0x3];
    int field_98;
    char pad_9C[0x8];
    char field_A4[0x37E4];
    unsigned char field_3888;
    unsigned char field_3889;
    char pad_388A[0x2];
    char field_388C[0x24];
    const void *listener_vtable;
    struct BattleObject *listener_owner;
    int field_38B8;
} BattleObject;

extern "C" {

extern const char D_08B0CD18[];
extern void func_08B16D98(void *sub);
extern void func_08B09614(void *sub);
extern void func_08B12060(void *sub);

BattleObject *func_08B05BF8(BattleObject *self)
{
    BattleCounter::init(&self->counter);
    self->field_0C = 0;
    self->field_0D = 0;
    self->field_0E = 0;
    self->field_10 = 0;
    func_08B16D98(self->field_60);
    self->field_94 = 0;
    self->field_98 = 0;
    func_08B09614(self->field_A4);
    self->field_3888 = 0;
    self->field_3889 = 0;
    func_08B12060(self->field_388C);
    self->listener_vtable = D_08B0CD18;
    self->listener_owner = self;
    self->field_38B8 = 0;
    return self;
}

}
