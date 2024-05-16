# Tomasulo

## Key Structures

Consider the three `MIPS` instructions below:

```mips

DIV.D F0, F2, F4
ADD.D F10, F0, F8
SUB.D F12, F8, F14

```

Given that `ADD.D` is dependant on `DIV.D` to provide the final value of `F0`, there needs to be a way of buffering the `ADD.D` operation so that `SUB.D`, which has no dependency whatsoever, can proceed its execution without waiting for the stalls caused by the previous dependency.

In Tomasulo's algorithm these buffers are called **Reservation Stations**.

### Reservation Station

Fetches and stores instruction operands as soon as they're available. Each reservation station corresponds to one instruction. Once all source operands are available, the instruction is sent for execution, provided a functional unit is also available. Once execution is complete, the result is buffered at the reservation station.

### ...

## Pipeline Phases

1. **Instruction Fetch**: fetch the next instruction into queue of pending ones.

2. **Issue**: get the next instruction from head of instruction queue.
    - If the matching reservation station is free, issue instruction to it.

3. **Execute**: when all operands are available and the functional unit is free, execute.
    - If not, monitor reservation stations and register file for operands.

4. **Write Result**: write result to all awaiting stations and register file.
