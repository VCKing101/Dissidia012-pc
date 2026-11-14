/* Returns the id field of the first of four 0x80-byte members with `id`, or 0. */
struct Member {
    char pad_00[0x52];
    short id;
    char pad_54[0x80 - 0x54];
};

struct MemberList {
    char pad_00[0x80];
    Member members[4];
};

extern "C" {
short *func_088F4EB4(MemberList *list, int id)
{
    short *result = 0;

    for (int i = 0; i < 4; i++) {
        if (list->members[i].id == id) {
            result = &list->members[i].id;
            break;
        }
    }
    return result;
}
}
