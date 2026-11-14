/* Clears the HUD record and its 0x40-byte buffer, keeping `owner`. */
typedef struct HudRecord {
    int field_00;
    char buffer[0x40];
    float time;
    float alpha;
    void *owner;
    unsigned char shown;
    unsigned char active;
} HudRecord;

extern void func_08935828(void *dst, int value, int size);

void func_088EA310(HudRecord *hud, void *owner)
{
    hud->field_00 = 0;
    func_08935828(hud->buffer, 0x40, 0);
    hud->time = 0.0f;
    hud->owner = owner;
    hud->alpha = 0.0f;
    hud->shown = 0;
    hud->active = 0;
}
