/* Screen0B6A0 item tables: the text ids of an item's rank (+0x11) and
   type (+0x0E), each from a table of id pairs; out-of-range values get
   the first entry. */
typedef struct Item29 {
    char pad_00[0x4];
    unsigned int flags;
    char pad_08[0x6];
    unsigned char type;   /* +0x0E */
    char pad_0F[0x1];
    unsigned char kind;   /* +0x10 */
    unsigned char rank;   /* +0x11 */
    char pad_12[0x6];
} Item29;

typedef struct TextId29 {
    unsigned short id;
    unsigned short field_02;
} TextId29;

extern const TextId29 D_08A0B194[];
extern const TextId29 D_08A0B1A8[];

unsigned short func_089EF300(void *self, Item29 *items, int index)
{
    unsigned short id = D_08A0B194[0].id;
    unsigned int rank = items[index].rank;

    if (rank < 5) {
        id = D_08A0B194[rank].id;
    }
    return id;
}

unsigned short func_089EF340(void *self, Item29 *items, int index)
{
    unsigned short id = D_08A0B1A8[0].id;
    unsigned int type = items[index].type;

    if (type < 0x25) {
        id = D_08A0B1A8[type].id;
    }
    return id;
}
