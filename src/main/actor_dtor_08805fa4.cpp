/* Deleting destructor of an actor kept on the global actor list: unlinks it
   from the list, then runs the base destructor. */
struct Actor08805FA4 {
    char pad_000[0x544];
    void *vtable;
    char pad_548[0xB54 - 0x548];
    Actor08805FA4 *next;
};

struct ActorList08805FA4 {
    Actor08805FA4 *head;
    Actor08805FA4 *tail;

    static void remove(ActorList08805FA4 *list, Actor08805FA4 *actor)
    {
        if (list->head != 0 && list->tail != 0) {
            Actor08805FA4 *next = actor->next;
            Actor08805FA4 *prev;
            if (actor == list->head) {
                list->head = next;
                prev = 0;
            } else {
                prev = list->head;
                while (prev->next != actor) {
                    prev = prev->next;
                }
                prev->next = next;
            }
            if (actor == list->tail) {
                list->tail = prev;
            }
            actor->next = 0;
        }
    }
};

struct World08805FA4 {
    char pad_00[0x1C];
    ActorList08805FA4 actors;
};

extern "C" {

extern World08805FA4 *D_089D4958;
extern int D_089C6A20;

void func_088310BC(Actor08805FA4 *self, int flags);
void func_0883133C(Actor08805FA4 *self);

void func_08805FA4(Actor08805FA4 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C6A20;
        ActorList08805FA4::remove(&D_089D4958->actors, self);
        func_088310BC(self, 0);
        if (flags & 1) {
            func_0883133C(self);
        }
    }
}

}
