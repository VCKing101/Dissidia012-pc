/* Stores the value and the owner's flags, copies the 0x34-byte header and the 0x114-byte table into the block, and mirrors three fields of the header into the block's front. */
typedef struct Head08B30628 { int w0; unsigned char b4; char p5; unsigned short h6; char rest[0x2C]; } Head08B30628;
typedef struct Table08B30628 { int d[0x45]; } Table08B30628;
typedef struct Blk08B30628 {
    short pad0[4];
    unsigned short h8;
    unsigned char bA;
    char pB;
    int wC;
    char pad1[0x7A0 - 0x10];
    Head08B30628 head;
    Table08B30628 table;
    char pad2[0x9D0 - 0x7D4 - 0x114];
    int w9D0;
} Blk08B30628;
typedef struct S08B30628 { char pad[0x9D4]; int w9D4; Blk08B30628 *blk; } S08B30628;
void func_08B30628(S08B30628 *self, int v, Head08B30628 *head, Table08B30628 *table, int w)
{
    self->w9D4 = v;
    *(int *)((char *)self->blk + 4) = 1;
    self->blk->head = *head;
    self->blk->table = *table;
    self->blk->wC = self->blk->head.w0;
    self->blk->bA = self->blk->head.b4;
    self->blk->h8 = self->blk->head.h6;
    self->blk->w9D0 = w;
}
