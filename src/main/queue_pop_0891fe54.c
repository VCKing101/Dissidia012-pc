/* Pops the top entry of a queue, releasing what it holds, and clears the
   queue when it becomes empty. */
extern void *func_089227E8(void *entry);
extern void func_08922F54(void *item);
extern void func_0891FEC0(void *q);

typedef struct QueueEntry { char data[0x34]; } QueueEntry;

typedef struct EntryQueue {
    int count;
    int field_04;
    QueueEntry entries[24];     /* 0x08 */
} EntryQueue;

void func_0891FE54(EntryQueue *q)
{
    void *item = func_089227E8(&q->entries[q->count]);
    if (item != 0) {
        func_08922F54(item);
    }
    q->count--;
    if (q->count <= 0) {
        func_0891FEC0(q);
    }
}
