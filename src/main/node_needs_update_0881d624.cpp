/* Whether a node's owner has a running, non-finished motion track. */
struct Track0881D624 {
    int pad_0;
    int looping;
    int state;
};

struct Actor0881D624 {
    char pad_000[0x46C];
    void *model;
    char pad_470[0x568 - 0x470];
    Track0881D624 *track;
};

struct Node0881D624 {
    char pad_00[0x14];
    Actor0881D624 *owner;
};

extern "C" {

bool func_0881D624(void *self, Node0881D624 *node)
{
    if (node == 0) {
        return false;
    }
    Actor0881D624 *a = node->owner;
    if (a == 0) {
        return false;
    }
    if (a->model == 0) {
        return false;
    }
    Track0881D624 *t = a->track;
    if (t == 0) {
        return false;
    }
    if (t->looping != 1) {
        return false;
    }
    if (t->state == 2) {
        return false;
    }
    return true;
}

}
