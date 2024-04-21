#include <iostream>
#include <memory>
#include <string>

template <typename T>
struct ListNode {
    T body;
    std::unique_ptr<ListNode<T>> next;
};

template <typename T>
class LinkedList {
public:
    using NodePtr = std::unique_ptr<ListNode<T>>;

    LinkedList() = default;
    NodePtr find(int k) const {
        NodePtr* p = &head;
        int i = 1;
        while (i++ != k && *p)
            p = &((*p)->next);
        if (!*p) {
            std::cout << "Узла не существует\n";
            return nullptr;
        }
        return std::move(*p);
    }

    void addFirst(NodePtr newNode) {
        newNode->next = std::move(head);
        head = std::move(newNode);
        if (!tail)
            tail = head.get();
    }

    void addLast(NodePtr newNode) {
        newNode->next = nullptr;
        if (tail)
            tail->next = std::move(newNode);
        tail = newNode.get();
        if (!head)
            head = std::move(newNode);
    }

    void addAfter(NodePtr& p, NodePtr newNode) {
        if (!p->next)
            addLast(std::move(newNode));
        else {
            newNode->next = std::move(p->next);
            p->next = std::move(newNode);
        }
    }

    void addBefore(NodePtr& p, NodePtr newNode) {
        if (p == head)
            addFirst(std::move(newNode));
        else {
            NodePtr* prevNext = &head;
            while (*prevNext != p)
                prevNext = &(*prevNext)->next;
            newNode->next = std::move(p);
            *prevNext = std::move(newNode);
        }
    }

    void deleteNode(NodePtr& p) {
        if (p == head) {
            head = std::move(p->next);
            if (!head)
                tail = nullptr;
        }
        else {
            NodePtr prevNext = &head;
            while (*prevNext != p)
                prevNext = &(*prevNext)->next;
            *prevNext = std::move(p->next);
            if (!(*prevNext)->next)
                tail = *prevNext;
        }
    }
private:
    NodePtr head = nullptr;
    NodePtr tail = nullptr;
};
