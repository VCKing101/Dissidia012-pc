/* Resets a cursor record. */
typedef struct Cursor0884ECD4 {
    int field_00;
    int field_04;
    int field_08;
    float field_0C;
    short index;
    short field_12;
    short field_14;
    short field_16;
    unsigned char visible;
    unsigned char field_19;
    unsigned char field_1A;
    unsigned char field_1B;
    unsigned char field_1C;
    unsigned char field_1D;
} Cursor0884ECD4;

void func_0884ECD4(Cursor0884ECD4 *c)
{
    c->field_00 = 0;
    c->field_04 = 0;
    c->field_08 = 0;
    c->field_0C = 0.0f;
    c->index = -1;
    c->field_12 = 0;
    c->visible = 1;
    c->field_14 = 0;
    c->field_16 = 0;
    c->field_19 = 0;
    c->field_1A = 0;
    c->field_1B = 0;
    c->field_1C = 0;
    c->field_1D = 0;
}
