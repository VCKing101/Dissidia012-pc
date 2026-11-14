typedef struct MotionEntry74 {
    char data[0x74];
} MotionEntry74;

typedef struct MotionList74 {
    unsigned char active;
    int count;
    MotionEntry74 *entries;
    char pad_0C[0x10];
    char source_1C[1];
} MotionList74;

extern "C" {

extern void func_08ABC88C(MotionEntry74 *entry);
extern void func_08ABC71C(MotionEntry74 *entry, void *source);
extern void func_08ABC7D4(MotionEntry74 *entry, int arg);

/* Reset every entry and point it back at the list's source. */
int func_08ABE3F8(MotionList74 *self)
{
    for (int i = 0; i < self->count; i++) {
        func_08ABC88C(&self->entries[i]);
        func_08ABC71C(&self->entries[i], self->source_1C);
        func_08ABC7D4(&self->entries[i], 0);
    }
    return 0;
}

}
