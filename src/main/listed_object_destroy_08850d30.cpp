struct Obj08850D30 {
    unsigned char unk_000[0x544];
    void *vtable;                      /* +0x544 */
    unsigned char unk_548[0x5D8 - 0x548];
    unsigned char member[0x18];        /* +0x5D8 */
    Obj08850D30 *next;                 /* +0x5F0 */
};

struct List08850D30 {
    Obj08850D30 *head;
    Obj08850D30 *tail;

    void remove(Obj08850D30 *item) {
        if (head != 0 && tail != 0) {
            Obj08850D30 *next = item->next;
            Obj08850D30 *prev;

            if (item == head) {
                head = next;
                prev = 0;
            } else {
                for (prev = head; prev->next != item; prev = prev->next) {
                }
                prev->next = next;
            }
            if (item == tail) {
                tail = prev;
            }
            item->next = 0;
        }
    }
};

struct World08850D30 {
    unsigned char unk_00[0x24];
    List08850D30 objects;              /* +0x24 */
};

extern "C" {
extern char D_089C8AD8[];              /* this class's vtable */
extern World08850D30 *D_089D4958;
void func_0884BD7C(void *member);      /* member destructor */
void func_088310BC(Obj08850D30 *self, int flags);   /* base destructor */
void func_0883133C(void *block);       /* operator delete */

void func_08850D30(Obj08850D30 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = D_089C8AD8;
        func_0884BD7C(self->member);
        D_089D4958->objects.remove(self);
        func_088310BC(self, 0);
        if (flags & 1) {
            func_0883133C(self);
        }
    }
}
}
