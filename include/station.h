#ifndef STATION_H
#define STATION_H

#include "common.h"
#include "instruction.h"

typedef uint32_t StationId;

typedef enum {
    AddSub,
    MulDiv,
    LdStore,
} StationType;

typedef struct {
    StationId    id;
    StationType  type;
    Instruction* inst;

    i64 vj;
    i64 vk;
    StationId qj;
    StationId qk;
} Station;

void station_fmt(FILE* stream, Station* s);

#endif
