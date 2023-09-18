#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
                : value(val)
                , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;

        BasicIterator& operator=(const BasicIterator& rhs) = default;

        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) {}

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return node_ == rhs.node_;
        }
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        BasicIterator& operator++() noexcept {
            assert(node_);
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
            auto tmp = *this;
            ++(*this);
            return tmp;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_);
            return node_->value;
        }
        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_);
            return &node_->value;
        }

    private:
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) : node_(node) {}

    private:
        Node* node_ = nullptr;
    };

public:
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;


    [[nodiscard]] Iterator begin() const noexcept {
        return Iterator{head_.next_node};
    }

    [[nodiscard]] Iterator end() const noexcept {
        return Iterator{nullptr};
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{head_.next_node};
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator{nullptr};
    }

    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{&head_};
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{&head_};
    }

    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator{&head_};
    }

public:
    SingleLinkedList() : head_() {}

    SingleLinkedList(std::initializer_list<Type> values) {
        Construct(values);
    }

    SingleLinkedList(const SingleLinkedList& other) {
        Construct(other);
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        SingleLinkedList tmp(rhs);
        swap(tmp);
        return *this;
    }


    void swap(SingleLinkedList& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(head_.next_node, other.head_.next_node);
    }

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        assert(pos.node_);
        pos.node_->next_node = new Node(value, pos.node_->next_node);

        ++size_;
        return Iterator{pos.node_->next_node};
    }

    void PopFront() noexcept {
        assert(size_ > 0);
        auto* next_node = head_.next_node;
        head_.next_node = next_node->next_node;
        --size_;
        delete next_node;
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(pos.node_);
        assert(pos.node_->next_node);
        assert(size_ > 0);
        auto* next_node = pos.node_->next_node;
        pos.node_->next_node = next_node->next_node;
        delete next_node;
        --size_;
        return Iterator{pos.node_->next_node};
    }

    void Clear() noexcept {
        while (!IsEmpty()){
            PopFront();
        }
    }

    ~SingleLinkedList(){
        Clear();
    }

private:
    template<class Container>
    void Construct(const Container& values){
        SingleLinkedList tmp;

        auto it = tmp.before_begin();
        for(const auto& val: values){
            it = tmp.InsertAfter(it, val);
        }
        swap(tmp);
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    mutable Node head_;
    size_t size_ = 0u;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if(&lhs == &rhs){
        return true;
    }
    if(lhs.GetSize() != rhs.GetSize()){
        return false;
    }
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs <= rhs);
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);
}