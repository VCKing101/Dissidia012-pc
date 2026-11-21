#include "main/agent33_texture.h"

/* List and stock helpers for the seq objects and texture entries of the manager. */
struct GroupLists {
    static void unlink_seq(D012TexManager *mgr, D012SeqObject *seq) {
        if (seq->prev == 0) {
            mgr->seq_head = seq->next;
        } else {
            D012SeqObject **link = &seq->prev->next;
            *link = seq->next;
        }
        D012SeqObject **link;
        if (seq->next == 0) {
            mgr->seq_tail = seq->prev;
        } else {
            link = &seq->next->prev;
            *link = seq->prev;
        }
        seq->prev = 0;
        seq->next = 0;
        mgr->seq_count--;
    }
    static void give_back_seq(D012TexManager *mgr, D012SeqObject *seq) {
        D012StockSlot *slot;
        D012StockSlot *ret = mgr->seq_return;
        for (slot = mgr->seq_take; slot->object_addr != 0; slot = slot->next_addr) {
        }
        ret->object_addr = seq;
        mgr->seq_return = mgr->seq_return->next_addr;
    }
    static void unlink_tex(D012TexManager *mgr, D012TexEntry *entry) {
        if (entry->prev_addr == 0) {
            mgr->tex_head = entry->next_addr;
        } else {
            D012TexEntry **link = &entry->prev_addr->next_addr;
            *link = entry->next_addr;
        }
        D012TexEntry **link;
        if (entry->next_addr == 0) {
            mgr->tex_tail = entry->prev_addr;
        } else {
            link = &entry->next_addr->prev_addr;
            *link = entry->prev_addr;
        }
        entry->prev_addr = 0;
        entry->next_addr = 0;
        mgr->tex_count--;
    }
};

struct Text088D0370 {
    unsigned char unk_0000[0xF2C0];
    void *colors;                      /* +0xF2C0 */
};

extern "C" {

extern Text088D0370 *D_089D6998;
int func_088DCDF8(D012SeqObject *seq, unsigned char group, int id);   /* seq_match */
int func_088DCD60(D012SeqObject *seq);                                /* seq_release: 1 when freed */
int func_088DE1E8(D012TexEntry *entry, unsigned char group, int id);  /* tex_match */
int func_088DE1A8(D012TexEntry *entry);                               /* tex_release: 1 when freed */
void func_0887D60C(void *colors);

/* group_unload: release every seq object and texture entry of a group; those whose
 * reference count drops to zero are unlinked and given back to their stock. With
 * `reset_text` the text colour table is reset too. */
void func_088D0370(D012TexManager *mgr, unsigned char group, unsigned char reset_text)
{
    unsigned char also_text = reset_text;
    D012SeqObject *seq;
    D012SeqObject *next_seq;
    D012TexEntry *entry;
    D012TexEntry *next_entry;

    for (seq = mgr->seq_head; seq != 0; seq = next_seq) {
        next_seq = seq->next;
        if (func_088DCDF8(seq, group, -1) && func_088DCD60(seq)) {
            GroupLists::unlink_seq(mgr, seq);
            GroupLists::give_back_seq(mgr, seq);
        }
    }
    for (entry = mgr->tex_head; entry != 0; entry = next_entry) {
        next_entry = entry->next_addr;
        if (func_088DE1E8(entry, group, -1) && func_088DE1A8(entry)) {
            GroupLists::unlink_tex(mgr, entry);
            D012TexEntries::release(mgr, entry);
        }
    }
    if (also_text) {
        func_0887D60C(D_089D6998->colors);
    }
}

}
