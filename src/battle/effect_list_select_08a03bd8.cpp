/* Selects the keyed effect of the list and clears the waiting flag. */
class ListEffect {
public:
    virtual void slot_008();
    virtual void slot_010();
    virtual void slot_018();
    virtual void slot_020();
    virtual void stop();
    virtual void step(float scale, float time);
    virtual void pause();
    virtual void slot_040();
    virtual void select();
    virtual void resume();
    virtual void show();
    virtual void hide();
    virtual void set_mode(int mode);
    virtual int is_finished();
};

struct ListEntry {
    int key;
    ListEffect *effect;
};

struct FrameClock {
    unsigned char pad_00[0x18];
    float frame_scale;
};

struct EffectList {
    unsigned char open;
    unsigned char pad_01[3];
    void *owner;
    ListEntry entries[20];
    int waiting;
};

extern "C" {

extern void func_08891920(void *owner, int value);

int func_08A03BD8(EffectList *list, int key)
{
    if (!list->open) {
        return -1;
    }
    func_08891920(list->owner, 1);
    ListEffect *found = 0;
    for (int i = 0; i < 20; i++) {
        ListEntry *entry = &list->entries[i];
        if (entry->effect == 0) {
            continue;
        }
        if (entry->key != key) {
            continue;
        }
        found = entry->effect;
        break;
    }
    if (found != 0) {
        found->select();
    }
    list->waiting = 0;
    return 0;
}

}
