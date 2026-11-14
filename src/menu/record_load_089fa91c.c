#include "menu/agent29_types.h"

/* Screen0B6A0: loads the current record (+0x328C) into the pull page:
   its level (+0x3290), the current member's level (+0x3294, the
   record's when unset), the record's points (+0x32C8) and, unless the
   record is done, its next points and count (+0x32A0, +0x32B4). */
typedef struct Member29 {
    char pad_00[0xC5];
    unsigned char level; /* +0xC5 */
} Member29;

typedef struct Party29 {
    Member29 members[9];
    char pad_6F6[0x6D];
    signed char current; /* +0x763 */
} Party29;

typedef struct PartyRecord29 {
    char pad_00[0x1];
    unsigned char level;
    char pad_02[0xE];
    Party29 party; /* +0x10 */
} PartyRecord29;

extern int func_089FA6E0(Screen0B6A0 *self, int record, int arg);
extern int func_089FA83C(Screen0B6A0 *self, int record, int points);
extern int func_089FA884(Screen0B6A0 *self, int record);
extern void func_089FA8AC(Screen0B6A0 *self, int arg);

void func_089FA91C(Screen0B6A0 *self)
{
    PartyRecord29 *record = (PartyRecord29 *)func_088CB164(D_089D712C->field_F58, WORK_INT(self->work, 0x328C));
    Party29 *party;

    WORK_INT(self->work, 0x3290) = record->level;
    party = &record->party;
    WORK_BYTE(self->work, 0x3294) = party->members[party->current].level;
    if (WORK_BYTE(self->work, 0x3294) == 0) {
        WORK_BYTE(self->work, 0x3294) = WORK_INT(self->work, 0x3290);
    }
    WORK_INT(self->work, 0x32F4) = 0;
    WORK_INT(self->work, 0x32C8) = func_089FA6E0(self, WORK_INT(self->work, 0x328C), 1);
    if (WORK_INT(self->work, 0x2E98) & 1) {
        WORK_INT(self->work, 0x32A0) = 0;
        WORK_INT(self->work, 0x32B4) = 0;
    } else {
        WORK_INT(self->work, 0x32A0) = func_089FA83C(self, WORK_INT(self->work, 0x328C), WORK_INT(self->work, 0x32C8));
        WORK_INT(self->work, 0x32B4) = func_089FA884(self, WORK_INT(self->work, 0x328C));
    }
    func_089FA8AC(self, 0);
}
