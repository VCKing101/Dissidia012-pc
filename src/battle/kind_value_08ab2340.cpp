typedef struct KindEntry {
    char pad_00[0x18];
    int value;
} KindEntry;

typedef struct KindInfo {
    unsigned char id;
} KindInfo;

typedef struct KindBody {
    char pad_000[0x5A4];
    KindInfo *info;
} KindBody;

typedef struct KindLink {
    char pad_00[0x4];
    KindBody *body;
} KindLink;

typedef struct KindOwner {
    unsigned char active;
    char pad_01[0x3];
    KindLink *link;
} KindOwner;

extern "C" {

extern int D_08A80218;
extern int func_08A80200(int id);
extern KindEntry *func_08AACD30(int context, int id);

bool func_08AB2340(KindOwner *self)
{
    if (!self->active) {
        return false;
    }
    KindBody *body = self->link->body;
    if (body == 0) {
        return false;
    }
    KindInfo *info = body->info;
    if (info == 0) {
        return false;
    }
    int context = func_08A80200(D_08A80218);
    KindEntry *entry = func_08AACD30(context, info->id);
    if (entry == 0) {
        return false;
    }
    return entry->value != 0;
}

}
