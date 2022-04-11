#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define REP(i,a,b) for(ll i=a; i<b; i++)


class Node {
    private:
        vector<int> data;
        vector<Node*> children;
        Node* parent;
        Node *left, *right;
        int degree;
        bool isDataNode;
        int data_capacity;

    public:
        Node(int, int, bool);
        void setParent(Node*);
        bool insert(int);
        void setKey(int, int);
        void setDataNode(bool);
        Node* getParent();
        Node* getChild(int);
        void setChild(int, Node*);
        int getKey(int);
        int getDegree();
        bool isNodeData();
        bool isFull();
        bool isEmpty();
        void sortData();
        void setLeft(Node*);
        void setRight(Node*);
        Node* getLeft();
        Node* getRight();
        vector<int> getData();
        void clearData();
        int getSize();
};

Node::Node(int degree, int data_capacity, bool data) {
    this->degree = degree;
    this->data_capacity = data_capacity;
    this->isDataNode = data;

    ll i;

    REP(i,0,degree) {
        children.push_back(NULL);
    }
    children.push_back(NULL);
    parent = NULL;    
    left = NULL; right = NULL;
}

vector<int> Node::getData() {
    return data;
}

Node* Node::getParent() {
    return parent;
}

Node* Node::getChild(int i) {
    return children[i];
}

int Node::getKey(int i) {
    return data[i];
}

void Node::setChild(int i, Node* child) {
    children[i] = child;
}

void Node::setLeft(Node* child) {
    left = child;
}

void Node::setRight(Node* child) {
    right = child;
}

Node* Node::getLeft() {
    return left;
}

Node* Node::getRight() {
    return right;
}

int Node::getSize() {
    return data.size();
}

bool Node::isFull() {
    if (isDataNode) {
        if (data.size() == data_capacity) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        if (data.size() == degree) {
            return true;
        }
        else {
            return false;
        }
    }
}

bool Node::isEmpty() {
    if (isDataNode) {
        if (data.size() == 0) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        if (data.size() == 0) {
            return true;
        }
        else {
            return false;
        }
    }
}

bool Node::isNodeData() {
    return isDataNode;
}

bool Node::insert(int key) {
    if(isFull())
        return false;
    else {
        data.push_back(key);
        return true;
    }
}

void Node::clearData() {
    data.clear();
}

void Node::sortData(){
    sort(data.begin(), data.end());
}

class BPlusTree {
    private:
        Node* root;
        int degree;
        int data_capacity;
        int no_of_data_nodes;
        int no_of_index_nodes;
        void insert(Node*, int);
        void split(Node*, Node*, Node*, int);
        void splitIndex(Node*, Node*, Node*, Node*, int);

    public:
        BPlusTree(int, int);
        void insert(int);
        void traverse(Node*);
        void traverse();
};

BPlusTree::BPlusTree(int degree, int data_capacity) {
    this->degree = degree;
    this->data_capacity = data_capacity;
    root = new Node(degree, data_capacity, true);
    no_of_data_nodes = 0;
    no_of_index_nodes = 0;
}

void BPlusTree::splitIndex(Node* root, Node* sib, Node* newroot, Node* newchild, int key) {
    if (root->getParent() == NULL)
        root->setParent(newroot);

    sib->setParent(root->getParent());

    vector<int> temp = root->getData();
    temp.push_back(key);
    sort(temp.begin(), temp.end());

    

}

void BPlusTree::split(Node* root, Node* sib, Node* new_root, int key) {
    if (root->getParent() == NULL) 
        root->setParent(new_root);

    sib->setParent(root->getParent());

    sib->setLeft(root);
    sib->setRight(root->getRight());

    vector<int> temp = root->getData();

    temp.push_back(key);
    sort(temp.begin(), temp.end());

    int mid = data_capacity/2;

    ll i;
    REP(i,mid,temp.size()) {
        sib->insert(temp[i]);
    }

    sib->sortData();
    root->clearData();

    REP(i,0,mid) {
        root->insert(temp[i]);
    }
    
    if (root->getParent()->getSize() < degree) {
        root->getParent()->insert(sib->getKey(0));
        root->getParent()->sortData();

        if (root->getParent()->getParent() == NULL)
            this->root = root->getParent();

        int index = root->getParent()->getSize();

        if (index == 1) 
            root->getParent()->setChild(0, root);
        
        root->getParent()->setChild(index, sib);

        for (int i=0; i<root->getParent()->getSize()+1; i++) {
            for (int j=0; j < root->getParent()->getSize() + 1; j++) {
                if (root->getParent()->getChild(i)->getKey(0) > root->getParent()->getChild(j)->getKey(0) ) {
                    Node* temp = root->getParent()->getChild(i);
                    root->getParent()->setChild(i, root->getParent()->getChild(j));
                    root->getParent()->setChild(j, temp);
                }
            }
        }
    }
    else {
        Node* new_sib = new Node(degree, data_capacity, false);
        Node* root_new = new Node(degree, data_capacity,false);
        int val = sib->getKey(0);

        splitIndex(root->getParent(), new_sib, root_new, sib, val);
    }
}

void BPlusTree::insert(Node *root, int key) {
    if (root->isNodeData()) {
        bool status = root->insert(key);

        if (!status) {
            Node* sibling = new Node(degree, data_capacity, true);
            Node* new_root = new Node(degree, data_capacity, false);
            
            split(root, sibling, new_root, key);
        } else {
            root->sortData();
        }
    } else {
        int index = 0;
        for (int i = 0; i < root->getSize(); ++i) {
            if (key > root->getKey(i)) {
                index++;
            }
        }

        insert(root->getChild(index), key);
    }
}

void BPlusTree::insert(int key) {
    insert(root, key);
}