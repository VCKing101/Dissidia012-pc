/* Deleting destructor of a summon kept on the global summon list: shuts it
   down, unlinks it from the list, then runs the base destructor. */
struct Actor088279F0 {
    char pad_000[0x544];
    void *vtable;
    char pad_548[0x59C - 0x548];
    Actor088279F0 *next;
};

struct ActorList088279F0 {
    Actor088279F0 *head;
    Actor088279F0 *tail;

    static void remove(ActorList088279F0 *list, Actor088279F0 *actor)
    {
        if (list->head != 0 && list->tail != 0) {
            Actor088279F0 *next = actor->next;
            Actor088279F0 *prev;
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

struct World088279F0 {
    char pad_00[0x2C];
    ActorList088279F0 actors;
};

extern "C" {

extern World088279F0 *D_089D4958;
extern int D_089C76A8;

void func_08827704(Actor088279F0 *self);
void func_088310BC(Actor088279F0 *self, int flags);
void func_0883133C(Actor088279F0 *self);

void func_088279F0(Actor088279F0 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C76A8;
        func_08827704(self);
        ActorList088279F0::remove(&D_089D4958->actors, self);
        func_088310BC(self, 0);
        if (flags & 1) {
            func_0883133C(self);
        }
    }
}

}
