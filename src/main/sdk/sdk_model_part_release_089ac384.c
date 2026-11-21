/* Model library: release one model part's arrays (its 0x24-byte items through func_089AC3DC, then
 * three blocks of heap 1); returns the part. */
typedef struct D012ModelPartData {
    unsigned int unk_00;  /* 0x00 */
    void *items;          /* 0x04: array of 0x24-byte items */
    void *unk_08;         /* 0x08 */
    unsigned int unk_0C;  /* 0x0C */
    void *unk_10;         /* 0x10 */
    void *unk_14;         /* 0x14 */
} D012ModelPartData;

void *func_089AC3DC(void *items, int count);
void func_089ABF70(int heap, void *block);

D012ModelPartData *func_089AC384(D012ModelPartData *part)
{
    if (part != 0) {
        func_089AC3DC(part->items, 1);
        func_089ABF70(1, part->unk_08);
        func_089ABF70(1, part->unk_14);
        func_089ABF70(1, part->unk_10);
    }
    return part;
}
