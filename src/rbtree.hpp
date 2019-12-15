////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief     Реализация классов красно-черного дерева
/// \author    Sergey Shershakov
/// \version   0.1.0
/// \date      01.05.2017
///            This is a part of the course "Algorithms and Data Structures" 
///            provided by  the School of Software Engineering of the Faculty 
///            of Computer Science at the Higher School of Economics.
///
/// "Реализация" (шаблонов) методов, описанных в файле rbtree.h
///
////////////////////////////////////////////////////////////////////////////////

#include <stdexcept>        // std::invalid_argument
#include "rbtree.h"

using namespace std;

namespace xi {


//==============================================================================
// class RBTree::node
//==============================================================================

template <typename Element, typename Compar >
RBTree<Element, Compar>::Node::~Node()
{
    if (_left)
        delete _left;
    if (_right)
        delete _right;
}



template <typename Element, typename Compar>
typename RBTree<Element, Compar>::Node* RBTree<Element, Compar>::Node::setLeft(Node* lf)
{
    // предупреждаем повторное присвоение
    if (_left == lf)
        return nullptr;

    // если новый левый — действительный элемент
    if (lf)
    {
        // если у него был родитель
        if (lf->_parent)
        {
            // ищем у родителя, кем был этот элемент, и вместо него ставим бублик
            if (lf->_parent->_left == lf)
                lf->_parent->_left = nullptr;
            else                                    // доп. не проверяем, что он был правым, иначе нарушение целостности
                lf->_parent->_right = nullptr;      
        }

        // задаем нового родителя
        lf->_parent = this;
    }

    // если у текущего уже был один левый — отменяем его родительскую связь и вернем его
    Node* prevLeft = _left;
    _left = lf;

    if (prevLeft)
        prevLeft->_parent = nullptr;

    return prevLeft;
}


template <typename Element, typename Compar>
typename RBTree<Element, Compar>::Node* RBTree<Element, Compar>::Node::setRight(Node* rg)
{
    // предупреждаем повторное присвоение
    if (_right == rg)
        return nullptr;

    // если новый правый — действительный элемент
    if (rg)
    {
        // если у него был родитель
        if (rg->_parent)
        {
            // ищем у родителя, кем был этот элемент, и вместо него ставим бублик
            if (rg->_parent->_left == rg)
                rg->_parent->_left = nullptr;
            else                                    // доп. не проверяем, что он был правым, иначе нарушение целостности
                rg->_parent->_right = nullptr;
        }

        // задаем нового родителя
        rg->_parent = this;
    }

    // если у текущего уже был один левый — отменяем его родительскую связь и вернем его
    Node* prevRight = _right;
    _right = rg;

    if (prevRight)
        prevRight->_parent = nullptr;

    return prevRight;
}


//==============================================================================
// class RBTree
//==============================================================================

template <typename Element, typename Compar >
RBTree<Element, Compar>::RBTree()
{
    _root = nullptr;
    _dumper = nullptr;
}

template <typename Element, typename Compar >
RBTree<Element, Compar>::~RBTree()
{
    // грохаем пока что всех через корень
    if (_root)
        delete _root;
}


template <typename Element, typename Compar >
void RBTree<Element, Compar>::deleteNode(Node* nd)
{
    // если переданный узел не существует, просто ничего не делаем, т.к. в вызывающем проверок нет
    if (nd == nullptr)
        return;

    // потомков убьет в деструкторе
    delete nd;
}


template <typename Element, typename Compar >
void RBTree<Element, Compar>::insert(const Element& key)
{
    // этот метод можно оставить студентам целиком
    Node* newNode = insertNewBstEl(key);

    // отладочное событие
    if (_dumper)
        _dumper->rbTreeEvent(IRBTreeDumper<Element, Compar>::DE_AFTER_BST_INS, this, newNode);

    rebalance(newNode);

    // отладочное событие
    if (_dumper)
        _dumper->rbTreeEvent(IRBTreeDumper<Element, Compar>::DE_AFTER_INSERT, this, newNode);

}




template <typename Element, typename Compar>
const typename RBTree<Element, Compar>::Node* RBTree<Element, Compar>::find(const Element& key)
{
    Node* extra = _root;
    while(extra != nullptr)
    {
        if (key < extra->_key)
            extra = extra->_left;
        else if (key > extra->_key)
            extra = extra->_right;
        else
            return extra;
    }
    return nullptr;
}

template <typename Element, typename Compar >
typename RBTree<Element, Compar>::Node* 
        RBTree<Element, Compar>::insertNewBstEl(const Element& key)
{
    Node* insertNode = new Node(key);
    Node* extra = _root;
    Node* extraFather = nullptr;
    while (extra != nullptr)
    {
        extraFather = extra;
        if (insertNode->_key < extra->_key)
            extra = extra->_left;
        else if (insertNode->_key > extra->_key)
            extra = extra->_right;
        else
            throw invalid_argument("Keys are equal!");
    }
    insertNode->_parent = extraFather;
    if (insertNode->_key < extraFather->_key)
        extraFather->_left = insertNode;
    else if (extraFather == nullptr)
        _root = insertNode;
    else
        extraFather->_right = insertNode;
    insertNode->setRed();
    return insertNode;
}


template <typename Element, typename Compar >
typename RBTree<Element, Compar>::Node* 
    RBTree<Element, Compar>::rebalanceDUG(Node* nd)
{

    // TODO: этот метод студенты могут оставить и реализовать при декомпозиции балансировки дерева
    // В методе оставлены некоторые важные комментарии/snippet-ы


    // попадание в этот метод уже означает, что папа есть (а вот про дедушку пока не известно)
    //...

    Node* uncle = nd->_parent->_parent->getChild(!nd->_parent->isLeftChild()); // для левого случая нужен правый дядя и наоборот.

    // если дядя такой же красный, как сам нод и его папа...
    if (uncle->isRed() && uncle != nullptr)
    {
        // дядю и папу красим в черное
        // а дедушку — в коммунистические цвета
        uncle->setBlack();
        nd->_parent->setBlack();
        uncle->_parent->setRed();

        nd = uncle->_parent;

        // отладочное событие
        if (_dumper)
            _dumper->rbTreeEvent(IRBTreeDumper<Element, Compar>::DE_AFTER_RECOLOR1, this, nd);

        // теперь чередование цветов "узел-папа-дедушка-дядя" — К-Ч-К-Ч, но надо разобраться, что там
        // с дедушкой и его предками, поэтому продолжим с дедушкой
        //..
    }
    else
    {

        if (nd->isLeftChild() && nd->_parent->isRightChild())
        {
            nd = nd->_parent;
            rotRight(nd);
        }
        else if ( nd->isRightChild() && nd->_parent->isLeftChild())
        {
            nd = nd->_parent;
            rotLeft(nd);
        }

        nd->_parent->setBlack();
        if (_dumper)
            _dumper->rbTreeEvent(IRBTreeDumper<Element, Compar>::DE_AFTER_RECOLOR3D, this, nd);

        nd->_parent->_parent->setRed();
        if (_dumper)
            _dumper->rbTreeEvent(IRBTreeDumper<Element, Compar>::DE_AFTER_RECOLOR3G, this, nd);

        if (nd->_parent->isLeftChild())
            rotRight(nd->_parent->_parent);
        else
            rotLeft(nd->_parent->_parent);
    }
    return nd;
}


template <typename Element, typename Compar >
void RBTree<Element, Compar>::rebalance(Node* nd)
{
   if (nd->isBlack())
       return;

    // пока папа — цвета пионерского галстука, действуем
    while (nd->isDaddyRed())
    {
        // локальная перебалансировка семейства "папа, дядя, дедушка" и повторная проверка
        nd = rebalanceDUG(nd);
    }
    _root->setBlack();
}



template <typename Element, typename Compar>
void RBTree<Element, Compar>::rotLeft(typename RBTree<Element, Compar>::Node* nd)
{
    // TODO: метод реализуют студенты

    // правый потомок, который станет после левого поворота "выше"
    Node* y = nd->_right;
    
    if (!y)
        throw std::invalid_argument("Can't rotate left since the right child is nil");
    else if (nd->isRightChild())
        nd->_parent->setRight(y);
    else if (nd->isLeftChild())
        nd->_parent->setLeft(y);
    else
        _root = y;
    nd->setRight(y->_left);
    y->setLeft(nd);

    // отладочное событие
    if (_dumper)
        _dumper->rbTreeEvent(IRBTreeDumper<Element, Compar>::DE_AFTER_LROT, this, nd);
}



template <typename Element, typename Compar>
void RBTree<Element, Compar>::rotRight(typename RBTree<Element, Compar>::Node* nd)
{
    Node* y = nd->_left;
    if (y == nullptr)
        throw invalid_argument ("Left child is nill!");
    else if (nd->isRightChild())
        nd->_parent->setRight(y);
    else if (nd->isLeftChild())
        nd->_parent->setLeft(y);
    else
        _root = y;
    nd->setLeft(y->_right);
    y->setRight(nd);

    // отладочное событие
    if (_dumper)
        _dumper->rbTreeEvent(IRBTreeDumper<Element, Compar>::DE_AFTER_RROT, this, nd);
}

    template<typename Element, typename Compar>
    void RBTree<Element, Compar>::remove(const Element &key)
    {
        Element e = key;

        Node* delNode = _root;
        while(delNode != nullptr)
        {
            if (key < delNode->_key)
                delNode = delNode->_left;
            else if (key > delNode->_key)
                delNode = delNode->_right;
            else
                return ;
        }
        //Node* delNode = find(key);
        Node* extra1;
        Node* extra2;
        if (delNode == nullptr)
            return;
        else if (delNode->_left == nullptr || delNode->_right == nullptr)
            extra2 = delNode;
        else
        {
            extra2 = delNode->_right;
            while (extra2->_left != nullptr)
                extra2 = extra2->_left;
        }
        if(extra2->_left != nullptr)
            extra1 = extra2->_left;
        else
            extra1 = extra2->_right;
        extra1->_parent = extra2->_parent;
        if (extra2->_parent != nullptr)
        {
            if(extra2 == extra2->_parent->_left)
                extra2->_parent->_left = extra1;
            else
                extra2->_parent->_right = extra1;
        }
        else
            _root = extra1;
        if(extra2 != delNode)
            delNode->_key = extra2->_key;
        if (extra2->isBlack())
            deleteExtra(extra1);
        deleteNode(extra2);
    }

    template<typename Element, typename Compar>
    void RBTree<Element, Compar>::deleteExtra(RBTree::Node *x)
    {
        while (x != _root && x->isBlack()) {
            if (x == x->_parent->_left) {
                Node *w = x->_parent->_right;
                if (w->isRed()) {
                    w->setBlack();
                    x->_parent->setRed();
                    rotLeft (x->_parent);
                    w = x->_parent->_right;
                }
                if (w->_left->isBlack() && w->_right->isBlack()) {
                    w->setRed();
                    x = x->_parent;
                } else {
                    if (w->_right->isBlack()) {
                        w->_left->setBlack();
                        w->setRed();
                        rotRight (w);
                        w = x->_parent->_right;
                    }
                    w->_color = x->_parent->_color;
                    x->_parent->setBlack();
                    w->_right->setBlack();
                    rotLeft (x->_parent);
                    x = _root;
                }
            } else {
                Node *w = x->_parent->_left;
                if (w->isRed()) {
                    w->setBlack();
                    x->_parent->setRed();
                    rotRight (x->_parent);
                    w = x->_parent->_left;
                }
                if (w->_right->isBlack() && w->_left->isBlack()) {
                    w->setRed();
                    x = x->_parent;
                } else {
                    if (w->_left->isBlack()) {
                        w->_right->setBlack();
                        w->setRed();
                        rotLeft (w);
                        w = x->_parent->_left;
                    }
                    w->_color = x->_parent->_color;
                    x->_parent->setBlack();
                    w->_left->setBlack();
                    rotRight (x->_parent);
                    x = _root;
                }
            }
        }
        x->setBlack();
    }


} // namespace xi

