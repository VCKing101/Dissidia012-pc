/* Initialises the three record tables and the card board of a world block. */
extern void func_08920A28(void *self);
extern void func_08924F94(void *entry);
extern void func_089228C4(void *entry);
extern void func_08920640(void *entry);
extern void func_0891A3E4(void *board);
typedef struct Entry8 { char data[0x8]; } Entry8;
typedef struct Entry24 { char data[0x24]; } Entry24;
typedef struct Entry14 { char data[0x14]; } Entry14;
typedef struct WorldTables {
    char head[0x10];
    Entry8 small[70];
    Entry24 medium[32];
    Entry14 large[5];
    char pad_724[0x738 - 0x724];
    char board[4];
} WorldTables;
void func_0891F0BC(WorldTables *self)
{
    int i;
    func_08920A28(self);
    for (i = 0; i < 70; i++) {
        func_08924F94(&self->small[i]);
    }
    for (i = 0; i < 32; i++) {
        func_089228C4(&self->medium[i]);
    }
    for (i = 0; i < 5; i++) {
        func_08920640(&self->large[i]);
    }
    func_0891A3E4(self->board);
}
