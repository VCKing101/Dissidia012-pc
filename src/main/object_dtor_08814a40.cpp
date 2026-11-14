/* Deleting destructor of an object kept on the world's third list: unlinks
   it from the list, then runs the base destructor. */
struct Actor08814A40 {
    char pad_000[0x544];
    void *vtable;
    char pad_548[0x550 - 0x548];
    Actor08814A40 *next;
};

struct ActorList08814A40 {
    Actor08814A40 *head;
    Actor08814A40 *tail;

    static void remove(ActorList08814A40 *list, Actor08814A40 *actor)
    {
        if (list->head != 0 && list->tail != 0) {
            Actor08814A40 *next = actor->next;
            Actor08814A40 *prev;
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

struct World08814A40 {
    char pad_00[0x34];
    ActorList08814A40 actors;
};

extern "C" {

extern World08814A40 *D_089D4958;
extern int D_089C7030;

void func_088310BC(Actor08814A40 *self, int flags);
void func_0883133C(Actor08814A40 *self);

void func_08814A40(Actor08814A40 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C7030;
        ActorList08814A40::remove(&D_089D4958->actors, self);
        func_088310BC(self, 0);
        if (flags & 1) {
            func_0883133C(self);
        }
    }
}

}
