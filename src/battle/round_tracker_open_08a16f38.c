/* Opens a round tracker: checks every argument and keeps them. */
struct RoundTracker {
    unsigned char open;
    unsigned char pad_01[0x67];
    unsigned int side;
    void *source;
    void *models;
    int name;
    void *names;
    void *bones;
    void *colors;
};

extern void func_08A16E50(struct RoundTracker *tracker);

int func_08A16F38(struct RoundTracker *tracker, unsigned int side, void *source, void *models, int name,
                  void *names, void *bones, void *colors)
{
    func_08A16E50(tracker);
    if (side >= 2 || source == 0 || models == 0 || name < 0 || names == 0 || bones == 0 || colors == 0) {
        return -1;
    }
    tracker->side = side;
    tracker->source = source;
    tracker->models = models;
    tracker->name = name;
    tracker->names = names;
    tracker->bones = bones;
    tracker->colors = colors;
    tracker->open = 1;
    return 0;
}
