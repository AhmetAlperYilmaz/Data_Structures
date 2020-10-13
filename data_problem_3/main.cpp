/* @Author

Ahmet Alper Yılmaz

Date: 18-12-2019 */

#include <iostream>	
#include <cstring>
#include <stdlib.h>
#include <fstream> 

using namespace std;

struct mystack{
int number;
mystack *prev;
mystack *next;
};

struct node{
char *nodetype;
int self_id;
int parent_id;
node *child;
node *level;
node *parent;
node *mobilehost;
};

struct rootstruct{
bool found;
node *root;
mystack *stcroot;
mystack *parentpathroot;
void create(node *paranode);
void searchforbasestation(node *searching, node *traverse);
void addbasestation(node *paranode);
void addmobilehost(node *paranode);
void depthfirsttraversal(node *searching, node *traverse);
node *searched;
}rtstrct;

void rootstruct::create(node *paranode){
	root = paranode;
	root->self_id = 0;
	root->child = NULL;
	root->level = NULL;
	root->parent = NULL;
	root->mobilehost = NULL;
}

void rootstruct::searchforbasestation(node *searching, node *traverse){
	if(searching->parent_id == traverse->self_id){
		searched = traverse;
	}
	if(traverse->child != NULL){
		node *newsearching = searching;
		node *newtraverse = traverse->child;
		searchforbasestation(newsearching, newtraverse);
	}
	if(traverse->level != NULL){
		node *newsearching = searching;
		node *newtraverse = traverse->level;
		searchforbasestation(newsearching, newtraverse);
	}
}

void rootstruct::addbasestation(node *paranode){
	if(root->child == NULL && paranode->parent_id == 0){
		root->child = paranode;
		paranode->parent = root;
		paranode->child = NULL;
		paranode->level = NULL;
		paranode->mobilehost = NULL;
		return;
	}
	if(root->child == NULL && paranode->parent_id != 0){
		cerr << "Invalid try" << endl;
		exit(1);
	}
	if(paranode->parent_id == 0){
		if(root->child->level == NULL){
			paranode->parent = root;
			root->child->level = paranode;
			paranode->child = NULL;
			paranode->level = NULL;
			paranode->mobilehost = NULL;
			return;
		}
		node *iter = root->child;
		while(iter->level != NULL){
			iter = iter->level;
		}
		paranode->parent = root;
		iter->level = paranode;
		paranode->child = NULL;
		paranode->level = NULL;
		paranode->mobilehost = NULL;
		return;
	}
	node *traversal = root->child;
	searchforbasestation(paranode, traversal);
	paranode->parent = searched;
	if(searched->child == NULL){
		searched->child = paranode;
		paranode->child = NULL;
		paranode->level = NULL;
		paranode->mobilehost = NULL;
		return;
	}
	if(searched->child->level == NULL){
		searched->child->level = paranode;
		paranode->child = NULL;
		paranode->level = NULL;
		paranode->mobilehost = NULL;
		return;
	}
	node *iter = searched->child;
	while(iter->level != NULL){
		iter = iter->level;
	}
	iter->level = paranode;
	paranode->child = NULL;
	paranode->level = NULL;
	paranode->mobilehost = NULL;
	return;

}

void rootstruct::addmobilehost(node *paranode){
	node *traversal = root->child;
	searchforbasestation(paranode, traversal);
	paranode->parent = searched;
	if(searched->mobilehost == NULL){
		searched->mobilehost = paranode;
		paranode->child = NULL;
		paranode->level = NULL;
		paranode->mobilehost = NULL;
		return;
	}
	if(searched->mobilehost->level == NULL){
		searched->mobilehost->level = paranode;
		paranode->child = NULL;
		paranode->level = NULL;
		paranode->mobilehost = NULL;
		return;
	}
	node *iter = searched->mobilehost;
	while(iter->level != NULL){
		iter = iter->level;
	}
	iter->level = paranode;
	paranode->child = NULL;
	paranode->level = NULL;
	paranode->mobilehost = NULL;
	return;

}

void rootstruct::depthfirsttraversal(node *searching, node *traverse){
	if(traverse == root){
		mystack *newstack = new mystack;
		stcroot = newstack;
		stcroot = new mystack;
		stcroot->number = traverse->self_id;
		stcroot->prev = NULL;
		stcroot->next = NULL;
		if(root->child != NULL){
			traverse = traverse->child;
		}
	}
	if(traverse == root->child){
		mystack *newstack2 = new mystack;
		newstack2->number = traverse->self_id;
		stcroot->prev = newstack2;
		stcroot->next = newstack2;
		newstack2->prev = stcroot;
		newstack2->next = stcroot;
	}
	if(traverse->mobilehost != NULL){
		node *iter = traverse->mobilehost;
		if(iter->self_id == searching->self_id){
			found = true;
			mystack *temporary = new mystack;
			temporary->next = temporary;
			temporary->prev = temporary;
			parentpathroot = temporary;
			parentpathroot->number = traverse->self_id;
			node *trvrs = traverse->parent;
			while(trvrs != root){
				mystack *tempforparent = new mystack;
				tempforparent->number = trvrs->self_id;
				parentpathroot->prev->next = tempforparent;
				tempforparent->prev = parentpathroot->prev;
				parentpathroot->prev = tempforparent;
				tempforparent->next = parentpathroot;
				if(trvrs != root){
					trvrs = trvrs->parent;
				}
			}
			if(trvrs == root){
				mystack *tempforaryparent = new mystack;
				tempforaryparent->number = 0;
				parentpathroot->prev->next = tempforaryparent;
				tempforaryparent->prev = parentpathroot->prev;
				parentpathroot->prev = tempforaryparent;
				tempforaryparent->next = parentpathroot;
			}
			return;
		}
		while(iter->level != NULL){
			if(iter->self_id == searching->self_id){
				found = true;
				mystack *temporary = new mystack;
				temporary->next = temporary;
				temporary->prev = temporary;
				parentpathroot = temporary;
				parentpathroot->number = traverse->self_id;
				node *trvrs = traverse->parent;
				while(trvrs != root){
					mystack *tempforparent = new mystack;
					tempforparent->number = trvrs->self_id;
					parentpathroot->prev->next = tempforparent;
					tempforparent->prev = parentpathroot->prev;
					parentpathroot->prev = tempforparent;
					tempforparent->next = parentpathroot;
					if(trvrs != root){
						trvrs = trvrs->parent;
					}
				}
				if(trvrs == root){
					mystack *tempforaryparent = new mystack;
					tempforaryparent->number = 0;
					parentpathroot->prev->next = tempforaryparent;
					tempforaryparent->prev = parentpathroot->prev;
					parentpathroot->prev = tempforaryparent;
					tempforaryparent->next = parentpathroot;
				}
				return;
			}
			if(iter->level != NULL){
				iter = iter->level;
			}
		}
		if(iter->self_id == searching->self_id){
			found = true;
			mystack *temporary = new mystack;
			temporary->next = temporary;
			temporary->prev = temporary;
			parentpathroot = temporary;
			parentpathroot->number = traverse->self_id;
			node *trvrs = traverse->parent;
			while(trvrs != root){
				mystack *tempforparent = new mystack;
				tempforparent->number = trvrs->self_id;
				parentpathroot->prev->next = tempforparent;
				tempforparent->prev = parentpathroot->prev;
				parentpathroot->prev = tempforparent;
				tempforparent->next = parentpathroot;
				if(trvrs != root){
					trvrs = trvrs->parent;
				}
			}
			if(trvrs == root){
				mystack *tempforaryparent = new mystack;
				tempforaryparent->number = 0;
				parentpathroot->prev->next = tempforaryparent;
				tempforaryparent->prev = parentpathroot->prev;
				parentpathroot->prev = tempforaryparent;
				tempforaryparent->next = parentpathroot;
			}
			return;
		}
	}
	if(traverse->child != NULL){
		if(found == false){
			node *newtraverse = traverse->child;
			node *newsearching = searching;
			mystack *newstack = new mystack;
			newstack->number = newtraverse->self_id;
			stcroot->prev->next = newstack;
			newstack->prev = stcroot->prev;
			stcroot->prev = newstack;
			newstack->next = stcroot;
			depthfirsttraversal(newsearching, newtraverse);
		}
	}
	if(traverse->level != NULL){
		if(found == false){
			node *newtraverse = traverse->level;
			node *newsearching = searching;
			mystack *newstack = new mystack;
			newstack->number = newtraverse->self_id;
			stcroot->prev->next = newstack;
			newstack->prev = stcroot->prev;
			stcroot->prev = newstack;
			newstack->next = stcroot;
			depthfirsttraversal(newsearching, newtraverse);
		}
	}
}
	
int main(int argc, char *argv[]) {
		node *head = new node;
		rtstrct.create(head);
		string input1 = argv[1];
		string input2 = argv[2];
		ifstream inNetwork;
		ifstream inMessage; 
		string typeofchild;
		int the_id;
		int the_parent_id;
		inNetwork.open(input1);
		//Checking for error
		if(!inNetwork){
			cerr << "Error occured when opening the file" << endl;
			exit(1);
		}
		while(inNetwork >> typeofchild >> the_id >> the_parent_id){	
			node *temp = new node;
			temp->self_id = the_id;
			temp->parent_id = the_parent_id;
			if(typeofchild == "BS"){
				rtstrct.addbasestation(temp);
			}
			else if(typeofchild== "MH"){
				rtstrct.addmobilehost(temp);
			}
		}
		inNetwork.close();
		node *temp = new node;
		rtstrct.found = false;
		temp->self_id = 27;
		node *traversal = rtstrct.root;
		rtstrct.depthfirsttraversal(temp,traversal);
		inMessage.open(input2);
		//Checking for error
		if(!inMessage){
			cerr << "Error occured when opening the file" << endl;
			exit(1);
		}
		while(inMessage.peek() != EOF){
			string messagefrom;
			string target_id;
			int targetID;
			getline(inMessage,messagefrom,'>');
			getline(inMessage,target_id,'\n');
			char messager[messagefrom.size() + 1];
			messagefrom.copy(messager, messagefrom.size() + 1);
			messager[messagefrom.size()] = '\0';
			targetID = stoi(target_id);
			node *temp = new node;
			rtstrct.found = false;
			temp->self_id = targetID;
			node *traversal = rtstrct.root;
			cout << "Traversing:";
			rtstrct.depthfirsttraversal(temp, traversal);
			mystack *iters = rtstrct.stcroot;
			while (iters->next != rtstrct.stcroot)
			{
				cout << iters->number << " ";
				iters = iters->next;
			}
			cout << iters->number << endl;
			if(rtstrct.found == false){
				cout << "Can not be reached the mobile host mh_" << targetID << " at the moment" << endl;
			}
			else if(rtstrct.found == true){
				cout << "Message:";
				cout << messager << " To:";
				mystack *printer = rtstrct.parentpathroot->prev;
				while(printer != rtstrct.parentpathroot){
					cout << printer->number << " ";
					printer = printer->prev;
				}
				cout << rtstrct.parentpathroot->number << " ";
				cout << "mh_" << targetID << endl;
			}
		}
		inMessage.close();
		return 0;
}
