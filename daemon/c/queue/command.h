#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

#include <string>

#include "nodebase.h"

class Command : public NodeBase {
public:
    Command(std::string command);
    ~Command();

    // Thread entry point. Somehow 'returns' the data to be sent back over the socket.
    // void start();

    std::string DBGECHO();
private:
    std::string verbatim;
};

#endif /* ifndef COMMAND_H_INCLUDED */
