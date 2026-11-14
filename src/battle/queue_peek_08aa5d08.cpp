/* Queue readers guarded by func_08AA5CB8 (returns 1 while the queue is busy):
   the current item, the item after it, and the number of items left.
   Retail keeps the queue in the frame (0($sp)) across the guard call: a local
   object with a constructor (Kept) is never promoted by SNC -O2, and copying
   it back right after the call puts the reload where the spill reload is. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Queue08AA5D08 {
    unsigned char active;
    char unknown_01[0x3B];
    int *items;
    int count;
    int index;
};

extern "C" {

int func_08AA5CB8(Queue08AA5D08 *queue);

int func_08AA5D08(Queue08AA5D08 *queue)
{
    if (queue->active == 0) {
        return -1;
    }
    Kept<Queue08AA5D08 *> saved(queue);
    int busy = func_08AA5CB8(queue);
    queue = saved.value;
    if (busy == 1) {
        return -2;
    }
    return queue->items[queue->index];
}

int func_08AA5D60(Queue08AA5D08 *queue)
{
    if (queue->active == 0) {
        return -1;
    }
    Kept<Queue08AA5D08 *> saved(queue);
    int busy = func_08AA5CB8(queue);
    queue = saved.value;
    if (busy == 1) {
        return -2;
    }
    return (&queue->items[queue->index])[1];
}

int func_08AA5DB8(Queue08AA5D08 *queue)
{
    if (queue->active == 0) {
        return 0;
    }
    Kept<Queue08AA5D08 *> saved(queue);
    int busy = func_08AA5CB8(queue);
    queue = saved.value;
    if (busy == 1) {
        return 0;
    }
    return queue->count - queue->index;
}

}
