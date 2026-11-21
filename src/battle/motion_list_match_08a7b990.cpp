/* Whether the last valid motion of the given motion list is the stage
 * object's current view motion (0 when closed, the list is missing or
 * empty, or there is no fighter or view). */
typedef struct ListEntry {
    short motion;
    short pad_02;
    int pad_04;
} ListEntry;

typedef struct MotionList {
    short pad_00;
    short count;
    int pad_04;
    ListEntry entries[1];
} MotionList;

typedef struct ListView {
    unsigned short motion;
} ListView;

typedef struct ListOwner {
    unsigned char pad_00[4];
    void *fighter;
} ListOwner;

typedef struct ListEvent {
    unsigned char open;
    unsigned char pad_01[3];
    ListOwner *owner;
} ListEvent;

extern void *D_08AB8388;

extern "C" {

extern MotionList *func_08AB809C(void *lists, int id);
extern ListView *func_08B30898(void *fighter);

int func_08A7B990(ListEvent *self, int id)
{
    MotionList *list;
    int last;
    void *fighter;
    ListView *view;
    if (!self->open) {
        return 0;
    }
    if (id < 0) {
        return 0;
    }
    list = func_08AB809C(D_08AB8388, id);
    if (list == 0) {
        return 0;
    }
    last = -1;
    for (int i = 0; i < list->count; i++) {
        ListEntry *entry = &list->entries[i];
        if (entry->motion < 0) {
            break;
        }
        last = entry->motion;
    }
    if (last < 0) {
        return 0;
    }
    fighter = self->owner->fighter;
    if (fighter == 0) {
        return 0;
    }
    view = func_08B30898(fighter);
    if (view == 0) {
        return 0;
    }
    return last == view->motion;
}

}
