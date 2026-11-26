// Builds a replay save's detail text at save_block+0x100 (0x400 bytes): optional title, both fighters' names, the stage and the duration; does nothing when the match info is out of range.
extern int D_089D7260;
extern int D_089D712C;
extern int D_089CB758;
extern int func_088AFBAC(void);
extern int func_088AFB8C(int, int);
extern int func_088AFBB4(int);
extern int func_0887AFBC(int, int, int);
extern int func_08935BB4(int, int, int);
extern int func_089359E8(int);
extern int func_0893FDCC(int, int, int);
extern int func_088ABAF4(int, void *, int, int);

void func_088B0CC8(int save_block, int title_text)
{
    int length;
    int piece;
    int strings;
    int text_start;
    int end_ptr;
    int fighter2, alt2;
    int duration;
    int write_at;
    int capacity;
    int info;
    int stage_count;
    char time_text[128];

    stage_count = func_088AFBAC();
    info = *(int *)(D_089D7260 + 0x14);
    if (*(int *)(info + 0xEA48) < 1 || *(int *)(info + 0xEA48) >= 0x21 || *(int *)(info + 0xEA4C) < 0 || *(int *)(info + 0xEA4C) >= 3
        || *(int *)(info + 0xEA50) < 1 || *(int *)(info + 0xEA50) >= 0x21
        || *(int *)(info + 0xEA54) < 0 || *(int *)(info + 0xEA54) >= 3
        || *(int *)(info + 0xEA58) < 0 || *(int *)(info + 0xEA58) >= stage_count) return;
    strings = *(int *)(D_089D712C + 0xEE0);
    length = 0;
    capacity = 0x400;
    text_start = save_block + 0x100;
    if (title_text != 0) {
        write_at = length + save_block + 0x100;
        piece = func_0887AFBC(strings, 4, 0);
        func_08935BB4(write_at, piece, capacity);
        length = func_089359E8(text_start);
        func_08935BB4(length + save_block + 0x100, title_text, capacity - length);
        length = func_089359E8(text_start);
        func_08935BB4(length + save_block + 0x100, (int)&D_089CB758, capacity - length);
        length = func_089359E8(text_start);
    }
    write_at = length + save_block + 0x100;
    piece = func_0887AFBC(strings, func_088AFB8C(*(int *)(info + 0xEA48), *(int *)(info + 0xEA4C)), 0);
    func_08935BB4(write_at, piece, capacity - length);
    length = func_089359E8(text_start);
    write_at = length + save_block + 0x100;
    piece = func_0887AFBC(strings, 0x25, 0);
    func_08935BB4(write_at, piece, capacity - length);
    length = func_089359E8(text_start);
    end_ptr = length + save_block;
    fighter2 = *(int *)(info + 0xEA50);
    alt2 = *(int *)(info + 0xEA54);
    write_at = end_ptr + 0x100;
    piece = func_0887AFBC(strings, func_088AFB8C(fighter2, alt2), 0);
    func_08935BB4(write_at, piece, capacity - length);
    length = func_089359E8(text_start);
    func_08935BB4(length + save_block + 0x100, (int)&D_089CB758, capacity - length);
    length = func_089359E8(text_start);
    write_at = length + save_block + 0x100;
    piece = func_0887AFBC(strings, 0x26, 0);
    func_08935BB4(write_at, piece, capacity - length);
    length = func_089359E8(text_start);
    write_at = length + save_block + 0x100;
    piece = func_0887AFBC(strings, func_088AFBB4(*(int *)(info + 0xEA58)), 0);
    func_08935BB4(write_at, piece, capacity - length);
    length = func_089359E8(text_start);
    func_08935BB4(length + save_block + 0x100, (int)&D_089CB758, capacity - length);
    length = func_089359E8(text_start);
    write_at = length + save_block + 0x100;
    piece = func_0887AFBC(strings, 0x24, 0);
    func_08935BB4(write_at, piece, capacity - length);
    length = func_089359E8(text_start);
    duration = *(int *)(info + 0xEA5C);
    func_088ABAF4(D_089D712C, time_text, 0x80, duration);
    func_0893FDCC(length + save_block + 0x100, capacity - length, (int)time_text);
    func_089359E8(text_start);
}
