typedef struct Agent4TableHeader {
    int field_00;
    int field_04;
    int count;
    int field_0C;
} Agent4TableHeader;

typedef struct Agent4TableEntry {
    unsigned char data[0xCC];
} Agent4TableEntry;

typedef struct Agent4TableView {
    Agent4TableHeader *header;
    Agent4TableEntry *entries;
    Agent4TableEntry *end;
} Agent4TableView;

void func_08828A98(Agent4TableView *self, Agent4TableHeader *header) {
    Agent4TableEntry *entries;
    self->header = header;
    entries = (Agent4TableEntry *)(header + 1);
    self->entries = entries;
    self->end = entries + header->count;
}

Agent4TableHeader *func_08828AC8(Agent4TableView *self) {
    Agent4TableHeader *header = self->header;
    if (header != 0) {
        self->header = 0;
        self->entries = 0;
        self->end = 0;
    }
    return header;
}
