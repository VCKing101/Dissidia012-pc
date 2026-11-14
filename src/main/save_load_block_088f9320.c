/* Clears the 0x200-byte block at 0xD34 and loads it from the save data. */
typedef struct SaveBlock {
    int words[0x80];
} SaveBlock;

typedef struct BlockOwner {
    char pad_000[0xD34];
    SaveBlock block;
} BlockOwner;

typedef struct SaveData {
    char pad_00000[0x1DF88];
    SaveBlock block;
} SaveData;

extern SaveData *D_089D712C;

void func_088F9320(BlockOwner *owner)
{
    SaveBlock *saved = &D_089D712C->block;
    int i;

    for (i = 0; i < 0x80; i++) {
        owner->block.words[i] = 0;
    }
    owner->block = *saved;
}
