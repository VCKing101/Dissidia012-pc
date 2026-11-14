/* Effect list: resume, show, hide and mode changes for every effect. */
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

struct EffectList {
    unsigned char open;
    unsigned char pad_01[3];
    void *owner;
    ListEntry entries[20];
    int waiting;
};

extern "C" {

int func_08A03CA4(EffectList *list)
{
    if (!list->open) {
        return -1;
    }
    for (int i = 0; i < 20; i++) {
        ListEntry *entry = &list->entries[i];
        if (entry->effect != 0) {
            entry->effect->resume();
        }
    }
    return 0;
}

int func_08A03D1C(EffectList *list)
{
    if (!list->open) {
        return -1;
    }
    for (int i = 0; i < 20; i++) {
        ListEntry *entry = &list->entries[i];
        if (entry->effect != 0) {
            entry->effect->show();
        }
    }
    list->waiting = 1;
    return 0;
}

int func_08A03DA4(EffectList *list)
{
    if (!list->open) {
        return -1;
    }
    for (int i = 0; i < 20; i++) {
        ListEntry *entry = &list->entries[i];
        if (entry->effect != 0) {
            entry->effect->hide();
        }
    }
    return 0;
}

int func_08A03E1C(EffectList *list, int mode)
{
    if (!list->open) {
        return -1;
    }
    for (int i = 0; i < 20; i++) {
        ListEntry *entry = &list->entries[i];
        if (entry->effect != 0) {
            entry->effect->set_mode(mode);
        }
    }
    return 0;
}

}
