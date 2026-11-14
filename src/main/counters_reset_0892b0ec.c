typedef struct Counters { int total; int first[1]; int second[7]; int third[4]; int field_34; unsigned short ids[5]; } Counters;
void func_0892B0EC(Counters *c)
{
    int i;
    c->total = 0;
    for (i = 0; i < 1; i++) {
        c->first[i] = 0;
    }
    for (i = 0; i < 7; i++) {
        c->second[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        c->third[i] = 0;
    }
    c->field_34 = 0;
    c->ids[0] = 0xFFFF;
    c->ids[1] = 0xFFFF;
    c->ids[2] = 0xFFFF;
    c->ids[3] = 0xFFFF;
    c->ids[4] = 0xFFFF;
}
