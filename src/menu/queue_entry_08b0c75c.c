/* Looks up the entry id (halfword +6 of the item) in the owner's tables, builds its flag set (0x40 when func_08AF8B78 says so, bits from func_08AF9064, 0x400 when its rank byte is 99 or more) and passes it on with func_08B19C14. */
typedef struct Item08B0C75C { char pad[6]; short id; } Item08B0C75C;
typedef struct Own08B0C75C { char pad[0x140]; char *tbl; } Own08B0C75C;
typedef struct Rec08B0C75C { char pad[2]; signed char rank; char pad2[9]; signed char kind; } Rec08B0C75C;
extern Own08B0C75C *D_08B01CF8;
extern int func_08AF90BC(Own08B0C75C *g, unsigned short id, int a);
extern Rec08B0C75C *func_088B8FD0(char *tbl, int id);
extern Rec08B0C75C *func_088C0F8C(int id);
extern int func_08B09D2C(void *self, int kind);
extern int func_08AF8B78(Own08B0C75C *g, unsigned short id);
extern int func_08AF9064(Own08B0C75C *g, unsigned short id, int a);
extern void func_08B19C14(void *self, Item08B0C75C *item, int a, int b, int c, int d, int e, int f);
void func_08B0C75C(void *self, Item08B0C75C *item)
{
    int count = func_08AF90BC(D_08B01CF8, item->id, 0);
    int has = count != 0;
    Rec08B0C75C *rec = func_088B8FD0(D_08B01CF8->tbl + 0x13E8, item->id);
    int flags;
    int ok;
    int rank;
    ok = (unsigned char)func_08B09D2C(self, func_088C0F8C(item->id)->kind);
    flags = 0;
    if (func_08AF8B78(D_08B01CF8, item->id)) {
        flags = 0x40;
    }
    flags = (flags | func_08AF9064(D_08B01CF8, item->id, 1)) & 0xFFFF;
    rank = rec->rank;
    if (rank >= 0x63) {
        flags = (flags | 0x400) & 0xFFFF;
    }
    func_08B19C14(self, item, 0, count, (signed char)ok, flags, (unsigned char)rank, has);
}
