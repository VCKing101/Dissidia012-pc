/* Binds every node of a track set's two node lists to an actor's
   attachment point. */
struct Node0881F858 {
    Node0881F858 *next;
};

struct Actor0881F858 {
    char pad_00[0x30];
    char point[1];
};

struct TrackSet0881F858 {
    short active;
    char pad_02[6];
    Node0881F858 *first_a;
    char pad_0C[4];
    Node0881F858 *first_b;

    static bool valid(TrackSet0881F858 *s) { return s->active != 0; }
    static Node0881F858 *next_a(TrackSet0881F858 *s, Node0881F858 *n) { return n == 0 ? s->first_a : n->next; }
    static Node0881F858 *next_b(TrackSet0881F858 *s, Node0881F858 *n) { return n == 0 ? s->first_b : n->next; }
};

extern "C" {

void func_0881FC0C(Node0881F858 *node, char *point);
void func_0881FC30(Node0881F858 *node, char *point);

int func_0881F858(TrackSet0881F858 *self, Actor0881F858 *actor)
{
    if (!TrackSet0881F858::valid(self)) {
        return -1;
    }
    Node0881F858 *n = 0;
    char *point = actor->point;
    while ((n = TrackSet0881F858::next_a(self, n)) != 0) {
        func_0881FC0C(n, point);
    }
    return 0;
}

int func_0881F8E0(TrackSet0881F858 *self, Actor0881F858 *actor)
{
    if (!TrackSet0881F858::valid(self)) {
        return -1;
    }
    Node0881F858 *n = 0;
    char *point = actor->point;
    while ((n = TrackSet0881F858::next_b(self, n)) != 0) {
        func_0881FC30(n, point);
    }
    return 0;
}

}
