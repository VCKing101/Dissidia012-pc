typedef struct StageLoadSlot {
    char data[0x2C];
} StageLoadSlot;

typedef struct StagePair {
    unsigned char active;   /* 0x00 */
    char pad01[3];
    int field_04;
    int field_08;
    StageLoadSlot loads[2]; /* 0x0C */
    float field_64;
    float field_68;
} StagePair;

extern void func_08B1FF28(StageLoadSlot *load);
extern void func_08B20B50(StagePair *pair);

void func_08B20B50(StagePair *pair)
{
    pair->active = 0;
    pair->field_04 = -1;
    pair->field_08 = 0;
    pair->field_64 = 0.0f;
    pair->field_68 = 0.0f;
}

void func_08B20B70(StagePair *pair)
{
    {
        int i;
        for (i = 0; i < 2; i++) {
            func_08B1FF28(&pair->loads[i]);
        }
    }
    func_08B20B50(pair);
}
