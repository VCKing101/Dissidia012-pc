typedef struct Track { char pad_00[0x4D]; unsigned char used; char pad_4E[2]; } Track;
typedef struct Mixer { Track tracks[10]; char pad_320[0x10]; int field_330; int field_334; unsigned char field_338; char pad_339[3]; int field_33C; int field_340; float field_344; } Mixer;
void func_08858668(Mixer *self)
{
    int i;
    for (i = 0; i < 10; i++) {
        self->tracks[i].used = 0;
    }
    self->field_338 = 1;
    self->field_334 = 0;
    self->field_330 = 0;
    self->field_340 = 0;
    self->field_33C = 0;
    self->field_344 = 1.0f;
}
