typedef struct EffectRow4AD90 {
    void *handles[4];
    char pad_010[0xC];
} EffectRow4AD90;

typedef union EffectSet4AD90 {
    struct {
        char pad_000[0x1C];
        EffectRow4AD90 rows[7];
    } grid;
    struct {
        char pad_000[0xD4];
        void *handles[2];
    } pair;
} EffectSet4AD90;

typedef struct EffectOwner4AD90 {
    char pad_000[0xC];
    int field_0C;
    char pad_010[0x4];
    EffectSet4AD90 *set;
} EffectOwner4AD90;

extern void func_088DA060(void *handle);

void func_08A4AD90(EffectOwner4AD90 *self)
{
    if (self->field_0C) {
        {
            int i;
            for (i = 0; i < 2; i++) {
                if (self->set->pair.handles[i]) {
                    func_088DA060(self->set->pair.handles[i]);
                    self->set->pair.handles[i] = 0;
                }
            }
        }
        {
            int i;
            for (i = 0; i < 7; i++) {
                int j;
                for (j = 0; j < 4; j++) {
                    if (self->set->grid.rows[i].handles[j]) {
                        func_088DA060(self->set->grid.rows[i].handles[j]);
                        self->set->grid.rows[i].handles[j] = 0;
                    }
                }
            }
        }
    }
}
