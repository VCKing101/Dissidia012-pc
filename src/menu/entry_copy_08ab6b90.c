/* Copies a 0x154-byte entry into slot `index` of the array at +0xAC.
   (Not a stack-spill function: a struct assignment.) */
typedef struct Entry08AB6B90 {
    int words[0x55];
} Entry08AB6B90;

typedef struct Owner08AB6B90 {
    char unknown_00[0xAC];
    Entry08AB6B90 entries[1];
} Owner08AB6B90;

void func_08AB6B90(Owner08AB6B90 *self, int index, Entry08AB6B90 *entry)
{
    self->entries[index] = *entry;
}
