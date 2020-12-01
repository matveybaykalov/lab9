#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <cmath>


using std::cout;
using std::cin;
using std::endl;
using std::vector;

template<typename tn, typename info>
struct Node{
    Node* RightN = nullptr;
    Node* LeftN = nullptr;
    int height = 1;
    Node* prev = nullptr;
    tn key;
    info information;
};

template<typename tn, typename info>
struct Tree{
private:
    Node<tn, info>* root = new Node<tn, info>;
    bool IfExistRoot = false;
    vector<vector<Node<tn, info>*>> VectorForPrint;
    int MaxLengthOfEl;

    int GiveLength (info value){
        std::stringstream ss;
        ss << value;  //перевод любого значения к string
        std::string str;
        ss >> str;
        return str.length();
    }

    void ComparMax (Node<tn, info>* leave){
        if (GiveLength(leave->information) > MaxLengthOfEl){
            MaxLengthOfEl = GiveLength(leave->information);
        }
    }
    Node<tn, info>* SmallLeftTurn(Node<tn, info>* node){
        Node<tn, info>* TempNodeD = node->RightN->LeftN;
        Node<tn, info>* TempNodeA = node;
        if (node->prev == nullptr){
            root = node->RightN; //если новая вершина станет корнем, то указатель корня поменять на эту вершину
        } else if(TempNodeA->prev->RightN == node){
            TempNodeA->prev->RightN = node->RightN; //связь предыдущей от текущей вершины с будущей вершиной
        } else TempNodeA->prev->LeftN = node->RightN;
        node->RightN->prev = node->prev; //связь той вершины, которая станет новой с предыдущей от текущей
        node = node->RightN; //изменение на новую вершину
        TempNodeA->prev = node; //связываем вершину а с текущей
        node->LeftN = TempNodeA; // связываем текущую вершину с а
        TempNodeA->RightN = TempNodeD; // связываем вершину a с d
        if (TempNodeD != nullptr) TempNodeD->prev = TempNodeA; // связываем вершину d с a
        TempNodeA->height = TakeHeight(TempNodeD) + 1; //изменяем глубину вершины a
        node->height = std::max(TakeHeight(node->LeftN), TakeHeight(node->RightN))+1;
        return node;
    }

    Node<tn, info>* SmallRightTurn(Node<tn, info>* node){
        Node<tn, info>* TempNodeD = node->LeftN->RightN;
        Node<tn, info>* TempNodeA = node;
        if (node->prev == nullptr){
            root = node->LeftN; //если новая вершина станет корнем, то указатель корня поменять на эту вершину
        } else if(TempNodeA->prev->RightN == node){
            TempNodeA->prev->RightN = node->LeftN; //связь предыдущей от текущей вершины с будущей вершиной
        } else TempNodeA->prev->LeftN = node->LeftN;
        node->LeftN->prev = node->prev; //связь той вершины, которая станет новой с предыдущей от текущей
        node = node->LeftN; //изменение на новую вершину
        TempNodeA->prev = node; //связываем вершину а с текущей
        node->RightN = TempNodeA; // связываем текущую вершину с а
        TempNodeA->LeftN = TempNodeD; // связываем вершину a с d
        if (TempNodeD != nullptr) TempNodeD->prev = TempNodeA; // связываем вершину d с a
        TempNodeA->height = TakeHeight(TempNodeD) + 1; //изменяем глубину вершины a
        node->height = std::max(TakeHeight(node->LeftN), TakeHeight(node->RightN))+1;
        return node;
    }

    Node<tn, info>* BigLeftTurn(Node<tn, info>* node){
        Node<tn, info>* TempNodeA = node;
        Node<tn, info>* TempNodeB = node->RightN;
        Node<tn, info>* TempNodeE = node->RightN->LeftN->LeftN;
        Node<tn, info>* TempNodeF = node->RightN->LeftN->RightN;
        if (node->prev == nullptr){
            root = node->RightN->LeftN;
        } else if(node->prev->RightN == node){
            node->prev->RightN = node->RightN->LeftN;
        } else node->prev->LeftN = node->RightN->LeftN;
        node->RightN->LeftN->prev = node->prev;
        node = node->RightN->LeftN;
        node->LeftN = TempNodeA;
        TempNodeA->prev = node;
        TempNodeA->RightN = TempNodeE;
        if (TempNodeE != nullptr) TempNodeE->prev = TempNodeA;
        node->RightN = TempNodeB;
        TempNodeB->prev =node;
        TempNodeB->LeftN = TempNodeF;
        if (TempNodeF != nullptr) TempNodeF->prev = TempNodeB;
        TempNodeA->height -= 2;
        TempNodeB->height --;
        node->height++;
        return node;
    }

    Node<tn, info>* BigRightTurn(Node<tn, info>* node){
        Node<tn, info>* TempNodeA = node;
        Node<tn, info>* TempNodeB = node->LeftN;
        Node<tn, info>* TempNodeE = node->LeftN->RightN->LeftN;
        Node<tn, info>* TempNodeF = node->LeftN->RightN->RightN;
        if (node->prev == nullptr){
            root = node->LeftN->RightN;
        } else if(node->prev->RightN == node){
            node->prev->RightN = node->LeftN->RightN;
        } else node->prev->LeftN = node->LeftN->RightN;
        node->LeftN->RightN->prev = node->prev;
        node = node->LeftN->RightN;
        node->LeftN = TempNodeB;
        TempNodeB->prev = node;
        TempNodeB->RightN = TempNodeE;
        if (TempNodeE != nullptr) TempNodeE->prev = TempNodeB;
        node->RightN = TempNodeA;
        TempNodeA->prev =node;
        TempNodeA->LeftN = TempNodeF;
        if (TempNodeF != nullptr) TempNodeF->prev = TempNodeA;
        TempNodeA->height -= 2;
        TempNodeB->height --;
        node->height++;
        return node;
    }

    int TakeHeight(Node<tn, info>* node){
        if (node == nullptr){
            return 0;
        } else return node->height;
    }

    void Balance (Node<tn, info>* temp){
        if (TakeHeight(temp->RightN)-TakeHeight(temp->LeftN) == 2 &&
            TakeHeight(temp->RightN->RightN)-TakeHeight(temp->RightN->LeftN) <= 1 &&
            TakeHeight(temp->RightN->RightN)-TakeHeight(temp->RightN->LeftN) >= 0){
            temp = SmallLeftTurn(temp);
        }
        if (TakeHeight(temp->LeftN)-TakeHeight(temp->RightN) == 2 &&
            TakeHeight(temp->LeftN->LeftN)-TakeHeight(temp->LeftN->RightN) <= 1 &&
            TakeHeight(temp->LeftN->LeftN)-TakeHeight(temp->LeftN->RightN) >= 0){
            temp = SmallRightTurn(temp);
        }
        if (TakeHeight(temp->RightN)-TakeHeight(temp->LeftN) == 2 &&
            TakeHeight(temp->RightN->LeftN) > TakeHeight(temp->RightN->RightN)){
            temp = BigLeftTurn(temp);
        }
        if (TakeHeight(temp->LeftN)-TakeHeight(temp->RightN) == 2 &&
            TakeHeight(temp->LeftN->RightN) > TakeHeight(temp->LeftN->LeftN)){
            temp = BigRightTurn(temp);
        }
    }

    void ChangeH (Node<tn, info>* temp){
        while (temp != root) {
            if (temp->prev != nullptr){
                temp = temp->prev;
                temp->height = std::max(TakeHeight(temp->LeftN), TakeHeight(temp->RightN))+1;
            }
            Balance(temp);
        }
    }
public:
    void print () {
        if (IfExistRoot) {
            Node<tn, info> *leave = root;
            VectorForPrint.resize(leave->height);
            VectorForPrint[leave->height - 1].push_back(leave);
            MaxLengthOfEl = GiveLength(leave->information);
            if (leave->height > 1) {
                SetLeave(leave->LeftN, leave->height - 2);
                SetLeave(leave->RightN, leave->height - 2);
            }
            int length = MaxLengthOfEl;
            for (int i = leave->height - 1; i >= 0; --i) {
                for (Node<tn, info> *el : VectorForPrint[i]) {
                    if (el == nullptr) {
                        cout << std::setw(pow(2, i) * (length + 1)) << std::setfill(' ') << ' ';
                    } else {
                        int a = pow(2, i - 1) * (length + 1);
                        int b = ceil(length / 2.);
                        cout << std::setw(pow(2, i - 1) * (length + 1) + ceil(length / 2.))
                             << std::setfill(' ') << el->information
                             << std::setw(pow(2, i - 1) * (length + 1) - ceil(length / 2.)) << ' ';
                    }
                }
                cout << endl;
            }
            VectorForPrint.clear();
        } else cout << "Tree not found" << endl;
    }
    void SetLeave(Node<tn, info>* leave, int height) {
        if (height != 0) {
            if (leave == nullptr) {
                VectorForPrint[height].push_back(nullptr);
                SetLeave(nullptr, height-1);
                SetLeave(nullptr, height-1);
            } else {
                ComparMax(leave);
                VectorForPrint[height].push_back(leave);
                SetLeave(leave->LeftN, height-1);
                SetLeave(leave->RightN, height-1);
            }
        } else{
            if (leave != nullptr){
                ComparMax(leave);
            }
            VectorForPrint[height].push_back(leave);
        }
    }
    void insert(tn value, info information){
        if (!IfExistRoot){
            root->key = value;
            IfExistRoot = true;
        } else {
            bool flag = true;
            Node<tn, info>* NewLeave = root;
            while (flag) {
                if (value >= NewLeave->key) {
                    if (NewLeave->RightN == nullptr) {
                        flag = false;
                        NewLeave->RightN = new Node<tn, info>;
                        NewLeave->RightN->prev = NewLeave;
                        NewLeave = NewLeave->RightN;
                        NewLeave->key = value;
                        NewLeave->information = information;
                        ChangeH(NewLeave);
                    } else {
                        NewLeave = NewLeave->RightN;
                    }
                } else {
                    if (NewLeave->LeftN == nullptr) {
                        flag = false;
                        NewLeave->LeftN = new Node<tn, info>;
                        NewLeave->LeftN->prev = NewLeave;
                        NewLeave = NewLeave->LeftN;
                        NewLeave->key = value;
                        NewLeave->information = information;
                        ChangeH(NewLeave);
                    } else {
                        NewLeave = NewLeave->LeftN;
                    }
                }
            }
        }
    }
    void remove(tn value) {
        if (IfExistRoot) {
            bool flag = true;
            Node<tn, info> *leave = root;
            while (flag) {
                if (value == leave->value) {
                    if (leave != root || leave->RightN != nullptr || leave->LeftN != nullptr) {
                        if (leave->RightN == nullptr && leave->LeftN == nullptr) {
                            flag = false;
                            leave = leave->prev;
                            if (leave->RightN != nullptr && leave->RightN->value == value) {
                                delete leave->RightN;
                                leave->RightN = nullptr;
                                leave->height = std::max(TakeHeight(leave->LeftN), TakeHeight(leave->RightN)) + 1;
                                Balance(leave);
                                ChangeH(leave);
                            } else {
                                delete leave->LeftN;
                                leave->LeftN = nullptr;
                                leave->height = std::max(TakeHeight(leave->LeftN), TakeHeight(leave->RightN)) + 1;
                                Balance(leave);
                                ChangeH(leave);
                            }
                        } else {
                            flag = false;
                            if (leave->LeftN == nullptr) {
                                if (leave != root) { //ситуация 5-10
                                    if (leave->prev->RightN == leave) {
                                        leave->prev->RightN = leave->RightN;
                                    } else {
                                        leave->prev->LeftN = leave->RightN;
                                    }
                                    leave->RightN->prev = leave->prev;
                                    Balance(leave->RightN);
                                    ChangeH(leave->RightN);
                                    delete leave;
                                } else {
                                    root = leave->RightN;
                                    leave->RightN->prev = nullptr;
                                    IfExistRoot = false;
                                    delete leave;
                                }
                            } else {
                                Node<tn, info> *MaxNode = leave->LeftN;
                                while (MaxNode->RightN != nullptr) {
                                    MaxNode = MaxNode->RightN;
                                }
                                leave->value = MaxNode->value;
                                leave->information = MaxNode->information;
                                if (MaxNode->LeftN != nullptr) {
                                    if (MaxNode->prev->RightN == MaxNode) {
                                        MaxNode->prev->RightN = MaxNode->LeftN;
                                    } else {
                                        MaxNode->prev->LeftN = MaxNode->LeftN;
                                    }
                                    MaxNode->LeftN->prev = MaxNode->prev;
                                    MaxNode->prev->height = std::max(TakeHeight(MaxNode->prev->LeftN),
                                                                     TakeHeight(MaxNode->prev->RightN)) + 1;
                                    ChangeH(MaxNode);
                                    delete MaxNode;
                                } else {
                                    if (MaxNode->prev->RightN == MaxNode) {
                                        MaxNode->prev->RightN = nullptr;
                                    } else {
                                        MaxNode->prev->LeftN = nullptr;
                                    }
                                    MaxNode->prev->height = std::max(TakeHeight(MaxNode->prev->LeftN),
                                                                     TakeHeight(MaxNode->prev->RightN)) + 1;
                                    ChangeH(MaxNode);
                                    delete MaxNode;
                                }
                            }
                        }
                    } else {
                        root = nullptr;
                        delete leave;
                    }
                } else if (leave->RightN != nullptr || leave->LeftN != nullptr) {
                    if (value > leave->value) {
                        leave = leave->RightN;
                    } else leave = leave->LeftN;
                } else {
                    cout << "Element "<< value <<" not found" << endl;
                    flag = false;
                }
            }
        } else {
            cout << "Element "<< value <<" not found" << endl;
        }
    }
    void destruct(){
        while(root->LeftN != nullptr || root->RightN != nullptr){
            remove(root->value);
        }
    }
};

int main() {
    Tree<int, std::string> MyTree;
    MyTree.insert(1, "sdfdfs");
    MyTree.insert(2, "dfaqwe");
    MyTree.insert(3,"uil");
    MyTree.insert(4,"opwerk");
    MyTree.insert(5,"jhwer");
    MyTree.insert(6,"Aosqpw");
    MyTree.print();
    return 0;
}
