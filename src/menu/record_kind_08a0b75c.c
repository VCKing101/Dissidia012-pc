/* Looks up the record of the object's id and returns its kind's value
   (func_088C09FC), or 0 when there is no record. */
typedef struct Record29 {
    char pad_00[0x1];
    unsigned char kind;
} Record29;

typedef struct Holder29 {
    char pad_00[0x4];
    void *object;
} Holder29;

extern int func_089FE58C(void *object);
extern Record29 *func_088B70C8(int id);
extern int func_088C09FC(unsigned char kind);

int func_08A0B75C(Holder29 *self)
{
    Record29 *record = func_088B70C8(func_089FE58C(self->object));
    int result = 0;

    if (record != 0) {
        result = func_088C09FC(record->kind);
    }
    return result;
}
