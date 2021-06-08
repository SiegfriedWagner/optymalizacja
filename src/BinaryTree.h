//
// Created by mateu on 04.05.2021.
//

#ifndef OPTYMALIZACJA_BINARYTREE_H
#define OPTYMALIZACJA_BINARYTREE_H
#include <memory>
namespace btree {
    template<typename Data>
    class BTreeNode {
    private:
        std::weak_ptr<BTreeNode<Data>> parent;
        std::shared_ptr<BTreeNode<Data>> left;
        std::shared_ptr<BTreeNode<Data>> right;
        Data data;
    public:
        BTreeNode();
        explicit BTreeNode(Data data);
        explicit BTreeNode(Data &data);
        BTreeNode(BTreeNode<Data> &&node) noexcept;
        Data &GetData();
        const Data &GetData() const;
        std::shared_ptr<BTreeNode<Data>> GetLeft() const;
        std::shared_ptr<BTreeNode<Data>> GetRight() const;
        std::shared_ptr<BTreeNode<Data>> GetParent() const;
        static void SetLeft(std::shared_ptr<BTreeNode<Data>> parent, std::shared_ptr<BTreeNode<Data>> node);
        static void SetRight(std::shared_ptr<BTreeNode<Data>> parent, std::shared_ptr<BTreeNode<Data>> node);
        [[nodiscard]] bool HasParent() const;
        [[nodiscard]] bool IsLeftChild() const;
        [[nodiscard]] bool IsRightChild() const;
        operator Data&() { return data; }
    };
    template<typename Data>
    BTreeNode<Data>::BTreeNode() : left(nullptr), right(nullptr), data(Data()) {}
    template<typename Data>
    BTreeNode<Data>::BTreeNode(Data data) : left(nullptr), right(nullptr), data(data) {}

    template<typename Data>
    BTreeNode<Data>::BTreeNode(Data &data) : left(nullptr), right(nullptr), data(data) {}

    template<typename Data>
    BTreeNode<Data>::BTreeNode(BTreeNode<Data> &&node) noexcept : left(node.left), right(node.right), data(std::move(node.data)) {}

    template<typename Data>
    inline Data & BTreeNode<Data>::GetData() { return data; }

    template<typename Data>
    inline const Data & BTreeNode<Data>::GetData() const { return data; }

    template<typename Data>
    inline std::shared_ptr<BTreeNode<Data>> BTreeNode<Data>::GetLeft() const { return left; }

    template<typename Data>
    inline std::shared_ptr<BTreeNode<Data>> BTreeNode<Data>::GetRight() const { return right; }

    template<typename Data>
    inline std::shared_ptr<BTreeNode<Data>> BTreeNode<Data>::GetParent() const {
        if(HasParent())
            return parent.lock();
        else
            return nullptr;
    }

    template<typename Data>
    void BTreeNode<Data>::SetLeft(std::shared_ptr<btree::BTreeNode<Data>> parent,
                                  std::shared_ptr<btree::BTreeNode<Data>> node) {
        parent->left = node;
        if (node != nullptr)
            node->parent = parent;
    }

    template<typename Data>
    void BTreeNode<Data>::SetRight(std::shared_ptr<btree::BTreeNode<Data>> parent,
                                  std::shared_ptr<btree::BTreeNode<Data>> node) {
        parent->right = node;
        if (node != nullptr)
            node->parent = parent;
    }

    template<typename Data>
    inline bool BTreeNode<Data>::HasParent() const { return !parent.expired(); }

    template<typename Data>
    bool BTreeNode<Data>::IsLeftChild() const {
        if(HasParent())
        {
            auto parent_l = parent.lock();
            if (parent_l->left.get() == this)
                return true;
        }
        return false;
    }

    template<typename Data>
    bool BTreeNode<Data>::IsRightChild() const {
        if(HasParent())
        {
            auto parent_l = parent.lock();
            if (parent_l->right.get() == this)
                return true;
        }
        return false;
    }

}
#endif //OPTYMALIZACJA_BINARYTREE_H
