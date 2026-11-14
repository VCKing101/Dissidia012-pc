typedef struct Entry65CF4 {
    int field_00;
    int field_04;
} Entry65CF4;

unsigned char func_08A65CF4(void *self, Entry65CF4 *entry)
{
    int result;
    if (!entry) {
        return 0;
    }
    result = 0;
    if (entry->field_00 == 0 && entry->field_04 == 1) {
        result = 1;
    }
    return result;
}
