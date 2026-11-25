/* True when the item's three links are usable and, for kind 4, one of its span entries accepts the second link. */
typedef struct Span { short count; short start; } Span;
typedef struct Item { char p0[4]; short a; short b; short c; char p1; unsigned char low:4; signed char kind:4; } Item;
typedef struct Tab { char p[0x6C]; unsigned int spans_n; Span *spans; } Tab;
typedef struct Tab2 { char p[0xB0]; unsigned int items_n; Item *items; } Tab2;
typedef struct Own { unsigned char active; char pad[3]; Tab2 *t; } Own;
extern int func_08AA7C80(Own *, int);
extern int func_08AA7AD0(Own *, int);
extern int func_08AA7D3C(Own *, int, int);
int func_08AA7F28(Own *self, unsigned int idx)
{
    Item *it;
    if (self->active == 0) {
        return 0;
    }
    if (idx < self->t->items_n) {
        it = &self->t->items[idx];
        if (func_08AA7C80(self, it->a) == 0) {
            return 0;
        } else if (func_08AA7C80(self, it->b) == 0) {
            return 0;
        } else if (func_08AA7AD0(self, it->c) == 0) {
            return 0;
        } else {
            if (it->kind == 4) {
                Tab *t = (Tab *)self->t;
                int sidx = it->a;
                if ((unsigned int)sidx < t->spans_n) {
                    int i = 0;
                    int found = 0;
                    Span *sp = &t->spans[sidx];
                    for (i = 0; i < sp->count; i++) {
                        if (func_08AA7D3C(self, sp->start + i, it->b) == 1) {
                            found = 1;
                            break;
                        }
                    }
                    if (found == 0) {
                        return 0;
                    }
                }
            }
            return 1;
        }
    }
    return 0;
}
