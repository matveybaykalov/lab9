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
    Node<tn, info>* root = nullptr;
    int MaxLengthOfEl;

    int GiveLength (info value){
        std::stringstream ss;
        ss << value;  //перевод любого значения к string
        std::string str;
        ss >> str;
        return str.length();
    }

    void CompareMax (Node<tn, info>* leave){
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
        } else if (TakeHeight(temp->LeftN)-TakeHeight(temp->RightN) == 2 &&
            TakeHeight(temp->LeftN->LeftN)-TakeHeight(temp->LeftN->RightN) <= 1 &&
            TakeHeight(temp->LeftN->LeftN)-TakeHeight(temp->LeftN->RightN) >= 0){
            temp = SmallRightTurn(temp);
        } else if (TakeHeight(temp->RightN)-TakeHeight(temp->LeftN) == 2 &&
            TakeHeight(temp->RightN->LeftN) > TakeHeight(temp->RightN->RightN)){
            temp = BigLeftTurn(temp);
        } else if (TakeHeight(temp->LeftN)-TakeHeight(temp->RightN) == 2 &&
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
    void SetLeave(Node<tn, info>* leave, int height, vector<vector<Node<tn, info>*>>& VectorForPrint) {
        if (height != 0) {
            if (leave == nullptr) {
                VectorForPrint[height].push_back(nullptr);
                SetLeave(nullptr, height-1, VectorForPrint);
                SetLeave(nullptr, height-1, VectorForPrint);
            } else {
                CompareMax(leave);
                VectorForPrint[height].push_back(leave);
                SetLeave(leave->LeftN, height-1, VectorForPrint);
                SetLeave(leave->RightN, height-1, VectorForPrint);
            }
        } else{
            if (leave != nullptr){
                CompareMax(leave);
            }
            VectorForPrint[height].push_back(leave);
        }
    }
public:
    void print () {
        if (root != nullptr) {
            vector<vector<Node<tn, info>*>> VectorForPrint;
            Node<tn, info> *leave = root;
            VectorForPrint.resize(leave->height);
            VectorForPrint[leave->height - 1].push_back(leave);
            MaxLengthOfEl = GiveLength(leave->information);
            if (leave->height > 1) {
                SetLeave(leave->LeftN, leave->height - 2, VectorForPrint);
                SetLeave(leave->RightN, leave->height - 2, VectorForPrint);
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
        } else cout << "Tree not found" << endl;
    }
    void insert(tn value, info information){
        if (root == nullptr){
            root = new Node<tn, info>;
            root->key = value;
            root->information = information;
        } else {
            bool flag = true;                   //этот флаг мне нужен для нахожения нужного места, где будет находится новый узел
            Node<tn, info>* NewLeave = root;    //можно было бы сделать по-другому, но тогда нужно было бы заводить временную ячейку, которая была бы копией текущей
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
        if (root != nullptr) {
            Node<tn, info> *leave = root;
            while (value != leave->key){ //поиск удаляемого узла
                if (leave->RightN != nullptr || leave->LeftN != nullptr) { //тут дальнейшее движение ячейки
                    if (value > leave->key) {
                        leave = leave->RightN;
                    } else leave = leave->LeftN;
                } else {
                    break;
                }
            }
            if (leave->key == value) { //это проверка на то, является ли текущий элемент удаляемым или нет, потому что пользователь может попросить удалить элемент не принадлежащий дереву
                if (leave != root || leave->RightN != nullptr || leave->LeftN != nullptr) { //проверка является ли текущий узел корнем или листом
                    if (leave->RightN == nullptr && leave->LeftN == nullptr) {
                        leave = leave->prev;
                        if (leave->RightN != nullptr &&
                            leave->RightN->key == value) { //смотрю из какой вершины я поднялся, потом ту и удаляю
                            delete leave->RightN;
                            leave->RightN = nullptr;
                            leave->height = std::max(TakeHeight(leave->LeftN), TakeHeight(leave->RightN)) + 1; //изменяю глубину текщей вершины
                            Balance(leave);
                            ChangeH(leave); //изменяю глубину вершин которые находятся выше
                        } else {
                            delete leave->LeftN;
                            leave->LeftN = nullptr;
                            leave->height = std::max(TakeHeight(leave->LeftN), TakeHeight(leave->RightN)) + 1;
                            Balance(leave);
                            ChangeH(leave);
                        }
                    } else {
                        if (leave->LeftN ==
                            nullptr) { //проверяю есть ли в левый наследник у удаялемого узла, если да, то там буду искать максимум,
                            if (leave != root) {       //есле нет, то я смотрю откуда я попал в текущий узел и тому узлу присваиваю правого наследника
                                if (leave->prev->RightN == leave) {
                                    leave->prev->RightN = leave->RightN;
                                } else {
                                    leave->prev->LeftN = leave->RightN;
                                }
                                leave->RightN->prev = leave->prev;
                                Balance(leave->RightN);
                                ChangeH(leave->RightN);
                                delete leave;
                            } else { //ситуация 5-10
                                root = leave->RightN; //это одна из крайних ситуация, когда нужно удалить кореной элемент, к оторого есть только правый наследник
                                leave->RightN->prev = nullptr;
                                delete leave;
                            }
                        } else {
                            Node<tn, info> *MaxNode = leave->LeftN; //MaxNode ищет максимальный элемент в правом поддереве
                            while (MaxNode->RightN != nullptr) { //поиск максимального элемента
                                MaxNode = MaxNode->RightN;
                            }
                            leave->key = MaxNode->key;
                            leave->information = MaxNode->information;
                            if (MaxNode->prev->RightN ==
                                MaxNode) {    //смотрю из какой вершины я попал в MaxNode и присваиваю этой левого потомка
                                MaxNode->prev->RightN = MaxNode->LeftN;
                            } else {
                                MaxNode->prev->LeftN = MaxNode->LeftN;
                            }
                            if (MaxNode->LeftN != nullptr) {
                                MaxNode->LeftN->prev = MaxNode->prev;
                            }
                            MaxNode->prev->height = std::max(TakeHeight(MaxNode->prev->LeftN), TakeHeight(MaxNode->prev->RightN)) + 1;
                            ChangeH(MaxNode);
                            delete MaxNode;
                        }
                    }
                } else { //это ситуация,когда удаляемый элемент является корнем, у которого нет наследника
                    root = nullptr;
                    delete leave;
                }
            } else {
                cout << "Element "<< value <<" not found" << endl;
            }
        } else {
            cout << "Tree is not exist" << endl;
        }
    }
    void destruct(){
        del(root);
        root = nullptr;
    }
};

template<typename tn, typename info>
void del(Node<tn, info>* node){
    if (node->LeftN != nullptr){
        del(node->LeftN);
        node->LeftN = nullptr;
    }
    if (node->RightN != nullptr){
        del(node->RightN);
        node->RightN = nullptr;
    }
    delete node;
}

int main() {
    Tree<int, std::string> MyTree;
    MyTree.insert(1, "sdfdfs");
    MyTree.insert(2, "dfaqwe");
    MyTree.insert(3,"uil");
    MyTree.insert(4,"opwerk");
    MyTree.insert(5,"jhwer");
    MyTree.insert(6,"Aosqpw");
    MyTree.print();
    MyTree.remove(4);
    MyTree.print();
    MyTree.destruct();
    return 0;
}
