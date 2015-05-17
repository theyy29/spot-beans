#ifndef NODEBASE_H_INCLUDED
#define NODEBASE_H_INCLUDED

class NodeBase {
public:
    NodeBase();
    ~NodeBase();

    NodeBase *next();
    NodeBase *prev();

    void setNext(NodeBase *next);
    void setPrev(NodeBase *prev);

private:
    NodeBase *nextNode;
    NodeBase *prevNode;
};

#endif /* ifndef NODEBASE_H_INCLUDED */
