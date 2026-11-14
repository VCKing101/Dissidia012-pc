#include "menu/agent29_types.h"

/* MenuRoot: loads the root's profile view (+0xD9C of its work) from the
   save (profile +0x1FDD8, records, and the tables at +0x39F38, +0x3E758
   and +0x3E0B0), shows it, marks the options (flag 2), takes the two
   chosen items and sets the list flag from the profile's bit 4. */
typedef struct Profile29 {
    char pad_00[0xF0];
    short field_F0;
    short field_F2;
} Profile29;

typedef struct ProfileView29 {
    char pad_00[0x27];
    unsigned char flags;

    static bool Marked(ProfileView29 *view) { return (view->flags & 4) != 0; }
} ProfileView29;

extern "C" {
void func_088F58C4(ProfileView29 *view, Profile29 *profile, void *records, void *table1, void *table2, void *table3);
void func_08A21530(MenuRoot *self, ProfileView29 *view, int arg2, int arg3, int arg4, int arg5, int count, int arg7,
                   int arg8);
}

extern "C" void func_08A1BF00(MenuRoot *self)
{
    func_088F58C4((ProfileView29 *)&WORK_BYTE(self->work, 0xD9C), (Profile29 *)((char *)D_089D712C + 0x1FDD8),
                  D_089D712C->field_F58, (char *)D_089D712C + 0x39F38, (char *)D_089D712C + 0x3E758,
                  (char *)D_089D712C + 0x3E0B0);
    func_08A21530(self, (ProfileView29 *)&WORK_BYTE(self->work, 0xD9C), 0, 0, 0, 0, 0x19, -1, 0);
    WORK_U16(self->work, 0x652C) |= 2;
    Profile29 *profile = (Profile29 *)((char *)D_089D712C + 0x1FDD8);
    WORK_SHORT(self->work, 0x6604) = profile->field_F0;
    WORK_SHORT(self->work, 0x6606) = profile->field_F2;
    if (!ProfileView29::Marked((ProfileView29 *)&WORK_BYTE(self->work, 0xD9C))) {
        WORK_INT(self->work, 0x2BB8) &= ~1;
    } else {
        WORK_INT(self->work, 0x2BB8) |= 1;
    }
    if (WORK_INT(self->work, 0x2BB8) & 1) {
        WORK_U16(self->work, 0x652C) |= 0x20;
    }
}
