typedef struct Slot0886FAC0 {
    short entry;                       /* +0x00 */
    short unk_02;
    unsigned int sector;               /* +0x04 offset / 0x2000 */
    unsigned char unk_08[0x14];
} Slot0886FAC0;

typedef struct Table0886FAC0 {
    unsigned char unk_00[0x80];
    Slot0886FAC0 slots[0xFD2];         /* +0x80 */
} Table0886FAC0;

extern Table0886FAC0 D_08B63800;
extern void *D_089D5FF0;               /* the package reader */
extern int func_08871674(void *package);   /* entry count */
extern void *func_08935828(void *dst, int c, unsigned int n);   /* memset */

/* Register package entry `entry` (at a 64-bit byte offset) as file slot `index`. */
void func_0886FAC0(void *self, int index, int entry, long long offset)
{
    Slot0886FAC0 *slot;

    if (index < 0 || index >= 0xFD2) {
        return;
    }
    if (entry < 0 || func_08871674(D_089D5FF0) - 1 < entry) {
        return;
    }
    slot = &D_08B63800.slots[index];
    slot->entry = entry;
    slot->sector = offset >> 13;
    slot->unk_02 = 0;
    func_08935828(slot->unk_08, 0, sizeof(slot->unk_08));
}
