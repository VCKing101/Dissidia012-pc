/* Make a text item: take one from the stock, reset it, build it from a string (colour code 0x1C or plain text), append it to the manager list or give it back on failure. */
struct Slot0887D868 {
    void *object;
    Slot0887D868 *next;
};
struct Item0887D868 {
    unsigned char unk_00[0x48];
    Item0887D868 *next;
    unsigned char unk_4C[4];
    Item0887D868 *prev;
};
struct Mgr0887D868 {
    unsigned char unk_0000[0x30];
    unsigned char sub[0x168 - 0x30];
    Item0887D868 *head;
    Item0887D868 *tail;
    int count;
    unsigned char unk_0174[0x6E40 - 0x174];
    Slot0887D868 *take;
    Slot0887D868 *give;
    unsigned char unk_6E48[0xF2C0 - 0x6E48];
    void *colors;

    Item0887D868 *take_one() {
        Item0887D868 *item = (Item0887D868 *)take->object;
        take->object = 0;
        take = take->next;
        return item;
    }
    void push_back(Item0887D868 *item) {
        Item0887D868 **link;
        if (tail == 0) {
            tail = item;
            head = item;
            item->next = 0;
            item->prev = 0;
        } else {
            link = &tail->next;
            *link = item;
            Item0887D868 *prev = tail;
            item->next = 0;
            item->prev = prev;
            tail = item;
        }
        count++;
    }
    void give_back(Item0887D868 *item) {
        Slot0887D868 *slot;
        Slot0887D868 *ret = give;
        for (slot = take; slot->object != 0; slot = slot->next) {
        }
        ret->object = item;
        give = give->next;
    }
};
extern "C" {
void func_0887B6A0(Item0887D868 *item);
void func_0887B820(Item0887D868 *item, int a);
int func_0887E9CC(void *colors, void *sub, Item0887D868 *item, const unsigned short *text, int a, int b);
int func_0887F294(Mgr0887D868 *m, Item0887D868 *item, int slot, const unsigned short *text, int a, int b, int c, unsigned char d);
void func_0887DAC0(Mgr0887D868 *m, Item0887D868 *item);

Item0887D868 *func_0887D868(Mgr0887D868 *m, int slot, const unsigned short *text, int a3, int a4, int a5, int a6, unsigned char a7)
{
    Item0887D868 *item = m->take_one();
    int r;
    func_0887B6A0(item);
    func_0887B820(item, a3);
    if (*text != 0x1C) {
        r = func_0887F294(m, item, slot, text, a4, a5, a6, a7);
    } else {
        r = func_0887E9CC(m->colors, m->sub, item, text + 1, a5, a6);
    }
    if (r != 0) {
        m->push_back(item);
    } else {
        func_0887DAC0(m, item);
        m->give_back(item);
        item = 0;
    }
    return item;
}
}
