#include "command.h"
#include "commands.h"

#include <string.h>

void c_unknown(Command *c){
}

void c_echo(Command *c){
    c->reply = (char *)malloc(strlen(c->command) + 1);
    memcpy(c->reply, c->command, strlen(c->command) + 1);
}

void c_info(Command *c){
}
