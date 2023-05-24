#include <iostream>
#include <utility>
enum Color { RED, BLACK };

template <typename K, typename V>
struct Node {
    K key;
    V value;
    Node<K, V>* left;
    Node<K, V>* right;
    Node<K, V>* parent;
    Color color;
    std::pair<K, V> para;
    Node() {
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
    }
    Node(const K& key, const V& value) {
        this->left = nullptr;
        this->right = nullptr;
        this->parent = nullptr;
        this->key = key;
        this->value = value;
        para.first = key;
        para.second = value;
    }
};

template <typename K, typename V>
class Map {
private:
    Node<K, V>* root;
    size_t tree_size = 0;
    struct Iterator {
        Iterator() {
            m_ptr_ = nullptr;
        }
        explicit Iterator(Node<K, V>* node) {
            m_ptr_ = node;
        }

        const std::pair<K, V>& operator*() const {
            return m_ptr_->para;
        }
        const std::pair<K, V>* operator->() const {
            return &(m_ptr_->para);
        }

        Iterator& operator++() {
            K mx;
            Node<K, V>* now = m_ptr_;
            while (now->parent != nullptr) {
                now = now->parent;
            }
            while (now->right != nullptr) {
                now = now->right;
            }
            if (!(now->key < m_ptr_->key || m_ptr_->key < now->key)) {
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
            Node<K, V>* otec = m_ptr_->parent;
            now = m_ptr_;
            while (otec->right != nullptr &&
                !(now->key < otec->right->key || otec->right->key < now->key)) {
                now = otec;
                otec = otec->parent;
            }
            m_ptr_ = otec;
            return *this;
        }
        Iterator operator++(int) {
            Iterator ans = *this;
            K mx;
            Node<K, V>* now = m_ptr_;
            while (now->parent != nullptr) {
                now = now->parent;
            }
            while (now->right != nullptr) {
                now = now->right;
            }
            if (!(now->key < m_ptr_->key || m_ptr_->key < now->key)) {
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
                Node<K, V>* otec = m_ptr_->parent;
                now = m_ptr_;
                while (otec->right != nullptr &&
                    !(now->key < otec->right->key || otec->right->key < now->key)) {
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
                Node<K, V>* now = m_ptr_->left;
                while (now->right != nullptr) {
                    now = now->right;
                }
                m_ptr_ = now;
                return *this;
            }
            Node<K, V>* otec = m_ptr_->parent;
            Node<K, V>* now = m_ptr_;
            while (otec->left != nullptr &&
                !(now->key < otec->left->key || otec->left->key < now->key)) {
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
                Node<K, V>* now = m_ptr_->left;
                while (now->right != nullptr) {
                    now = now->right;
                }
                m_ptr_ = now;
            }
            else {
                Node<K, V>* otec = m_ptr_->parent;
                Node<K, V>* now = m_ptr_;
                while (otec->left != nullptr &&
                    !(now->key < otec->left->key || otec->left->key < now->key)) {
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
        Node<K, V>* m_ptr_;
        bool konec = 0;
    };
    void del(Node<K, V>* v) {
        if (v->left != nullptr) {
            del(v->left);
        }
        if (v->right != nullptr) {
            del(v->right);
        }
        delete v;
    }

    void copydfs(Node<K, V>* now, Node<K, V>* origin) {
        now->color = origin->color;
        now->value = origin->value;
        now->key = origin->key;
        if (origin->left != nullptr) {
            now->left = new Node<K, V>;
            now->left->parent = now;
            copydfs(now->left, origin->left);
        }
        if (origin->right != nullptr) {
            now->right = new Node<K, V>;
            now->right->parent = now;
            copydfs(now->right, origin->right);
        }
    }

    Node<K, V>* rotateLeft(Node<K, V>* now) {
        now->parent->right = now->left;
        if (now->left != nullptr) {
            now->left->parent = now->parent;
        }
        now->left = now->parent;
        Node<K, V>* fic_node = now->parent->parent;
        now->parent->parent = now;
        if (fic_node != nullptr) {
            if (fic_node->left != nullptr && now->parent->key < fic_node->key) {
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

    Node<K, V>* rotateRight(Node<K, V>* now) {
        now->parent->left = now->right;
        if (now->right != nullptr) {
            now->right->parent = now->parent;
        }
        now->right = now->parent;
        Node<K, V>* fic_node = now->parent->parent;
        now->parent->parent = now;
        if (fic_node != nullptr) {
            if (fic_node->left != nullptr && now->parent->key < fic_node->key) {
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

    void getRid(Node<K, V>* now) {
        if (now->left != nullptr && now->right != nullptr) {
            Node<K, V>* last = now->left;
            while (last->right != nullptr) {
                last = last->right;
            }
            std::pair<K, V> res = last->para;
            getRid(last);
            now->key = res.first;
            now->value = res.second;
            now->para.first = res.first;
            now->para.second = res.second;
            return;
        }
        if (now->left != nullptr && now->right == nullptr) {
            if (!(now->key < root->key || root->key < now->key)) {
                root = now->left;
                root->color = Color::BLACK;
                root->parent = nullptr;
                delete now;
                tree_size--;
                return;
            }
            if (now->key < now->parent->key) {
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
            if (!(now->key < root->key || root->key < now->key)) {
                root = now->right;
                root->color = Color::BLACK;
                root->parent = nullptr;
                delete now;
                tree_size--;
                return;
            }
            if (now->key < now->parent->key) {
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
            if (now->key < now->parent->key) {
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
        Node<K, V>* otec = now->parent;
        bool low = 0;
        if (now->key < otec->key) {
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
            if (!first && !(now->key < root->key || root->key < now->key)) {
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
                        if (!(otec->key < root->key || root->key < otec->key)) {
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
                        if (!(otec->key < root->key || root->key < otec->key)) {
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
                        if (!(root->key < otec->key || otec->key < root->key)) {
                            root = otec->parent;
                        }
                        return;
                    }
                    if (otec->color == Color::BLACK) {
                        rotateLeft(otec->right);
                        otec->parent->right->color = Color::BLACK;
                        if (!(root->key < otec->key || otec->key < root->key)) {
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
                        if (!(otec->key < root->key || root->key < otec->key)) {
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
                        if (!(otec->key < root->key || root->key < otec->key)) {
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
                        if (!(root->key < otec->key || otec->key < root->key)) {
                            root = otec->parent;
                        }
                        return;
                    }
                    if (otec->color == Color::BLACK) {
                        rotateRight(otec->left);
                        otec->parent->left->color = Color::BLACK;
                        if (!(root->key < otec->key || otec->key < root->key)) {
                            root = otec->parent;
                        }
                        return;
                    }
                }
            }
            low = 0;
            otec = now->parent;
            if (now->key < otec->key) {
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
                    if (!(otec->key < root->key || root->key < otec->key)) {
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
                    if (!(otec->key < root->key || root->key < otec->key)) {
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
                    if (!(root->key < otec->key || otec->key < root->key)) {
                        root = otec->parent;
                    }
                    return;
                }
                if (otec->color == Color::BLACK) {
                    rotateLeft(otec->right);
                    otec->parent->right->color = Color::BLACK;
                    if (!(root->key < otec->key || otec->key < root->key)) {
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
                    if (!(otec->key < root->key || root->key < otec->key)) {
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
                    if (!(otec->key < root->key || root->key < otec->key)) {
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
                    if (!(root->key < otec->key || otec->key < root->key)) {
                        root = otec->parent;
                    }
                    return;
                }
                if (otec->color == Color::BLACK) {
                    rotateRight(otec->left);
                    otec->parent->left->color = Color::BLACK;
                    if (!(root->key < otec->key || otec->key < root->key)) {
                        root = otec->parent;
                    }
                    return;
                }
            }
        }
    }

public:
    Map() {
        root = nullptr;
        tree_size = 0;
    }
    Map(std::initializer_list<std::pair<K, V>> list) {
        root = nullptr;
        tree_size = 0;
        for (auto c : list) {
            insert(c.first, c.second);
        }
    }
    
    Map(const Map& other) {
        if (!(other.tree_size < 0 || 0 < other.tree_size)) {
            root = nullptr;
            tree_size = 0;
            return;
        }
        tree_size = other.tree_size;
        root = new Node<K, V>;
        copydfs(root, other.root);
    }

    Map<K, V>& operator=(const Map& other) {
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
            root = new Node<K, V>;
            copydfs(root, other.root);
        }
        return *this;
    }

    ~Map() {
        if (!(tree_size < 0 || 0 < tree_size)) {
            delete root;
            return;
        }
        del(root);
    }

    Node<K, V>* search(Node<K, V>* root, const K& key) {
        Node<K, V>* now = root;
        while (now->key < key || key < now->key) {
            if (key < now->key) {
                now = now->left;
                continue;
            }
            now = now->right;
        }
        return now;
    }

    void insert(const K& key, const V& value) {
        if (!(tree_size < 0 || 0 < tree_size)) {
            tree_size = 1;
            root = new Node<K, V>;
            root->value = value;
            root->key = key;
            root->para.first = key;
            root->para.second = value;
            root->color = Color::BLACK;
            return;
        }
        Node<K, V>* now = root;
        bool success = 0;
        while (now->key < key || key < now->key) {
            if (key < now->key) {
                if (now->left == nullptr) {
                    now->left = new Node<K, V>;
                    now->left->value = value;
                    now->left->key = key;
                    now->left->para.first = key;
                    now->left->para.second = value;
                    now->left->color = Color::RED;
                    now->left->parent = now;
                    now = now->left;
                    success = 1;
                    break;
                }
                now = now->left;
                continue;
            }
            if (now->key < key) {
                if (now->right == nullptr) {
                    now->right = new Node<K, V>;
                    now->right->value = value;
                    now->right->key = key;
                    now->right->para.first = key;
                    now->right->para.second = value;
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
        if (!success && !(now->key < key || key < now->key)) {
            now->value = value;
            now->para.second = value;
            return;
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
                now->parent->key < now->parent->parent->key) {
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
                if (now->parent->right != nullptr && now->parent->key < now->key) {
                    rotateLeft(now);
                    now = now->left;
                }
                now->parent->color = Color::BLACK;
                now->parent->parent->color = Color::RED;
                rotateRight(now->parent);
                if (!(root->key < now->parent->right->key || now->parent->right->key < root->key)) {
                    root = now->parent;
                }
                now = now->parent;
                continue;
            }
            if (now->parent->parent->right != nullptr &&
                now->parent->parent->key < now->parent->key) {
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
                if (now->parent->left != nullptr && now->key < now->parent->key) {
                    rotateRight(now);
                    now = now->right;
                }
                now->parent->color = Color::BLACK;
                now->parent->parent->color = Color::RED;
                rotateLeft(now->parent);
                if (!(root->key < now->parent->left->key || now->parent->left->key < root->key)) {
                    root = now->parent;
                }
                now = now->parent;
            }
        }
        root->color = Color::BLACK;
    }
    

    void erase(const K& key) {
        if (find(key).konec) {
            return;
        }
        if (!(tree_size < 1 || 1 < tree_size)) {
            Node<K, V>* now = root;
            root = nullptr;
            delete now;
            tree_size = 0;
            return;
        }
        Node<K, V>* now = search(root, key);
        getRid(now);
    }

    size_t size() const {
        return tree_size;
    }
    bool empty() const {
        return !(tree_size < 0 || 0 < tree_size);
    }

    Iterator lowerBound(const K& key) const {
        if (tree_size == 0) {
            return end();
        }
        Node<K, V>* now = root;
        Node<K, V>* ans = now;
        bool success = 0;
        while (now->key < key || key < now->key) {
            if (key < now->key) {
                if (success == false || now->key < ans->key) {
                    ans = now;
                }
                success = 1;
                if (now->left == nullptr) {
                    break;
                }
                now = now->left;
                continue;
            }
            if (now->key < key) {
                if (now->right == nullptr) {
                    break;
                }
                now = now->right;
            }
        }
        if ((now->key < key || key < now->key) && !success) {
            return end();
        }
        if (!(now->key < key || key < now->key)) {
            return Iterator(now);
        }
        return Iterator(ans);
    }
    Iterator find(const K& key) const {
        if (tree_size == 0) {
            return end();
        }
        Node<K, V>* now = root;
        bool success = 1;
        while (now->key < key || key < now->key) {
            if (key < now->key) {
                if (now->left == nullptr) {
                    success = 0;
                    break;
                }
                now = now->left;
                continue;
            }
            if (now->key < key) {
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
        Node<K, V>* now = root;
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
        Node<K, V>* now = root;
        while (now->right != nullptr) {
            now = now->right;
        }
        Iterator ans = Iterator(now);
        ans.konec = true;
        return ans;
    }
};
