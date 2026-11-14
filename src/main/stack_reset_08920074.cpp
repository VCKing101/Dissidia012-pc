/* Resets an id stack: six ids to -1, thirty-two values to zero, then the
   trailing counters. The first loop is C++-scoped (cursor built from the
   field), the second a plain C loop (base cursor, offset in the store). */
extern "C" {

typedef struct IdStack {
    int count;                  /* 0x00 */
    int ids[6];                 /* 0x04 */
    int values[32];             /* 0x1C */
    int field_9C;
    int field_A0;
    short field_A4;
    short field_A6;
    short field_A8;
    short field_AA;
    short field_AC;
    short field_AE;
    unsigned char field_B0;
} IdStack;

void func_08920074(IdStack *s)
{
    s->count = 0;
    for (int i = 0; i < 6; i++) {
        int *id = &s->ids[i];
        *id = -1;
    }
    int i;
    for (i = 0; i < 32; i++) {
        s->values[i] = 0;
    }
    s->field_9C = 0;
    s->field_A0 = 0;
    s->field_A4 = 0;
    s->field_A6 = 0;
    s->field_A8 = 0;
    s->field_AA = 0;
    s->field_AC = 0;
    s->field_B0 = 0;
}

}
