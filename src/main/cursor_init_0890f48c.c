typedef struct Cursor {
    int index;
    int field_04;
    int target;
    unsigned char visible;
    unsigned char field_0D;
} Cursor;

void func_0890F48C(Cursor *cursor)
{
    cursor->index = -1;
    cursor->field_04 = 0;
    cursor->target = -1;
    cursor->visible = 1;
    cursor->field_0D = 0;
}
