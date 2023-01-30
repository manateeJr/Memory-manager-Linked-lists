	#include "MemoryManager.h"
#include "MList.h"
#include <iostream>

using namespace std;

FreeList::~FreeList() {
	ITEM* temp = head;
	while (head != 0) {
		temp = head;
		head = head->next;		
		delete temp;
	}
}

AllocList::~AllocList() {
	ITEM* temp = head;
	while (head != 0) {
		temp = head;
		head = head->next;		
		delete temp;
	}
}

/* Starting point for MemoryManager constructor */
MemoryManager::MemoryManager()
{
	allocList = new AllocList(memStart, maxSize);
	freeList = new FreeList(memStart, maxSize);
}

/* Code correct destructor to cleanup all memory */
MemoryManager::~MemoryManager()
{
	allocList->~AllocList(); // clean alloclist
	freeList->~FreeList(); // clean freelist
	delete allocList;
	delete freeList;
}

/* pre : size > 0
 * post: If no memory block of size "size" available return -1
 *       Otherwise claimMemory() returns the first
 *       address in the freeList where a memory block of at least size is present.
 *       This memory is allocated and not any more available.
 *       The newly allocated block is added to the allocList and addr of the newly
 *       allocated block is returned
 */ 
int MemoryManager::claimMemory(int size)
{
	if (size <= 0 || size > MemoryManager::maxSize) {
		return -1;
	}
	
	int addr;
	if (allocList->is_empty()) { // first element
		addr = freeList->head->_addr;
		allocList->add_first(memStart, size);
		freeList->head->_size -= size;
		freeList->head->_addr += size;
	}
	else {
		int ret = 0;
		ITEM* to_add = findInFreeList(freeList->head, size, &ret);
		if (ret == -1) { return -1;}	
		
		addInAllocList(allocList->head, to_add, size);
		addr = to_add->_addr;
		
		adjustFreeMemory(to_add, size);
	}
	
	checkHead();
			
	return addr;
}


void MemoryManager::checkHead() {
	ITEM* temp = freeList->head;
	if (temp->_size == 0) {
		freeList->remove_first();
	}	
}

ITEM* MemoryManager::findInFreeList(ITEM* head, int size, int* ret) {
	while ((head->next != 0) && (size > head->_size)) {
		head = head->next;
	}
	if (head->_size - size < 0) {
		*ret = -1;
	}
	return head;	
}

void MemoryManager::addInAllocList(ITEM* temp_allocList, ITEM* to_add, int size) {
	if (temp_allocList->_addr > to_add->_addr) {
		allocList->add_first(to_add->_addr, size); // mem leak
	}
	else {
		while (temp_allocList->next != 0 && (temp_allocList->next->_addr < to_add->_addr)) {
			temp_allocList = temp_allocList->next;
		}
		allocList->add_after(to_add->_addr, size, temp_allocList->_addr); // mem leak
	}	
}

void MemoryManager::adjustFreeMemory(ITEM* to_add, int size) {
	to_add->_size -= size;
	to_add->_addr += size;		
}

/* pre:  Parameter addr must match the start of an allocatd memory block. Otherwhise return -1.
 *
 * post: If addr is a part of allocated memory of allocList then the memory from this address
 * 	     with registered size should be removed from the allocList and added to the freeList
 *
 *       freeMemory(addr) returns number of bytes (size) belongig to the address addr
 *       In all other cases freeMemory(freeList,allocList,addr) returns -1.
 */
 
 int MemoryManager::freeMemory(int addr) {
	if (addr < MemoryManager::memStart || (addr > MemoryManager::memStart + MemoryManager::maxSize))  {
		return -1;
	}
	if (allocList->head == 0) {
		return -1;
	}
	int size;
	int ret = 0;
	
	ITEM* to_add = findInAllocList(allocList->head, addr, &ret);
	if (ret == -1) { return -1;}
	size = to_add->_size;
	
	addInFreeList(freeList->head, to_add, addr);
	mergeMemoryBlocks();	
	
	return size;
 }

void MemoryManager::addInFreeList(ITEM* head, ITEM* to_add, int addr) {
	
	if (head != 0) {
		if (head->_addr > addr) {
			to_add->next = freeList->head;
			freeList->head = to_add;
		}
		else {
			while (head->next != 0 && (head->_addr > addr || head->next->_addr < addr)) {
				head = head->next;
			}
			to_add->next = head->next;
			head->next = to_add;
		}
	}
	else {
		freeList->add_first(to_add->_addr, to_add->_size);
		delete to_add;
	}
}
 
 ITEM* MemoryManager::findInAllocList(ITEM* to_add, int addr, int* ret) {
	if (to_add->_addr == addr) {		
		allocList->head = allocList->head->next;
	}
	else {
		ITEM* prev = to_add;
		while (to_add->next != 0 && to_add->_addr != addr) {
			prev = to_add;
			to_add = to_add->next;
		}
		
		if (to_add->_addr != addr) { *ret = -1;}
		else { prev->next = to_add->next;}
	}
	
	return to_add;
} 
 
void MemoryManager::mergeMemoryBlocks() {	
	ITEM* head = freeList->head;
	ITEM* to_free;
	while (head->next != 0) {			
		if ((head->_addr + head->_size == head->next->_addr) || (head->next->_size == 0)) {
			head->_size += head->next->_size;
			to_free = head->next;
			head->next = head->next->next;
			delete to_free;
		}
		else {
			head = head->next;
		}
	}
}
