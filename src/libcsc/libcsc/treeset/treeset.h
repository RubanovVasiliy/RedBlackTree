#pragma once

#include <iostream>

namespace my_set {

    class TreeSet {

        struct Node {
            Node *parent = nullptr;
            Node *left = nullptr;
            Node *right = nullptr;
            int key;
            char color = 'B';

            Node operator=(const Node &node) = delete;

            Node operator=(Node &&node) = delete;
        };

    private:
        Node *root_;
        Node *nil;
        std::size_t size_ = 0;

        Node *findHelper(Node *node, int key) const {
            if (node == nil || key == node->key) {
                return node;
            }
            if (key < node->key) {
                return findHelper(node->left, key);
            }
            return findHelper(node->right, key);
        }

        void balanceDelete(Node *node) {
            Node *s = nullptr;
            while (node != root_ && node->color == 'B') {
                if (node == node->parent->left) {
                    s = node->parent->right;
                    if (s->color == 'R') {
                        s->color = 'B';
                        node->parent->color = 'R';
                        leftRotate(node->parent);
                        s = node->parent->right;
                    }
                    if (s->left->color == 'B' && s->right->color == 'B') {
                        s->color = 'R';
                        node = node->parent;
                    } else {
                        if (s->right->color == 'B') {
                            s->left->color = 'B';
                            s->color = 'R';
                            rightRotate(s);
                            s = node->parent->right;
                        }
                        s->color = node->parent->color;
                        node->parent->color = 'B';
                        s->right->color = 'B';
                        leftRotate(node->parent);
                        node = root_;
                    }
                } else {
                    s = node->parent->left;
                    if (s->color == 'R') {
                        s->color = 'B';
                        node->parent->color = 'R';
                        rightRotate(node->parent);
                        s = node->parent->left;
                    }
                    if (s->right->color == 'B' && s->left->color == 'B') {
                        s->color = 'R';
                        node = node->parent;
                    } else {
                        if (s->left->color == 'B') {
                            s->right->color = 'B';
                            s->color = 'R';
                            leftRotate(s);
                            s = node->parent->left;
                        }
                        s->color = node->parent->color;
                        node->parent->color = 'B';
                        s->left->color = 'B';
                        rightRotate(node->parent);
                        node = root_;
                    }
                }
            }
            node->color = 'B';
        }

        void balanceInsert(Node *k) {
            Node *u = nullptr;
            while (k->parent->color == 'R') {
                if (k->parent == k->parent->parent->right) {
                    u = k->parent->parent->left; // uncle
                    if (u->color == 'R') {
                        u->color = 'B';
                        k->parent->color = 'B';
                        k->parent->parent->color = 'R';
                        k = k->parent->parent;
                    } else {
                        if (k == k->parent->left) {
                            k = k->parent;
                            rightRotate(k);
                        }
                        k->parent->color = 'B';
                        k->parent->parent->color = 'R';
                        leftRotate(k->parent->parent);
                    }
                } else {
                    u = k->parent->parent->right;
                    if (u->color == 'R') {
                        u->color = 'B';
                        k->parent->color = 'B';
                        k->parent->parent->color = 'R';
                        k = k->parent->parent;
                    } else {
                        if (k == k->parent->right) {
                            k = k->parent;
                            leftRotate(k);
                        }
                        k->parent->color = 'B';
                        k->parent->parent->color = 'R';
                        rightRotate(k->parent->parent);
                    }
                }
                if (k == root_) {
                    break;
                }
            }
            root_->color = 'B';
            size_++;
        }

        void transplant(Node *u, Node *v) {
            if (u->parent == nullptr) {
                root_ = v;
            } else if (u == u->parent->left) {
                u->parent->left = v;
            } else {
                u->parent->right = v;
            }
            v->parent = u->parent;
        }

        void leftRotate(Node *node) {
            Node *y = node->right;
            node->right = y->left;
            if (y->left != nil) {
                y->left->parent = node;
            }
            y->parent = node->parent;
            if (node->parent == nullptr) {
                root_ = y;
            } else if (node == node->parent->left) {
                node->parent->left = y;
            } else {
                node->parent->right = y;
            }
            y->left = node;
            node->parent = y;
        }

        void rightRotate(Node *node) {
            Node *y = node->left;
            node->left = y->right;
            if (y->right != nil) {
                y->right->parent = node;
            }
            y->parent = node->parent;
            if (node->parent == nullptr) {
                root_ = y;
            } else if (node == node->parent->right) {
                node->parent->right = y;
            } else {
                node->parent->left = y;
            }
            y->right = node;
            node->parent = y;
        }

        Node *getMin(Node *node) const {
            while (node->left != nil) {
                node = node->left;
            }
            return node;
        }

        Node *getMax(Node *node) const {
            while (node->right != nil) {
                node = node->right;
            }
            return node;
        }

        Node *getRoot() const {
            return root_;
        }

        void printTree(const Node *node) {
            if (node == nil) {
                return;
            }
            printTree(node->left);
            char color = node->color == 'R' ? 'R' : 'B';
            std::cout << color << " key: " << node->key << '\n';
            printTree(node->right);
        }

    public:
        TreeSet() : nil(new Node()) {
            root_ = nil;
        }

        void clear() {
            while (getRoot() != nil) {
                Node *node = getMin(root_);
                remove(node->key);
            }
        }

        std::size_t size() const {
            return size_;
        }

        bool contains(const int key) const {
            return findHelper(root_, key)->key == key;
        }

        void insert(int key) {
            Node *tmp = new Node();
            tmp->key = key;
            tmp->left = nil;
            tmp->right = nil;
            tmp->color = 'R';

            Node *y = nullptr;
            Node *node = root_;

            while (node != nil) {
                y = node;
                if (tmp->key < node->key) {
                    node = node->left;
                } else {
                    node = node->right;
                }
            }

            tmp->parent = y;
            if (y == nullptr) {
                root_ = tmp;
            } else if (tmp->key < y->key) {
                y->left = tmp;
            } else {
                y->right = tmp;
            }

            if (tmp->parent == nullptr) {
                tmp->color = 'B';
                size_++;
                return;
            }

            if (tmp->parent->parent == nullptr) {
                size_++;
                return;
            }
            balanceInsert(tmp);
        }

        bool remove(int key) {
            Node *root = getRoot();
            Node *z = nil;
            while (root != nil) {
                if (root->key == key) {
                    z = root;
                }
                if (root->key <= key) {
                    root = root->right;
                } else {
                    root = root->left;
                }
            }

            if (z == nil) {
                return false;
            }

            Node *node = nullptr;
            Node * y = z;
            char y_color = y->color;
            if (z->left == nil) {
                node = z->right;
                transplant(z, z->right);
            } else if (z->right == nil) {
                node = z->left;
                transplant(z, z->left);
            } else {
                y = getMin(z->right);
                y_color = y->color;
                node = y->right;
                if (y->parent == z) {
                    node->parent = y;
                } else {
                    transplant(y, y->right);
                    y->right = z->right;
                    y->right->parent = y;
                }
                transplant(z, y);
                y->left = z->left;
                y->left->parent = y;
                y->color = z->color;
            }
            delete z;
            if (y_color == 'B') {
                balanceDelete(node);
            }
            size_--;
            return true;
        }

        void print() {
            printTree(root_);
        }
    };
} // namespace my_set