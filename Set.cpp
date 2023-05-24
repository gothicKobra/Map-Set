#include <iostream>
enum class Color { RED, BLACK };

template <typename T>
struct Node {
    T value;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;
    Color color;

    Node() {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
    explicit Node(const T& x) {
        value = x;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

template <typename ValueType>
class RBTree {
private:
    Node<ValueType>* root{};
    size_t tree_size = 0;
    struct Iterator {
        Iterator() {
            m_ptr_ = nullptr;
        }
        explicit Iterator(Node<ValueType>* node) {
            m_ptr_ = node;
        }

        const ValueType& operator*() const {
            return m_ptr_->value;
        }
        const ValueType* operator->() const {
            return &(m_ptr_->value);
        }

        Iterator& operator++() {
            ValueType mx;
            Node<ValueType>* now = m_ptr_;
            while (now->parent != nullptr) {
                now = now->parent;
            }
            while (now->right != nullptr) {
                now = now->right;
            }
            if (!(now->value < m_ptr_->value || m_ptr_->value < now->value)) {
                konec = true;
                return *this;
            }
            if (m_ptr_->right != nullptr) {
                now = m_ptr_->right;
                while (now->left != nullptr) {
                    now = now->left;
                }
                m_ptr_ = now;
                return *this;
            }
            Node<ValueType>* otec = m_ptr_->parent;
            now = m_ptr_;
            while (otec->right != nullptr &&
                !(now->value < otec->right->value || otec->right->value < now->value)) {
                now = otec;
                otec = otec->parent;
            }
            m_ptr_ = otec;
            return *this;
        }
        Iterator operator++(int) {
            Iterator ans = *this;
            ValueType mx;
            Node<ValueType>* now = m_ptr_;
            while (now->parent != nullptr) {
                now = now->parent;
            }
            while (now->right != nullptr) {
                now = now->right;
            }
            if (!(now->value < m_ptr_->value || m_ptr_->value < now->value)) {
                konec = true;
                return ans;
            }
            if (m_ptr_->right != nullptr) {
                now = m_ptr_->right;
                while (now->left != nullptr) {
                    now = now->left;
                }
                m_ptr_ = now;
            }
            else {
                Node<ValueType>* otec = m_ptr_->parent;
                now = m_ptr_;
                while (otec->right != nullptr &&
                    !(now->value < otec->right->value || otec->right->value < now->value)) {
                    now = otec;
                    otec = otec->parent;
                }
                m_ptr_ = otec;
            }
            return ans;
        }

        Iterator& operator--() {
            if (konec) {
                konec = false;
                return *this;
            }
            if (m_ptr_->left != nullptr) {
                Node<ValueType>* now = m_ptr_->left;
                while (now->right != nullptr) {
                    now = now->right;
                }
                m_ptr_ = now;
                return *this;
            }
            Node<ValueType>* otec = m_ptr_->parent;
            Node<ValueType>* now = m_ptr_;
            while (otec->left != nullptr &&
                !(now->value < otec->left->value || otec->left->value < now->value)) {
                now = otec;
                otec = otec->parent;
            }
            m_ptr_ = otec;
            return *this;
        }
        Iterator operator--(int) {
            if (konec) {
                Iterator ans = *this;
                konec = false;
                return ans;
            }
            Iterator ans = *this;
            if (m_ptr_->left != nullptr) {
                Node<ValueType>* now = m_ptr_->left;
                while (now->right != nullptr) {
                    now = now->right;
                }
                m_ptr_ = now;
            }
            else {
                Node<ValueType>* otec = m_ptr_->parent;
                Node<ValueType>* now = m_ptr_;
                while (otec->left != nullptr &&
                    !(now->value < otec->left->value || otec->left->value < now->value)) {
                    now = otec;
                    otec = otec->parent;
                }
                m_ptr_ = otec;
            }
            return ans;
        }

        bool operator==(const Iterator& other) const {
            bool ok = 0;
            ok ^= konec;
            ok ^= other.konec;
            if (ok) {
                return false;
            }
            return !(m_ptr_ < other.m_ptr_ || other.m_ptr_ < m_ptr_);
        }
        bool operator!=(const Iterator& other) const {
            bool ok = 0;
            ok ^= konec;
            ok ^= other.konec;
            if (ok) {
                return true;
            }
            return (m_ptr_ < other.m_ptr_ || other.m_ptr_ < m_ptr_);
        }
        Node<ValueType>* m_ptr_;
        bool konec = 0;
    };
    void del(Node<ValueType>* v) {
        if (v->left != nullptr) {
            del(v->left);
        }
        if (v->right != nullptr) {
            del(v->right);
        }
        delete v;
    }

    void copydfs(Node<ValueType>* now, Node<ValueType>* origin) {
        now->color = origin->color;
        now->value = origin->value;
        if (origin->left != nullptr) {
            now->left = new Node<ValueType>;
            now->left->parent = now;
            copydfs(now->left, origin->left);
        }
        if (origin->right != nullptr) {
            now->right = new Node<ValueType>;
            now->right->parent = now;
            copydfs(now->right, origin->right);
        }
    }

    Node<ValueType>* rotateLeft(Node<ValueType>* now) {
        now->parent->right = now->left;
        if (now->left != nullptr) {
            now->left->parent = now->parent;
        }
        now->left = now->parent;
        Node<ValueType>* fic_node = now->parent->parent;
        now->parent->parent = now;
        if (fic_node != nullptr) {
            if (fic_node->left != nullptr && now->parent->value < fic_node->value) {
                fic_node->left = now;
                now->parent = fic_node;
            }
            else {
                fic_node->right = now;
                now->parent = fic_node;
            }
        }
        now->parent = fic_node;
        return now;
    }

    Node<ValueType>* rotateRight(Node<ValueType>* now) {
        now->parent->left = now->right;
        if (now->right != nullptr) {
            now->right->parent = now->parent;
        }
        now->right = now->parent;
        Node<ValueType>* fic_node = now->parent->parent;
        now->parent->parent = now;
        if (fic_node != nullptr) {
            if (fic_node->left != nullptr && now->parent->value < fic_node->value) {
                fic_node->left = now;
                now->parent = fic_node;
            }
            else {
                fic_node->right = now;
                now->parent = fic_node;
            }
        }
        now->parent = fic_node;
        return now;
    }

    void getRid(Node<ValueType>* now) {
        if (now->left != nullptr && now->right != nullptr) {
            Node<ValueType>* last = now->left;
            while (last->right != nullptr) {
                last = last->right;
            }
            ValueType res = last->value;
            getRid(last);
            now->value = res;
            return;
        }
        if (now->left != nullptr && now->right == nullptr) {
            if (!(now->value < root->value || root->value < now->value)) {
                root = now->left;
                root->color = Color::BLACK;
                root->parent = nullptr;
                delete now;
                tree_size--;
                return;
            }
            if (now->value < now->parent->value) {
                now->parent->left = now->left;
                now->left->parent = now->parent;
                now->left->color = Color::BLACK;
                delete now;
                tree_size--;
                return;
            }
            now->parent->right = now->left;
            now->left->parent = now->parent;
            now->left->color = Color::BLACK;
            delete now;
            tree_size--;
            return;
        }
        if (now->left == nullptr && now->right != nullptr) {
            if (!(now->value < root->value || root->value < now->value)) {
                root = now->right;
                root->color = Color::BLACK;
                root->parent = nullptr;
                delete now;
                tree_size--;
                return;
            }
            if (now->value < now->parent->value) {
                now->parent->left = now->right;
                now->right->parent = now->parent;
                now->right->color = Color::BLACK;
                delete now;
                tree_size--;
                return;
            }
            now->parent->right = now->right;
            now->right->parent = now->parent;
            now->right->color = Color::BLACK;
            delete now;
            tree_size--;
            return;
        }
        if (now->left == nullptr && now->right == nullptr && now->color == Color::RED) {
            if (now->value < now->parent->value) {
                now->parent->left = nullptr;
                delete now;
                tree_size--;
                return;
            }
            now->parent->right = nullptr;
            delete now;
            tree_size--;
            return;
        }
        Node<ValueType>* otec = now->parent;
        bool low = 0;
        if (now->value < otec->value) {
            otec->left = nullptr;
            low = 1;
        }
        else {
            otec->right = nullptr;
        }
        delete now;
        tree_size--;
        bool success = 0;
        bool first = 1;
        while (!success) {
            if (!first && !(now->value < root->value || root->value < now->value)) {
                return;
            }
            if (first) {
                if (low) {
                    if (otec->color == Color::RED && otec->right->color == Color::BLACK &&
                        (otec->right->right == nullptr ||
                            otec->right->right->color == Color::BLACK) &&
                            (otec->right->left == nullptr ||
                                otec->right->left->color == Color::BLACK)) {
                        otec->right->color = Color::RED;
                        otec->color = Color::BLACK;
                        return;
                    }
                    if (otec->color == Color::BLACK && otec->right->color == Color::BLACK &&
                        (otec->right->right == nullptr ||
                            otec->right->right->color == Color::BLACK) &&
                            (otec->right->left == nullptr ||
                                otec->right->left->color == Color::BLACK)) {
                        otec->right->color = Color::RED;
                        now = otec;
                        first = 0;
                        continue;
                    }
                    if (otec->color == Color::BLACK && otec->right->color == Color::RED &&
                        (otec->right->right == nullptr ||
                            otec->right->right->color == Color::BLACK) &&
                            (otec->right->left == nullptr ||
                                otec->right->left->color == Color::BLACK)) {
                        rotateLeft(otec->right);
                        if (!(otec->value < root->value || root->value < otec->value)) {
                            root = otec->parent;
                        }
                        otec->color = Color::RED;
                        otec->parent->color = Color::BLACK;
                        continue;
                    }
                    if (otec->color == Color::BLACK && otec->right->color == Color::BLACK &&
                        (otec->right->right != nullptr &&
                            otec->right->right->color == Color::RED) &&
                            (otec->right->left != nullptr && otec->right->left->color == Color::RED)) {
                        rotateLeft(otec->right);
                        if (!(otec->value < root->value || root->value < otec->value)) {
                            root = otec->parent;
                        }
                        otec->parent->right->color = Color::BLACK;
                        return;
                    }
                    if (otec->color == Color::RED && otec->right->color == Color::BLACK &&
                        (otec->right->right != nullptr &&
                            otec->right->right->color == Color::RED) &&
                            (otec->right->left != nullptr && otec->right->left->color == Color::RED)) {
                        rotateLeft(otec->right);
                        otec->color = Color::BLACK;
                        otec->parent->right->color = Color::BLACK;
                        otec->parent->color = Color::RED;
                        return;
                    }
                    if (otec->right->left != nullptr && otec->right->left->color == Color::RED) {
                        rotateRight(otec->right->left);
                        otec->right->color = Color::BLACK;
                        otec->right->right->color = Color::RED;
                        continue;
                    }
                    if (otec->color == Color::RED) {
                        rotateLeft(otec->right);
                        otec->color = Color::BLACK;
                        otec->parent->color = Color::RED;
                        otec->parent->right->color = Color::BLACK;
                        if (!(root->value < otec->value || otec->value < root->value)) {
                            root = otec->parent;
                        }
                        return;
                    }
                    if (otec->color == Color::BLACK) {
                        rotateLeft(otec->right);
                        otec->parent->right->color = Color::BLACK;
                        if (!(root->value < otec->value || otec->value < root->value)) {
                            root = otec->parent;
                        }
                        return;
                    }
                }
                else {
                    if (otec->color == Color::RED && otec->left->color == Color::BLACK &&
                        (otec->left->right == nullptr ||
                            otec->left->right->color == Color::BLACK) &&
                            (otec->left->left == nullptr || otec->left->left->color == Color::BLACK)) {
                        otec->left->color = Color::RED;
                        otec->color = Color::BLACK;
                        return;
                    }
                    if (otec->color == Color::BLACK && otec->left->color == Color::BLACK &&
                        (otec->left->right == nullptr ||
                            otec->left->right->color == Color::BLACK) &&
                            (otec->left->left == nullptr || otec->left->left->color == Color::BLACK)) {
                        otec->left->color = Color::RED;
                        now = otec;
                        first = 0;
                        continue;
                    }
                    if (otec->color == Color::BLACK && otec->left->color == Color::RED &&
                        (otec->left->right == nullptr ||
                            otec->left->right->color == Color::BLACK) &&
                            (otec->left->left == nullptr || otec->left->left->color == Color::BLACK)) {
                        rotateRight(otec->left);
                        if (!(otec->value < root->value || root->value < otec->value)) {
                            root = otec->parent;
                        }
                        otec->color = Color::RED;
                        otec->parent->color = Color::BLACK;
                        continue;
                    }
                    if (otec->color == Color::BLACK && otec->left->color == Color::BLACK &&
                        (otec->left->right != nullptr && otec->left->right->color == Color::RED) &&
                        (otec->left->left != nullptr && otec->left->left->color == Color::RED)) {
                        rotateRight(otec->left);
                        if (!(otec->value < root->value || root->value < otec->value)) {
                            root = otec->parent;
                        }
                        otec->parent->left->color = Color::BLACK;
                        return;
                    }
                    if (otec->color == Color::RED && otec->left->color == Color::BLACK &&
                        (otec->left->right != nullptr && otec->left->right->color == Color::RED) &&
                        (otec->left->left != nullptr && otec->left->left->color == Color::RED)) {
                        rotateRight(otec->left);
                        otec->color = Color::BLACK;
                        otec->parent->left->color = Color::BLACK;
                        otec->parent->color = Color::RED;
                        return;
                    }
                    if (otec->left->right != nullptr && otec->left->right->color == Color::RED) {
                        rotateLeft(otec->left->right);
                        otec->left->color = Color::BLACK;
                        otec->left->left->color = Color::RED;
                        continue;
                    }
                    if (otec->color == Color::RED) {
                        rotateRight(otec->left);
                        otec->color = Color::BLACK;
                        otec->parent->color = Color::RED;
                        otec->parent->left->color = Color::BLACK;
                        if (!(root->value < otec->value || otec->value < root->value)) {
                            root = otec->parent;
                        }
                        return;
                    }
                    if (otec->color == Color::BLACK) {
                        rotateRight(otec->left);
                        otec->parent->left->color = Color::BLACK;
                        if (!(root->value < otec->value || otec->value < root->value)) {
                            root = otec->parent;
                        }
                        return;
                    }
                }
            }
            low = 0;
            otec = now->parent;
            if (now->value < otec->value) {
                low = 1;
            }
            if (low) {
                if (otec->color == Color::RED && otec->right->color == Color::BLACK &&
                    (otec->right->right == nullptr || otec->right->right->color == Color::BLACK) &&
                    (otec->right->left == nullptr || otec->right->left->color == Color::BLACK)) {
                    otec->right->color = Color::RED;
                    otec->color = Color::BLACK;
                    return;
                }
                if (otec->color == Color::BLACK && otec->right->color == Color::BLACK &&
                    (otec->right->right == nullptr || otec->right->right->color == Color::BLACK) &&
                    (otec->right->left == nullptr || otec->right->left->color == Color::BLACK)) {
                    otec->right->color = Color::RED;
                    now = otec;
                    first = 0;
                    continue;
                }
                if (otec->color == Color::BLACK && otec->right->color == Color::RED &&
                    (otec->right->right == nullptr || otec->right->right->color == Color::BLACK) &&
                    (otec->right->left == nullptr || otec->right->left->color == Color::BLACK)) {
                    rotateLeft(otec->right);
                    if (!(otec->value < root->value || root->value < otec->value)) {
                        root = otec->parent;
                    }
                    otec->color = Color::RED;
                    otec->parent->color = Color::BLACK;
                    continue;
                }
                if (otec->color == Color::BLACK && otec->right->color == Color::BLACK &&
                    (otec->right->right != nullptr && otec->right->right->color == Color::RED) &&
                    (otec->right->left != nullptr && otec->right->left->color == Color::RED)) {
                    rotateLeft(otec->right);
                    if (!(otec->value < root->value || root->value < otec->value)) {
                        root = otec->parent;
                    }
                    otec->parent->right->color = Color::BLACK;
                    return;
                }
                if (otec->color == Color::RED && otec->right->color == Color::BLACK &&
                    (otec->right->right != nullptr && otec->right->right->color == Color::RED) &&
                    (otec->right->left != nullptr && otec->right->left->color == Color::RED)) {
                    rotateLeft(otec->right);
                    otec->color = Color::BLACK;
                    otec->parent->right->color = Color::BLACK;
                    otec->parent->color = Color::RED;
                    return;
                }
                if (otec->right->left != nullptr && otec->right->left->color == Color::RED) {
                    rotateRight(otec->right->left);
                    otec->right->color = Color::BLACK;
                    otec->right->right->color = Color::RED;
                    continue;
                }
                if (otec->color == Color::RED) {
                    rotateLeft(otec->right);
                    otec->color = Color::BLACK;
                    otec->parent->color = Color::RED;
                    otec->parent->right->color = Color::BLACK;
                    if (!(root->value < otec->value || otec->value < root->value)) {
                        root = otec->parent;
                    }
                    return;
                }
                if (otec->color == Color::BLACK) {
                    rotateLeft(otec->right);
                    otec->parent->right->color = Color::BLACK;
                    if (!(root->value < otec->value || otec->value < root->value)) {
                        root = otec->parent;
                    }
                    return;
                }
            }
            else {
                if (otec->color == Color::RED && otec->left->color == Color::BLACK &&
                    (otec->left->right == nullptr || otec->left->right->color == Color::BLACK) &&
                    (otec->left->left == nullptr || otec->left->left->color == Color::BLACK)) {
                    otec->left->color = Color::RED;
                    otec->color = Color::BLACK;
                    return;
                }
                if (otec->color == Color::BLACK && otec->left->color == Color::BLACK &&
                    (otec->left->right == nullptr || otec->left->right->color == Color::BLACK) &&
                    (otec->left->left == nullptr || otec->left->left->color == Color::BLACK)) {
                    otec->left->color = Color::RED;
                    now = otec;
                    first = 0;
                    continue;
                }
                if (otec->color == Color::BLACK && otec->left->color == Color::RED &&
                    (otec->left->right == nullptr || otec->left->right->color == Color::BLACK) &&
                    (otec->left->left == nullptr || otec->left->left->color == Color::BLACK)) {
                    rotateRight(otec->left);
                    if (!(otec->value < root->value || root->value < otec->value)) {
                        root = otec->parent;
                    }
                    otec->color = Color::RED;
                    otec->parent->color = Color::BLACK;
                    continue;
                }
                if (otec->color == Color::BLACK && otec->left->color == Color::BLACK &&
                    (otec->left->right != nullptr && otec->left->right->color == Color::RED) &&
                    (otec->left->left != nullptr && otec->left->left->color == Color::RED)) {
                    rotateRight(otec->left);
                    if (!(otec->value < root->value || root->value < otec->value)) {
                        root = otec->parent;
                    }
                    otec->parent->left->color = Color::BLACK;
                    return;
                }
                if (otec->color == Color::RED && otec->left->color == Color::BLACK &&
                    (otec->left->right != nullptr && otec->left->right->color == Color::RED) &&
                    (otec->left->left != nullptr && otec->left->left->color == Color::RED)) {
                    rotateRight(otec->left);
                    otec->color = Color::BLACK;
                    otec->parent->left->color = Color::BLACK;
                    otec->parent->color = Color::RED;
                    return;
                }
                if (otec->left->right != nullptr && otec->left->right->color == Color::RED) {
                    rotateLeft(otec->left->right);
                    otec->left->color = Color::BLACK;
                    otec->left->left->color = Color::RED;
                    continue;
                }
                if (otec->color == Color::RED) {
                    rotateRight(otec->left);
                    otec->color = Color::BLACK;
                    otec->parent->color = Color::RED;
                    otec->parent->left->color = Color::BLACK;
                    if (!(root->value < otec->value || otec->value < root->value)) {
                        root = otec->parent;
                    }
                    return;
                }
                if (otec->color == Color::BLACK) {
                    rotateRight(otec->left);
                    otec->parent->left->color = Color::BLACK;
                    if (!(root->value < otec->value || otec->value < root->value)) {
                        root = otec->parent;
                    }
                    return;
                }
            }
        }
    }

public:
    

    RBTree() {
        root = nullptr;
        tree_size = 0;
    }
    RBTree(std::initializer_list<ValueType> list) {
        root = nullptr;
        tree_size = 0;
        for (auto c : list) {
            insert(c);
        }
    }

    
    RBTree(const RBTree& other) {
        if (!(other.tree_size < 0 || 0 < other.tree_size)) {
            root = nullptr;
            tree_size = 0;
            return;
        }
        tree_size = other.tree_size;
        root = new Node<ValueType>;
        copydfs(root, other.root);
    }

    RBTree<ValueType>& operator=(const RBTree& other) {
        if (!(tree_size < 0 || 0 < tree_size)) {
            delete root;
        }
        else {
            del(root);
        }
        if (!(other.tree_size < 0 || 0 < other.tree_size)) {
            root = nullptr;
            tree_size = 0;
        }
        else {
            tree_size = other.tree_size;
            root = new Node<ValueType>;
            copydfs(root, other.root);
        }
        return *this;
    }

    ~RBTree() {
        if (!(tree_size < 0 || 0 < tree_size)) {
            delete root;
            return;
        }
        del(root);
    }

    

    void insert(const ValueType& value) {
        if (!(tree_size < 0 || 0 < tree_size)) {
            tree_size = 1;
            root = new Node<ValueType>;
            root->value = value;
            root->color = Color::BLACK;
            return;
        }
        Node<ValueType>* now = root;
        bool success = 0;
        while (now->value < value || value < now->value) {
            if (value < now->value) {
                if (now->left == nullptr) {
                    now->left = new Node<ValueType>;
                    now->left->value = value;
                    now->left->color = Color::RED;
                    now->left->parent = now;
                    now = now->left;
                    success = 1;
                    break;
                }
                now = now->left;
                continue;
            }
            if (now->value < value) {
                if (now->right == nullptr) {
                    now->right = new Node<ValueType>;
                    now->right->value = value;
                    now->right->color = Color::RED;
                    now->right->parent = now;
                    now = now->right;
                    success = 1;
                    break;
                }
                now = now->right;
                continue;
            }
        }
        if (!(success)) {
            return;
        }
        tree_size++;
        if (now->parent->color == Color::BLACK) {
            return;
        }
        while (now->color == Color::RED && now->parent != nullptr &&
            now->parent->color == Color::RED) {
            Color uncle = Color::BLACK;
            if (now->parent->parent->left != nullptr &&
                now->parent->value < now->parent->parent->value) {
                if (now->parent->parent->right != nullptr) {
                    uncle = now->parent->parent->right->color;
                }
                if (uncle == Color::RED) {
                    now->parent->parent->color = Color::RED;
                    now->parent->parent->left->color = Color::BLACK;
                    now->parent->parent->right->color = Color::BLACK;
                    now = now->parent->parent;
                    continue;
                }
                if (now->parent->right != nullptr && now->parent->value < now->value) {
                    rotateLeft(now);
                    now = now->left;
                }
                now->parent->color = Color::BLACK;
                now->parent->parent->color = Color::RED;
                rotateRight(now->parent);
                if (!(root->value < now->parent->right->value ||
                    now->parent->right->value < root->value)) {
                    root = now->parent;
                }
                now = now->parent;
                continue;
            }
            if (now->parent->parent->right != nullptr &&
                now->parent->parent->value < now->parent->value) {
                if (now->parent->parent->left != nullptr) {
                    uncle = now->parent->parent->left->color;
                }
                if (uncle == Color::RED) {
                    now->parent->parent->color = Color::RED;
                    now->parent->parent->right->color = Color::BLACK;
                    now->parent->parent->left->color = Color::BLACK;
                    now = now->parent->parent;
                    continue;
                }
                if (now->parent->left != nullptr && now->value < now->parent->value) {
                    rotateRight(now);
                    now = now->right;
                }
                now->parent->color = Color::BLACK;
                now->parent->parent->color = Color::RED;
                rotateLeft(now->parent);
                if (!(root->value < now->parent->left->value ||
                    now->parent->left->value < root->value)) {
                    root = now->parent;
                }
                now = now->parent;
            }
        }
        root->color = Color::BLACK;
    }

    Node<ValueType>* search(Node<ValueType>* root, const ValueType& value) {
        Node<ValueType>* now = root;
        while (now->value < value || value < now->value) {
            if (value < now->value) {
                now = now->left;
                continue;
            }
            now = now->right;
        }
        return now;
    }

    void erase(const ValueType& value) {
        if (find(value).konec) {
            return;
        }
        if (!(tree_size < 1 || 1 < tree_size)) {
            Node<ValueType>* now = root;
            root = nullptr;
            delete now;
            tree_size = 0;
            return;
        }
        Node<ValueType>* now = search(root, value);
        getRid(now);
    }

    size_t size() const {
        return tree_size;
    }
    bool empty() const {
        return !(tree_size < 0 || 0 < tree_size);
    }

    Iterator lowerBound(const ValueType& value) const {
        if (tree_size == 0) {
            return end();
        }
        Node<ValueType>* now = root;
        Node<ValueType>* ans = now;
        bool success = 0;
        while (now->value < value || value < now->value) {
            if (value < now->value) {
                if (success == false || now->value < ans->value) {
                    ans = now;
                }
                success = 1;
                if (now->left == nullptr) {
                    break;
                }
                now = now->left;
                continue;
            }
            if (now->value < value) {
                if (now->right == nullptr) {
                    break;
                }
                now = now->right;
            }
        }
        if ((now->value < value || value < now->value) && !success) {
            return end();
        }
        if (!(now->value < value || value < now->value)) {
            return Iterator(now);
        }
        return Iterator(ans);
    }
    Iterator find(const ValueType& value) const {
        if (tree_size == 0) {
            return end();
        }
        Node<ValueType>* now = root;
        bool success = 1;
        while (now->value < value || value < now->value) {
            if (value < now->value) {
                if (now->left == nullptr) {
                    success = 0;
                    break;
                }
                now = now->left;
                continue;
            }
            if (now->value < value) {
                if (now->right == nullptr) {
                    success = 0;
                    break;
                }
                now = now->right;
            }
        }
        if (!(success)) {
            return end();
        }
        return Iterator(now);
    }

    Iterator begin() const {
        if (!(tree_size < 0 || 0 < tree_size)) {
            return Iterator(nullptr);
        }
        if (root == nullptr) {
            return Iterator(nullptr);
        }
        if (root->left == nullptr) {
            return Iterator(root);
        }
        Node<ValueType>* now = root;
        while (now->left != nullptr) {
            now = now->left;
        }
        return Iterator(now);
    }
    Iterator end() const {
        if (!(tree_size < 0 || 0 < tree_size)) {
            Iterator ans = Iterator(nullptr);
            ans.konec = true;
            return ans;
        }
        if (root == nullptr) {
            Iterator ans = Iterator(nullptr);
            ans.konec = true;
            return ans;
        }
        if (root->right == nullptr) {
            Iterator ans = Iterator(root);
            ans.konec = true;
            return ans;
        }
        Node<ValueType>* now = root;
        while (now->right != nullptr) {
            now = now->right;
        }
        Iterator ans = Iterator(now);
        ans.konec = true;
        return ans;
    }
};
