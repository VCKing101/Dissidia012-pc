typedef struct Note { char data_00[0x18]; int arg; char data_1C[4]; unsigned char kind; } Note;
typedef struct Board { char data_0000[0xBDA8]; int nextId; } Board;
extern int func_088C83FC(Board *b);
extern Note *func_088C9044(Board *b, int id);

int func_088C8F64(Board *b, int arg, unsigned int kind)
{
    Note *note;
    int id;
    if (kind >= 3) {
        return 0;
    }
    if (func_088C83FC(b) <= 0) {
        return 0;
    }
    id = b->nextId;
    b->nextId = id + 1;
    note = func_088C9044(b, id);
    if (note == 0) {
        return 0;
    }
    note->arg = arg;
    note->kind = kind;
    return 1;
}
