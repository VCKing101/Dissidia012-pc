/* Effect list queries: true when any effect of the list is busy, visible
 * or selected, or answers yes for a side (optionally only the keyed one). */
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
    virtual int is_busy();
    virtual int is_visible();
    virtual int is_selected();
    virtual int is_side(unsigned int side);
    virtual int is_done(unsigned int side);
    virtual int is_ready(unsigned int side);
    virtual int is_waiting(unsigned int side);
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

int func_08A03EA0(EffectList *list)
{
    if (!list->open) {
        return 0;
    }
    int result = 0;
    for (int i = 0; i < 20; i++) {
        ListEntry *entries = list->entries;
        ListEntry *entry = &entries[i];
        if (entry->effect == 0) {
            continue;
        }
        if (entry->effect->is_busy() == 0) {
            continue;
        }
        result = 1;
        break;
    }
    return result;
}

int func_08A03F54(EffectList *list)
{
    if (!list->open) {
        return 0;
    }
    int result = 0;
    for (int i = 0; i < 20; i++) {
        ListEntry *entries = list->entries;
        ListEntry *entry = &entries[i];
        if (entry->effect == 0) {
            continue;
        }
        if (entry->effect->is_visible() == 0) {
            continue;
        }
        result = 1;
        break;
    }
    return result;
}

int func_08A04008(EffectList *list)
{
    if (!list->open) {
        return 0;
    }
    int result = 0;
    for (int i = 0; i < 20; i++) {
        ListEntry *entries = list->entries;
        ListEntry *entry = &entries[i];
        if (entry->effect == 0) {
            continue;
        }
        if (entry->effect->is_selected() == 0) {
            continue;
        }
        result = 1;
        break;
    }
    return result;
}

int func_08A040BC(EffectList *list, unsigned int side)
{
    if (!list->open) {
        return 0;
    }
    if (side >= 2) {
        return 0;
    }
    int result = 0;
    for (int i = 0; i < 20; i++) {
        ListEntry *entries = list->entries;
        ListEntry *entry = &entries[i];
        if (entry->effect == 0) {
            continue;
        }
        if (entry->effect->is_side(side) == 0) {
            continue;
        }
        result = 1;
        break;
    }
    return result;
}

int func_08A04194(EffectList *list, unsigned int side, int key)
{
    if (!list->open) {
        return 0;
    }
    if (side >= 2 || key < 0) {
        return 0;
    }
    int result = 0;
    for (int i = 0; i < 20; i++) {
        ListEntry *entries = list->entries;
        ListEntry *entry = &entries[i];
        if (entry->effect == 0) {
            continue;
        }
        if (entry->key != key) {
            continue;
        }
        if (entry->effect->is_done(side) == 0) {
            continue;
        }
        result = 1;
        break;
    }
    return result;
}

int func_08A0429C(EffectList *list, unsigned int side)
{
    if (!list->open) {
        return 0;
    }
    if (side >= 2) {
        return 0;
    }
    int result = 0;
    for (int i = 0; i < 20; i++) {
        ListEntry *entries = list->entries;
        ListEntry *entry = &entries[i];
        if (entry->effect == 0) {
            continue;
        }
        if (entry->effect->is_ready(side) == 0) {
            continue;
        }
        result = 1;
        break;
    }
    return result;
}

int func_08A04374(EffectList *list, unsigned int side)
{
    if (!list->open) {
        return 0;
    }
    if (side >= 2) {
        return 0;
    }
    int result = 0;
    for (int i = 0; i < 20; i++) {
        ListEntry *entries = list->entries;
        ListEntry *entry = &entries[i];
        if (entry->effect == 0) {
            continue;
        }
        if (entry->effect->is_waiting(side) == 0) {
            continue;
        }
        result = 1;
        break;
    }
    return result;
}

}
