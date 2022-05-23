#pragma once

#include <iostream>
#include <iterator>

namespace my_set {

    template <typename K>
    class TreeSet {

        struct Node {
            Node *parent = nullptr;
            Node *left = nullptr;
            Node *right = nullptr;
            K key;
            char color = 'B';

            Node operator=(const Node &node) = delete;

            Node operator=(Node &&node) = delete;
        };

    private:
        Node *root_;

        Node *nil_;

        std::size_t size_ = 0;

        Node *findHelper(Node *node, K key) const {
            if (node == nil_ || key == node->key) {
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
            if (y->left != nil_) {
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
            if (y->right != nil_) {
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

        void printTree(const Node *node) {
            if (node == nil_) {
                return;
            }
            printTree(node->left);
            char color = node->color == 'R' ? 'R' : 'B';
            std::cout << color << " key: " << node->key << '\n';
            printTree(node->right);
        }

        Node *getRoot() const { return root_; }

        Node *getMin(Node *node) const {
            if (node == nullptr || node->left == nullptr) {
                return nil_;
            }
            while (node->left != nil_) {
                node = node->left;
            }
            return node;
        }

        Node *getMax(Node *node) const {
            if (node->right == nullptr) {
                return nil_;
            }
            while (node->right != nil_) {
                node = node->right;
            }
            return node;
        }

        Node *getNil() const { return nil_; }

    public:

        class Iterator {
            friend TreeSet;

        private:
            Node *ptr = nullptr;
            Node *nil = nullptr;

            Iterator(Node *ptr, Node *nil) : ptr(ptr), nil(nil) {}

        public:
            using iterator_category = std::input_iterator_tag;
            using value_type = K;
            using difference_type = std::ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

            Iterator() = default;

            ~Iterator() = default;

            bool operator==(Iterator other) { return ptr == other.ptr; }

            bool operator!=(Iterator other) { return ptr != other.ptr; }

            K &operator*() const { return ptr->key; }

            K *operator->() const { return &ptr->key; }

            Iterator &operator++() {
                if (ptr->right != nil) {
                    ptr = ptr->right;
                    while (ptr->left != nil)
                        ptr = ptr->left;
                } else {
                    if (ptr->parent == nullptr) {
                        ptr = nil;
                        return *this;
                    }
                    Node *y = ptr->parent;
                    while (ptr == y->right) {
                        ptr = y;
                        y = y->parent;
                        if (ptr->parent == nullptr) {
                            ptr = nil;
                            return *this;
                        }
                    }
                    if (ptr->right != y)
                        ptr = y;
                }
                return *this;
            }

            Iterator operator++(int) {
                Iterator it = *this;
                ++*this;
                return it;
            }
        };

        TreeSet() : nil_(new Node()) {
            root_ = nil_;
        }

        TreeSet(const TreeSet &other) : nil_(new Node()) {
            root_ = nil_;

            for (auto it = other.begin(); it != other.end(); ++it) {
                this->insert(*it);
            }
        }

        TreeSet(TreeSet &&other) noexcept: root_(other.root_), nil_(other.nil_),
                                           size_(other.size_) {
            other.nil_ = nullptr;
            other.root_ = nullptr;
            other.size_ = 0;
        }

        ~TreeSet() {
            clear();
            delete nil_;
        }

        TreeSet &operator=(const TreeSet &other) {
            if (this == &other) {
                return *this;
            }

            clear();
            for (auto it = other.begin(); it != other.end(); ++it) {
                this->insert(*it);
            }
            return *this;
        }

        TreeSet &operator=(TreeSet &&other) noexcept {
            if (this == &other) {
                return *this;
            }

            nil_ = other.nil_;
            root_ = other.root_;
            size_ = other.size_;
            other.nil_ = nullptr;
            other.root_ = nullptr;
            other.size_ = 0;
            return *this;
        }

        void clear() {
            for (auto it = this->begin(); it != this->end(); it++) {
                this->remove(*it);
            }
        }

        std::size_t size() const { return size_; }

        bool contains(const K key) const {
            return findHelper(root_, key)->key == key;
        }

        void insert(K key) {
            Node *tmp = new Node();
            tmp->key = key;
            tmp->left = nil_;
            tmp->right = nil_;
            tmp->color = 'R';

            Node *y = nullptr;
            Node *node = root_;

            while (node != nil_) {
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

        bool remove(K key) {
            Node *root = getRoot();
            Node *z = nil_;
            while (root != nil_) {
                if (root->key == key) {
                    z = root;
                }
                if (root->key <= key) {
                    root = root->right;
                } else {
                    root = root->left;
                }
            }

            if (z == nil_) {
                return false;
            }

            Node *node = nullptr;
            Node *y = z;
            char y_color = y->color;
            if (z->left == nil_) {
                node = z->right;
                transplant(z, z->right);
            } else if (z->right == nil_) {
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

        Iterator begin() const { return Iterator(getMin(getRoot()), getNil()); }

        Iterator end() const { return Iterator(getNil(), getNil()); }
    };
} // namespace my_set