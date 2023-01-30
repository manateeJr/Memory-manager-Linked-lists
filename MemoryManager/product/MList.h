#pragma once 
typedef struct ITEM
{
    struct ITEM *next=0;
    int _addr;
    int _size;
    ITEM(int addr,int size) {_addr=addr; _size=size;}
}ITEM;

class MList {
    public:
        ITEM *head=0;
        ITEM *tail=0;
	
	/* all MList methods should come here */	
	int add_first(int address, int size); // claim mem
	int add_after(int address, int size, int memstart); // claim mem
	int remove_first();

	bool is_empty();
};

