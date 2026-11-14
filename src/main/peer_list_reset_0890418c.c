/* Resets the peer list. */
typedef struct PeerList {
    int id;
    int field_04;
    char buffer[0x100];
    int field_108;
    int field_10C;
    int field_110;
    unsigned char active;
    unsigned char state;
    char pad_116[0x11C - 0x116];
    unsigned char ready;
    unsigned char mac[6];
    char pad_123[0x184 - 0x123];
    int count;
    int field_188;
} PeerList;

extern void func_08905848(unsigned char *mac);
extern void func_08935828(void *dst, int value, int size);

void func_0890418C(PeerList *list)
{
    list->id = -1;
    list->field_04 = 0;
    list->field_108 = 0;
    list->field_10C = 0;
    list->field_110 = 0;
    list->active = 0;
    list->state = 0;
    list->ready = 0;
    func_08905848(list->mac);
    list->count = 0;
    list->field_188 = 0;
    func_08935828(list->buffer, 0, 0x100);
}
