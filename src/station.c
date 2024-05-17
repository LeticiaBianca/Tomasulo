#include "station.h"

static const char* _station_names[] = {
    [AddSub]  = "AddSub",
    [MulDiv]  = "MulDiv",
    [LdStore] = "LdStore",
};

void station_fmt(FILE* stream, Station* s)
{
    fprintf(stream, "{\n\t");
    fprintf(stream, "id: %u\n\t", s->id);
    fprintf(stream, "type: %s\n\t", _station_names[s->type]);
    fprintf(stream, "instruction: ");
    instruction_fmt(stream, s->inst);
    fprintf(stream, "vj: %li\n\t", s->vj);
    fprintf(stream, "vk: %li\n\t", s->vk);
    fprintf(stream, "qj: %u\n\t", s->qj);
    fprintf(stream, "qk: %u\n", s->qk);
    fprintf(stream, "}\n");
}
