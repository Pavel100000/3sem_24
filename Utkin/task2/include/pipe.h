#pragma once

typedef struct pPipe Pipe;
typedef struct op_table Ops;

size_t send(Pipe* pipe2);
size_t recieve(Pipe* pipe2);

typedef struct op_table  {
    size_t (*rcv)(Pipe *self); 
    size_t (*snd)(Pipe *self); 
} Ops;

typedef struct pPipe {
    char* data; // intermediate buffer
    int fd_direct[2]; // array of r/w descriptors for "pipe()" call (for parent-->child direction)
    int fd_back[2]; // array of r/w descriptors for "pipe()" call (for child-->parent direction)
    size_t len; // data length in intermediate buffer
    Ops actions;
} Pipe;

Pipe* c_pipe2();
size_t send(Pipe* pipe2);
size_t recieve(Pipe* pipe2);