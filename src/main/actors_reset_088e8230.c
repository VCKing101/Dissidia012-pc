/* Clears the actor list; unless `keep`, also the eight (actor, state) pairs. */
typedef struct ActorPair {
    void *actor;
    int state;
} ActorPair;

typedef struct ActorList {
    void *current;
    ActorPair pairs[8];
    int count;
    unsigned char dirty;
} ActorList;

void func_088E8230(ActorList *list, unsigned char keep)
{
    int i;

    list->current = 0;
    if (!keep) {
        for (i = 0; i < 8; i++) {
            ActorPair *pair = &list->pairs[i];

            pair->actor = 0;
            pair->state = 0;
        }
    }
    list->dirty = 0;
    list->count = 0;
}
