#include "main/agent33_texture.h"

/* An item of a seq object's table (0x10 bytes), linked into one of the object's lists. */
struct SeqItem {
    short unk_00;                      /* +0x00 */
    short pad_02;
    int unk_04;                        /* +0x04 */
    SeqItem *next;                     /* +0x08 */
    SeqItem *prev;                     /* +0x0C */
};

struct SeqItemList {
    SeqItem *head;
    SeqItem *tail;
    int count;

    void push_back(SeqItem *item) {
        if (tail == 0) {
            tail = item;
            head = item;
            item->next = 0;
            item->prev = 0;
        } else {
            SeqItem **link = &tail->next;
            *link = item;
            SeqItem *prev = tail;
            item->next = 0;
            item->prev = prev;
            tail = item;
        }
        count++;
    }
};

/* A layout element (0x1C bytes) with its list of items at +0x10. */
struct SeqElement {
    unsigned char unk_00[0x10];
    SeqItemList items;                 /* +0x10 */
};

struct Seq {
    unsigned char unk_00[0xC];
    SeqElement *elements;              /* +0x0C */
    unsigned char unk_10[0x54 - 0x10];
    SeqItem *items;                    /* +0x54 */
    int item_count;                    /* +0x58 */
    unsigned char unk_5C[4];
    int unk_60;                        /* +0x60 */
    unsigned short refcount;           /* +0x64 */
    unsigned short id;                 /* +0x66 */
    unsigned char group;               /* +0x68 */
};

extern "C" {

void *func_08935524(void *dst, const void *src, unsigned int n);   /* memcpy */
/* The layout data is read in four steps; each returns where the next one starts. */
void *func_088DC84C(Seq *seq, const void *data);
void *func_088DC944(Seq *seq, void *p, int a);
void *func_088DCF18(Seq *seq, void *p, int a, int b);
void *func_088DD21C(Seq *seq, void *p, int a, int b, int c);

/* seq_load: fill a seq (layout) object from a pair of pack entries: the layout data and
 * an optional table of items, each linked into the list of the element it names. */
void func_088DCBA8(Seq *seq, const void *data, const char *extra, int id, unsigned char group)
{
    seq->id = id;
    seq->group = group;
    seq->refcount = 1;
    seq->unk_60 = -1;
    const void *layout = data;
    const char *p = extra;
    if (p != 0) {
        func_08935524(&seq->item_count, p, 4);
        p += 4;
    } else {
        seq->item_count = 0;
    }
    func_088DD21C(seq, func_088DCF18(seq, func_088DC944(seq, func_088DC84C(seq, layout), 0), 0, 0), 0, 0, 0);
    if (p != 0) {
        int i;
        int offset;
        for (i = 0, offset = 0; i < seq->item_count; i++, offset += 0x10) {
            short key[1];                  /* element index (copied to the frame) */

            func_08935524(key, p, 2);
            p += 2;
            func_08935524((char *)seq->items + offset, p, 2);
            p += 2;
            func_08935524((char *)seq->items + offset + 4, p, 4);
            p += 4;
            seq->elements[key[0]].items.push_back((SeqItem *)((char *)seq->items + offset));
        }
    }
}

}
