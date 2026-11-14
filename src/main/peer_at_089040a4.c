/* Bounds-checked peer entry (6 bytes each, at 0x124). */
typedef struct PeerEntry {
    unsigned char mac[6];
} PeerEntry;

typedef struct PeerList {
    char pad_000[0x124];
    PeerEntry peers[16];
    int count;
} PeerList;

PeerEntry *func_089040A4(PeerList *list, int index)
{
    PeerEntry *result = 0;

    if (index >= 0 && index < list->count) {
        PeerEntry *peers = list->peers;

        result = &peers[index];
    }
    return result;
}
