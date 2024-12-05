#include <iostream>
#include <bits/stdc++.h>
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

    void insertBST(Node* root, Node* newNode){
        if(newNode->data < root->data){ // if new value small, go to the left
            if(root->getLeft() == nullptr)
                root->setLeft(newNode);
            else
                insertBST(root->getLeft(), newNode);
        }
        else{ // if new value big, go to the right
            if(root->getRight() == nullptr)
                root->setRight(newNode);
            else
                insertBST(root->getRight() , newNode);
        }

    }

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

    Node* findNode(int data){
        Node* current = root;

        while(current != nullptr){
            if(data == current->data)
                return current;
            else if (data < current->data)
                current = current->getLeft();
            else
                current = current->getRight();
        }
        return nullptr; // node doesn't exist
    }

    void replaceNode(Node* oldNode , Node* newNode){
        if(oldNode->getParent() == nullptr) // old node is root
            root = newNode;
        else if(oldNode == oldNode->getParent()->getLeft()) // old is left child
            oldNode->getParent()->setLeft(newNode);
        else // old is right child
            oldNode->getParent()->setRight(newNode);

        if (newNode != nullptr) {
            newNode->setParent(oldNode->getParent());
        }
    }

    Node* getMinimum(Node* node){
        while(node->getLeft() != nullptr){
            node = node->getLeft();
        }
        return node;
    }

//    void fixDeletion(Node* node){}
public:
    RBT(){
        root = nullptr;
    }

    void insertion(int data){
        Node *newNode = new Node(data);

        if(root == nullptr){
            root = newNode;
            newNode->setBlack();
        }
        else{
            insertBST(root , newNode);
            fixInsert(newNode);
        }
    }

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
    }

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
    }

    void preorderTraversal(Node *node){

        if(node == nullptr){
            return;
        }
        // Visit the root
        cout << node->data << " ";

        // Recursively visit the left subtree
        preorderTraversal(node->getLeft());

        // Recursively visit the right subtree
        preorderTraversal(node->getRight());
    }

    void display(){
        if (root == nullptr) {
            cout << "Tree is empty :(" << '\n';
        } else {
            preorderTraversal(root);
            cout << '\n'; // Print a newline after traversal
        }
    }

//    void deletion(int data){
//        Node* nodeToDelete = findNode(data);
//
//        if(nodeToDelete == nullptr){
//            cout << "Node doesn't exist." << '\n';
//            return;
//        }
//
//        Node* y = nodeToDelete; // node i want delete
//        Node* x = nullptr; // child that i will replace with
//        Color yColor = y->color;
//
//        if(nodeToDelete->getLeft() == nullptr){ // no left child
//            x = nodeToDelete->getRight();
//            replaceNode(nodeToDelete , x);
//        }
//        else if (nodeToDelete->getRight() == nullptr) { // no right child
//            x = nodeToDelete->getLeft();
//            replaceNode(nodeToDelete, x);
//        }
//        else{
//            y = getMinimum(nodeToDelete->getRight()); // use successor
//            yColor = y->color;
//            x = y->getRight();
//
//            if(nodeToDelete == y->getParent()){
//                if(x != nullptr)
//
//            }
//        }
//    }
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