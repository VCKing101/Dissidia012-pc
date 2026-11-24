/* Finds the 0x28-byte entry in the global table whose first halfword equals id. */
typedef struct { short id; char pad[0x26]; } Ent;
typedef struct { char d[0x12]; short count; } Hdr;
typedef struct { int x; Hdr *hdr; int y; Ent *ents; } Glob;
extern Glob D_09B6CD90;
Ent *func_08AD113C(short id)
{
    int i = 0;
    Ent *r = 0;
    if (i < D_09B6CD90.hdr->count) {
        Ent *ents = D_09B6CD90.ents;
        int off = 0;
        Ent *p = ents;
        do {
            if (p->id == id) { r = (Ent *)((char *)ents + off); break; }
            i++;
            off += 0x28;
            p++;
        } while (i < D_09B6CD90.hdr->count);
    }
    return r;
}
