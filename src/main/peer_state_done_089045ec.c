/* Moves state 1 to done; always returns 1. */
typedef struct PeerList {
    char pad_000[0x114];
    unsigned char done;
    unsigned char state;
} PeerList;

int func_089045EC(PeerList *list)
{
    int result = 1;

    if (list->state == 1) {
        list->done = result;
        list->state = 0;
    }
    return result;
}
