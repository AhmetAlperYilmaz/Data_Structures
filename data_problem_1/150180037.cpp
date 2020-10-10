/* @Author

Student Name: <Ahmet Alper Yılmaz>

Student ID: <150180037>

Date: <19-10-2019> */

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

struct node{
int sizes;
int quantity;
node *next;
};

struct stock{
node * create(node * stck);
node * add_stock(node * stc, int shoesize);
node * sell(node * stc, int shoesize);
void current_stock(node * stc);
void clearing(node * stc);
};
stock stonks;

node * stock::create(node * stck){
    node * stc = new node; // creating a node and sets its next to NULL
    stc->sizes = 0;
    stc->quantity = 0;
    stc->next = NULL;
    return stc;
}

node * stock::add_stock(node *stc, int shoesize){
    if (stc == NULL){ // if our shoe stock is empty
        stc= new node;
        stc->next = NULL;
        stc->sizes = shoesize;
        stc->quantity = 1;
        return stc;
    }
    if (stc->sizes > shoesize){ // if adding shoesize is smaller than our smallest shoesize of our stock
        node * temp = new node;
        temp->sizes = shoesize;
        temp->quantity = 1;
        temp->next = stc;
        stc = temp;
        return stc;
    }
    if (stc->sizes == shoesize){ // if adding shoesize equals to our smallest shoesize
        stc->quantity += 1;
        return stc;
    }
    node * psg = stc;
    while (psg->next!= NULL && psg->next->sizes <= shoesize){ // we create a node pointer to find the suitable order for our adding shoesize
        psg = psg->next;
    }
    if(psg->sizes == shoesize){ // if adding shoesize is already in our stock;
        psg->quantity += 1;
        return stc;
    }
    else if(stc->sizes == 0 && stc->quantity == 0){ // if we have an empty created node
        stc->sizes = shoesize;
        stc->quantity = 1;
        return stc;
    }
    else{
        node * temp = new node; // if this shoesize is new for our stock
        temp->next = psg->next;
        psg->next = temp;
        temp->sizes = shoesize;
        temp->quantity = 1;
        return stc;
    }
}

node * stock::sell(node * stc, int shoesize){
    node * temp;
    node * psg = stc;
    int shoesize2 = abs(shoesize); // we are taking absolute value of shoesize because we take input negatively
    if (stc->sizes == shoesize2){ // if stc is pointing to node which includes wanted shoesize
        if (stc->quantity == 1){ // if there is only one pair shoes in our first node, deleting node
            temp = stc;
            stc = stc->next;
            delete(temp);
            return stc;
        }
        else if(stc->quantity >1){ // if more than one shoes in our first node
            stc->quantity -= 1;
            return stc;
        }
    }
    while (psg->next != NULL && psg->next->sizes != shoesize2){ // looking for our wanted shoesize in our stock
        psg = psg->next;
    }
    if (psg->next == NULL) { // if we do not have wanted shoesize
        cout << "NO_STOCK" << endl;
        return stc;
    }
    else{
        if(psg->next->quantity == 1){ // if there is only one pair shoes, deleting node
            temp = psg->next;
            psg->next = psg->next->next;
            delete(temp);
            return stc;
        }
        else if(psg->next->quantity > 1){ // if more than one shoes
            psg->next->quantity -= 1;
            return stc;
        }
    }
}

void stock::current_stock(node *stc){
    while (stc!= NULL){ // printing all node's shoesizes and quantities
        cout << stc->sizes << ":" << stc->quantity << endl;
        stc = stc->next;
    }
}

void stock::clearing(node* stc){
    node * deleter;
    while (stc!= NULL){ // deleting every nodes one by one
      deleter = stc;
      stc = stc->next;
      delete(deleter);
    }
    delete(stc);
}

int main(){
    node * root;
    root = NULL;
    root = stonks.create(root);
    ifstream file("input.txt"); // our input file
    int size2; // taking inputs as integers
    if(file.is_open()){ // if we are successful while opening the file
        while(file >> size2){
            if(size2 > 0){ // adding stock condition
                root = stonks.add_stock(root, size2);
            }
            else if (size2==0){ // printing stock condition
                stonks.current_stock(root);
            }
            else if (size2 < 0){ // selling stock condition
                root = stonks.sell(root, size2);
            }
        }
        stonks.clearing(root); // deleting all stock
        file.close(); // closing file
    }
    else{ // input file is corrupted or could not open
            cout << "File could not open" << endl;
            std::cin.get();
    }

    return 0;
}
