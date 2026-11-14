/* Inserts an actor into the world's update list, keeping the list sorted
   by update priority. */
struct Actor08833598 {
    char pad_000[0x4A0];
    Actor08833598 *next;
    char pad_4A4[0x542 - 0x4A4];
    unsigned char priority;
};

struct ActorList08833598 {
    Actor08833598 *head;
    Actor08833598 *tail;

    static Actor08833598 *&next_of(Actor08833598 *p) { return p->next; }
    static Actor08833598 *next(ActorList08833598 *l, Actor08833598 *p)
    {
        if (p == 0) {
            return l->head;
        }
        return p->next;
    }
    void push_front(Actor08833598 *a)
    {
        if (this->head == 0) {
            this->tail = a;
            this->head = a;
            next_of(a) = 0;
        } else {
            next_of(a) = this->head;
            this->head = a;
        }
    }
    void push_back(Actor08833598 *a)
    {
        if (this->tail == 0) {
            this->tail = a;
            this->head = a;
            next_of(a) = 0;
        } else {
            next_of(this->tail) = a;
            next_of(a) = 0;
            this->tail = a;
        }
    }
    void insert_after(Actor08833598 *prev, Actor08833598 *a)
    {
        if (prev == 0) {
            push_front(a);
        } else if (prev == this->tail) {
            push_back(a);
        } else {
            next_of(a) = next_of(prev);
            next_of(prev) = a;
        }
    }
};

struct World08833598 {
    char pad_00[0xC];
    ActorList08833598 actors;
};

extern "C" {

extern World08833598 *D_089D4958;

void func_08833598(Actor08833598 *self)
{
    Actor08833598 *prev = 0;
    Actor08833598 *cur = 0;
    while ((cur = ActorList08833598::next(&D_089D4958->actors, cur)) != 0) {
        if (cur->priority >= self->priority) {
            break;
        }
        prev = cur;
    }
    D_089D4958->actors.insert_after(prev, self);
}

}
