typedef struct Cue {
    unsigned char active;   /* 0x00 */
    char pad01[3];
    int field_04;
    int field_08;
    int field_0C;
    unsigned char field_10;
    char pad11[3];
    int field_14;
    unsigned char field_18;
    char pad19[3];
    int field_1C;
    float field_20;
    float field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
} Cue;

void func_08B1D938(Cue *cue)
{
    cue->active = 0;
    cue->field_04 = -1;
    cue->field_08 = 0;
    cue->field_0C = 0;
    cue->field_10 = 0;
    cue->field_14 = -1;
    cue->field_18 = 0;
    cue->field_1C = -1;
    cue->field_20 = 0.0f;
    cue->field_24 = 0.0f;
    cue->field_28 = 0;
    cue->field_2C = 0;
    cue->field_30 = 0;
    cue->field_34 = 0;
    cue->field_38 = 0;
}
