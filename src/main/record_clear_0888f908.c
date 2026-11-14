typedef struct Record {
    char name[8];
    int field_08;
    int field_0C;
    char text[0x14];
    int field_24;
    char tag[5];
    char pad_2D[3];
    float x;
    float y;
    int field_38;
    int field_3C;
} Record;
extern void func_08935828(void *dst, int value, int size);

Record *func_0888F908(Record *r)
{
    r->field_08 = 0;
    r->field_0C = 0;
    r->field_24 = 0;
    r->x = 0.0f;
    r->y = -1.0f;
    r->field_38 = 0;
    r->field_3C = 0;
    func_08935828(r->name, 0, 8);
    func_08935828(r->text, 0, 0x14);
    func_08935828(r->tag, 0, 5);
    return r;
}
