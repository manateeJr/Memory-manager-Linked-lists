#include <iostream>
#include "MList.h"

using namespace std;

bool MList::is_empty() {
	if (head == 0) { return true;}
	else { return false;}
}
	

int MList::add_first(int address, int size) {
	if (MList::is_empty()) {
		head = new ITEM(address, size);	
	}
	else {
		ITEM* temp = new ITEM(address, size);
		temp->next = head;
		head = temp;
	}
	return 0;
}

int MList::add_after(int address, int size, int memstart) {
	if (MList::is_empty()) {
		return -1;
	}
	ITEM* temp = head;
	while (temp->_addr != memstart && temp->next != 0) {
		temp = temp->next;
	}
	
	ITEM* to_add = new ITEM(address, size);
	to_add->next = temp->next;
	temp->next = to_add;

	return 0;
	
}


int MList::remove_first() {
	if (MList::is_empty()) {
		return -1;
	}
	
	ITEM* temp = head->next;
	delete head;
	head = temp;
	
	return 0;
}
