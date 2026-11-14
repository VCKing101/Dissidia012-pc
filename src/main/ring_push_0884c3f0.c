typedef struct RingEntry { char data[0x40]; } RingEntry;
typedef struct Ring0884C3F0 { RingEntry entries[150]; int count; int tail; int head; } Ring0884C3F0;
extern void func_0884C4A8(Ring0884C3F0 *r, RingEntry *e);
RingEntry *func_0884C3F0(Ring0884C3F0 *r)
{
    int index = r->head++;
    RingEntry *e;
    r->head = r->head % 150;
    e = &r->entries[index];
    if (r->count < 150) {
        r->count++;
    } else {
        func_0884C4A8(r, e);
        r->tail = r->head;
    }
    return e;
}
