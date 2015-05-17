#include "nodebase.h"

#include <stdlib.h>

NodeBase::NodeBase(){
    nextNode = NULL;
    prevNode = NULL;
}

NodeBase::~NodeBase(){
}


NodeBase *NodeBase::next(){ return nextNode; }
NodeBase *NodeBase::prev(){ return prevNode; }


void NodeBase::setNext(NodeBase *next){ nextNode = next; }
void NodeBase::setPrev(NodeBase *prev){ prevNode = prev; }

