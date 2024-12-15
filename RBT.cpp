#include <iostream>
using namespace std;

enum Color { RED, BLACK };

class Node {
public:
    int data;
    Color color;
    Node *left , *right , *parent;

    // Default Constructor
    Node(){
        this -> data = 0;
        this -> color = RED;
        this -> left = nullptr;
        this -> right = nullptr;
        this -> parent = nullptr;
    }

    // Parameterized Constructor
    Node(int data){
        this -> data = data;
        this -> color = RED;
        this -> left = nullptr;
        this -> right = nullptr;
        this -> parent = nullptr;
    }

    void setLeft(Node *x){
        this -> left = x;

        if(x != nullptr)
            x ->parent = this;
    }

    void setRight(Node *x){
        this ->right = x;

        if(x != nullptr)
            x ->parent = this;
    }

    void setParent(Node *x){
        this ->parent = x;
    }

    Node* getLeft(){
        return this ->left;
    }

    Node* getRight(){
        return this ->right;
    }

    Node* getParent(){
        return this ->parent;
    }

    Node* getSibling(){
        if(this ->parent == nullptr)
            return nullptr;
        if(this == this ->parent->left) // if i was left child, i will return right child
            return this->parent->right;
        else
            return this->parent->left; // return left child because i'm the right child
    }

    void changeColor(){
        if(this -> color == RED)
            this ->color = BLACK;
        else
            this ->color =RED;
    }

    void swapData(Node *x , Node *y){
        int temp = x ->data;
        x ->data = y ->data;
        y ->data = temp;
    }

    bool isLeaf() {
        return (this->left == nullptr && this->right == nullptr);
    }

    bool isRed() {
        return this->color == RED;
    }

    bool isBlack() {
        return this->color == BLACK;
    }

    void setBlack(){
        this ->color = BLACK;
    }

    void setRed(){
        this ->color = RED;
    }
};

class RBT{
private:
    Node *root;

    void leftRotate(Node* x){
        Node *z = x->getRight();

        if (z == nullptr) return; // can't rotate if z is null

        // Update x's right child to z's left child
        x->setRight(z->getLeft());


        if(z->getLeft() != nullptr){
            z->getLeft()->setParent(x); // new parent for z's left is x
        }

        z->setParent(x->getParent()); // parent for x becomes parent for z

        if(x->getParent() != nullptr){  // x not thr root
            if(x == x->getParent()->getLeft()){ // if x is left child make z left child
                x->getParent()->setLeft(z);
            }
            else{
                x->getParent()->setRight(z); // if x is right child make z right child
            }
        }
        else{ // x is the root
            root = z;
        }

        z->setLeft(x); // left node for z becomes x
        x->setParent(z);
    }

    void rightRotate(Node* x){
        Node *z = x->getLeft();

        if (z == nullptr) return; // can't rotate if z is null

        // Update x's left child to z's right child
        x->setLeft(z->getRight());


        if(z->getRight() != nullptr){
            z->getRight()->setParent(x); // new parent for z's right is x
        }

        z->setParent(x->getParent()); // parent for x becomes parent for z

        if(x->getParent() != nullptr){  // x not thr root
            if(x == x->getParent()->getLeft()){ // if x is left child make z left child
                x->getParent()->setLeft(z);
            }
            else{
                x->getParent()->setRight(z); // if x is right child make z right child
            }
        }
        else{ // x is the root
            root = z;
        }

        z->setRight(x); // right node for z becomes x
        x->setParent(z);
    }

    // insert nodes as BST
    void insertBST(Node* root, Node* newNode){
        // lw new value as8r, go to left part
        if(newNode->data < root->data){
            // lw el left fady 7ot el value feh
            if(root->getLeft() == nullptr)
                root->setLeft(newNode);
            // lw la call function again
            else
                insertBST(root->getLeft(), newNode);
        }

        // lw new value akbr, go to right part
        else{
            // lw el right fady 7ot el value feh
            if(root->getRight() == nullptr)
                root->setRight(newNode);
            // lw la call function again
            else
                insertBST(root->getRight() , newNode);
        }

    } // comments done

    // fix el properties for RBT after insertion
    void fixInsert(Node* node){
        // loop runs until x becomes root or black
        while(node != root && node->getParent()->isRed()){
            // if node's parent is left child
            if(node->getParent() == node->getParent()->getParent()->getLeft()){
                // assign right child to "uncle" node
                Node* uncle = node->getParent()->getParent()->getRight();


                // case 1 : uncle is red
                if(uncle != nullptr && uncle->isRed()){
                    node->getParent()->setBlack();
                    uncle->setBlack();
                    node->getParent()->getParent()->setRed();
                    node = node->getParent()->getParent();  // Move to grandparent
                }
                else{
                    // case 2 : node is the right child and uncle is black
                    if(node == node->getParent()->getRight()){   //      x                x
                        node = node->getParent();               //     z      --->      y
                        leftRotate(node);                   //        y            z
                    }

                    // case 3: node is the left child (already in second case) and uncle is black
                    node->getParent()->setBlack();                      //       y
                    node->getParent()->getParent()->setRed();          //      z   x
                    rightRotate(node->getParent()->getParent());
                }

            }
                // if node's parent is right child
            else{
                // assign left child to "uncle" node
                Node* uncle = node->getParent()->getParent()->getLeft();

                // case 4 : uncle is red
                if(uncle != nullptr && uncle->isRed()){
                    node->getParent()->setBlack();
                    uncle->setBlack();
                    node->getParent()->getParent()->setRed();
                    node = node->getParent()->getParent();  // Move to grandparent
                }
                else{
                    // case 5 : uncle is black and node is left child
                    if(node == node->getParent()->getLeft()){
                        node = node->getParent();
                        rightRotate(node);
                    }

                    // case 6 : uncle is black and node is right child
                    node->getParent()->setBlack();
                    node->getParent()->getParent()->setRed();
                    leftRotate(node->getParent()->getParent());
                }
            }
        }
        root->setBlack();  // ensure the root is always black
    }

    // use it to get predecessor (left --> right ....right)
    Node* findMax(Node* node){
        if (node == nullptr) return nullptr;

        // talama 3ndy node in right efdl 48al
        while (node->right != nullptr){
            node = node->right;
        }
        return node;
    } // comments done

    // display nodes with in their colors
    void preorderTraversal(Node *node){
        // root ---> left ---> right
        if(node == nullptr){
            return;
        }

        // print root
        cout << node->data << " ";
        getColor(node->data);
        cout << '\n';

        // for left subtree
        preorderTraversal(node->getLeft());

        // for right subtree
        preorderTraversal(node->getRight());
    } // comments done

    // delete el node 3ala 2sas el data
    Node* deleteNode(Node* node , int data){
        // lw el tree empty
        if(node == nullptr){
            cout << "The tree is empty :(" << '\n';
        }

        // lw el node 2s8r call el function for left part
        else if(data < node->data){
            node->left = deleteNode(node->left , data);
        }

        // lw el node 2kbr call el function for right part
        else if(data > node->data){
            node->right = deleteNode(node->right, data);
        }

        // lw 5alas el node == el node that we want delete it
        else{
            // lw el node has 0 children
            if(node->left == nullptr && node->right == nullptr){
                node = nullptr; // just delete it
            }

            // lw el node has left child
            else if(node->left != nullptr && node->right == nullptr){
                node->data = node->left->data; // swap el node with its child
                node->left = nullptr; // delete the child
            }

            // lw el node has right child
            else if(node->left == nullptr && node->right != nullptr){
                node->data = node->right->data; // swap el node with its child
                node->right = nullptr; // delete the child
            }

            // lw el node has two children
            else{
                Node* max = findMax(node->left); // to get predecessor
                node->data = max->data; // make data for el node = data for predecessor
                node->left = deleteNode(node->left , max->data); // delete el max (predecessor)
            }
        }
        return node;
    } // comments done

    // fix el properties for RBT after deletion
    void fixDeletion(Node* node){
        // keep fixing tol ma el node black and not root
        while(node != root && node->isBlack()){
            // lw el node was the left child
            if(node == node->getParent()->getLeft()){
                Node* sibling = node->getSibling(); // store right child in this node


                // if sibling is red
                if(sibling->isRed()){
                    sibling->color = BLACK; // make sibling black
                    node->getParent()->color = RED; // make parent red
                    leftRotate(node->getParent()); // left rotate for parent
                    sibling = node->parent->right; // assign sibling again after rotation
                }

                // if both sibling's children & sibling is black
                if(sibling->left->isBlack() && sibling->right->isBlack()){
                    sibling->color = RED; // make sibling red
                    node = node->getParent(); // move to parent to fix it
                }

                // not both children is black and sibling is black
                else{
                    // lw el far (sibling's right) black
                    if(sibling->getRight()->isBlack()){
                        sibling->getLeft()->color = BLACK; // make near black
                        sibling->color = RED; // make sibling red
                        rightRotate(sibling); // right rotate for sibling
                        sibling = node->getParent()->right; // assign sibling again after rotation
                    }

                    // far is red (sibling's right)
                    sibling->color = sibling->parent->color; // make sibling color = parent color
                    node->parent->color = BLACK; // make parent black
                    sibling->right->color = BLACK; // make far is black
                    leftRotate(node->parent); // left rotate for parent
                    node = root; // After rotation, we end the fix-up process
                }
            }

            // lw el node was the right child (mirror the logic for the left side)
            else{
                Node* sibling = node->parent->left; // store left child in this node


                // if sibling is red
                if(sibling->isRed()){
                    sibling->color = BLACK; // make sibling black
                    node->getParent()->color = RED; // make parent red
                    rightRotate(node->parent); // right rotate for parent
                    sibling = node->parent->left; // assign sibling again after rotation
                }

                // if both sibling's children & sibling is black
                if (sibling->left->isBlack() && sibling->right->isBlack()) {
                    sibling->color = RED; // make sibling red
                    node = node->parent; // move to parent to fix it
                }

                // not both children is black and sibling is black
                else{
                    // lw el far (sibling's left) black
                    if(sibling->getLeft()->isBlack()){
                        sibling->getRight()->color = BLACK; // make near black
                        sibling->color = RED; // make sibling red
                        leftRotate(sibling); // left rotate for sibling
                        sibling = node->getParent()->left; // assign sibling again after rotation
                    }

                    // far is red (sibling's left)
                    sibling->color = sibling->parent->color; // make sibling color = parent color
                    node->parent->color = BLACK; // make parent black
                    sibling->left->color = BLACK; // make far is black
                    rightRotate(node->parent); // right rotate for parent
                    node = root; // After rotation, we end the fix-up process
                }
            }
        }
        node->color = BLACK; // make sure the node is black after fixing
    } // comments done
public:
    // constructor
    RBT(){
        root = nullptr;
    } // comments done

    // general function for insertion
    void insertion(int data){
        Node *newNode = new Node(data);

        // lw el tree empty
        if(root == nullptr){
            root = newNode;
            newNode->setBlack();
        }
        else{
            insertBST(root , newNode);
            fixInsert(newNode);
        }
    } // comments done

    // tell me if the node exist or not
    bool search(int data){
        Node* node = root;

        while(node != nullptr){
            if(data == node->data)
                return true;
            else if(data > node->data)
                node = node->getRight();
            else
                node = node->getLeft();
        }
        return false;
    } // comments done

    // tell me the color for a node
    void getColor(int data){
        Node* node = root;

        while(node != nullptr){
            if(data == node->data){
                if(node->color == BLACK){
                    cout << "Color is BLACK" << '\n';
                    return;
                }
                else{
                    cout << "Color is RED" << '\n';
                    return;
                }
            }
            else if(data > node->data)
                node = node->getRight();
            else
                node = node->getLeft();
        }
        cout << "Node doesn't exist." << '\n';
        return;
    } // comments done

    // general function to display the tree
    void display(){
        // lw el tree empty, just print it
        if (root == nullptr) {
            cout << "Tree is empty :(" << '\n';
            return;
        }
        // lw not empty, print elements
        else {
            preorderTraversal(root);
            return;
        }
    } // comments done

    // general function for deletion
    void deletion(int data) {
        Node* node = root;

        // delete a node
        node = deleteNode(root, data);

        // lw el tree not empty, fix it after deletion
        if (node) {
            fixDeletion(node);
        }
    } // comments done
};

int main(){
    RBT tree;
    int control = -1 , element;

    while (control !=0){
        cout << "Choose the option you want: " << '\n'
             << "1- Insert element."           << '\n'
             << "2- Delete element."           << '\n'
             << "3- Search for an element."    << '\n'
             << "4- Get color for an element." << '\n'
             << "5- Display tree's elements."  << '\n'
             << "0 TO END PROGRAM."            << '\n';


        cin >> control;

        switch (control) {
            case 1:
            {
                cout << "please enter the element: " << '\n';
                cin >> element;
                tree.insertion(element);
                continue;
            }
            case 2:
            {
                cout << "please enter the element: " << '\n';
                cin >> element;
                tree.deletion(element);
                continue;
            }
            case 3:
            {
                cout << "please enter the element: " << '\n';
                cin >> element;
                bool isExist = tree.search(element);
                if(isExist)
                    cout << "The element is exist." << '\n';
                else
                    cout << "The element doesn't exist." << '\n';
                continue;
            }
            case 4:
            {
                cout << "please enter the element: " << '\n';
                cin >> element;
                tree.getColor(element);
                continue;
            }
            case 5:
            {
                tree.display();
                cout << '\n';
                continue;
            }
            case 0:
                cout << "EXITING PROGRAM..." << '\n';
                break;
            default:
            {
                cout << "INVALID CHOICE :(" << '\n';
                continue;
            }
        }
    }

    return 0;
}

/*

 in public:
 1-deletion  ✔️
 2-display   ✔️
 3-getColor  ✔️
 4-search    ✔️
 5-insertion ✔️

 in private:
 1-fixDeletion
 2-deleteNode
 3-preorderTraversal  ✔️
 4-findMax            ✔️
 5-fixInsert          ✔️
 6-insertBST          ✔️
 7-rightRotate        ✔️
 8-leftRotate         ✔️
 * */