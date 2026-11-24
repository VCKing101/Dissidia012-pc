/* Returns the byte for slot 0..3 (offsets 7..0xA of the block at D_089D712C + 0x1DE84); 1 for any other slot. */
typedef struct Blk088f1288 { unsigned char pad[7]; unsigned char v[4]; } Blk088f1288;
extern char *D_089D712C;
unsigned char func_088F1288(int slot)
{
    Blk088f1288 *b = (Blk088f1288 *)(D_089D712C + 0x1DE84);
    switch (slot) {
    case 0: return b->v[0];
    case 1: return b->v[1];
    case 2: return b->v[2];
    case 3: return b->v[3];
    default: return 1;
    }
}
