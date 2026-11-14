/* Initialises a channel: no id, cleared state and flags. */
typedef struct Chan088A3E08 {
    int id;
    char inner[0x24];
    int field_28;
    int field_2C;
    unsigned char field_30;
    unsigned char field_31;
} Chan088A3E08;

void func_0889F63C(void *inner);

Chan088A3E08 *func_088A3E08(Chan088A3E08 *c)
{
    c->id = -1;
    func_0889F63C(c->inner);
    c->field_28 = 0;
    c->field_2C = 0;
    c->field_30 = 0;
    c->field_31 = 0;
    return c;
}
