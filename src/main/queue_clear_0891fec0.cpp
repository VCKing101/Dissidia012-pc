/* Clears a queue of 24 entries. */
extern "C" {

extern void func_089226F8(void *entry);

typedef struct QueueEntry { char data[0x34]; } QueueEntry;

typedef struct EntryQueue {
    int count;
    int field_04;
    QueueEntry entries[24];     /* 0x08 */
} EntryQueue;

void func_0891FEC0(EntryQueue *q)
{
    q->count = 0;
    for (int i = 0; i < 24; i++) {
        func_089226F8(&q->entries[i]);
    }
}

}
