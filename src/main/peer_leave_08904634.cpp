/* Forgets the stored MAC when it equals `mac`, then removes the peer. */
struct MacAddress {
    unsigned char bytes[6];
    char pad_06[2];
};

struct PeerList {
    char pad_000[0x11D];
    unsigned char mac[6];

    static unsigned char equal(int cmp)
    {
        return cmp == 0;
    }
};

extern "C" {
extern unsigned char *func_08905810(MacAddress *dst, const unsigned char *src);
extern int func_08935490(const void *a, const void *b, int size);
extern void func_08905848(unsigned char *mac);
extern void func_08904820(PeerList *list, const unsigned char *mac);

int func_08904634(PeerList *list, const unsigned char *mac)
{
    MacAddress copy;
    unsigned char *wanted = func_08905810(&copy, mac);

    if (PeerList::equal(func_08935490(list->mac, wanted, 6))) {
        func_08905848(list->mac);
    }
    func_08904820(list, mac);
    return 1;
}
}
