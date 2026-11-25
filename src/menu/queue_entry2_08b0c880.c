/* Like the queue routine for 08B0C75C but reads the entry's count and out byte with func_08AF9290 and passes mode 1 to func_08B19C14. */
typedef struct Item08B0C880 { char pad[6]; short id; } Item08B0C880;
typedef struct Own08B0C880 { char pad[0x140]; char *tbl; } Own08B0C880;
typedef struct Rec08B0C880 { char pad[2]; signed char rank; char pad2[0xF]; signed char kind; } Rec08B0C880;
extern Own08B0C880 *D_08B01CF8;
extern int func_08AF9290(Own08B0C880 *g, int id, signed char *out, int a);
extern Rec08B0C880 *func_088B29D8(int id);
extern Rec08B0C880 *func_088B9004(char *tbl, int id);
extern int func_08AF8C00(Own08B0C880 *g, int id);
extern void func_08B19C14(void *self, Item08B0C880 *item, int a, int b, int c, int d, int e, int f);
void func_08B0C880(void *self, Item08B0C880 *item)
{
    signed char out[4];
    Rec08B0C880 *rec2;
    int count;
    Rec08B0C880 *rec;
    int kind;
    int flags;
    count = func_08AF9290(D_08B01CF8, item->id, out, 0);
    rec = func_088B29D8(item->id);
    rec2 = func_088B9004(D_08B01CF8->tbl + 0x13E8, item->id);
    kind = (unsigned char)rec->kind;
    flags = 0;
    if (func_08AF8C00(D_08B01CF8, item->id)) {
        flags = 0x40;
    }
    if (rec2->rank >= 0x63) {
        flags = (flags | 0x400) & 0xFFFF;
    }
    func_08B19C14(self, item, 1, count, (signed char)kind, flags, (unsigned char)rec2->rank, out[0]);
}
