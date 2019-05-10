
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>



struct Game{
	char name[100];
	int sales;
	Game * next;
};

typedef struct Game;

typedef struct {
	Game * head;
	Game * tail;
	int Count;
}LinkedList;


Game * Game_init(const char * name_, int sales_ =0, Game * next = NULL) {
	Game * ans = (Game*)malloc(sizeof(Game));
	strcpy(ans->name, name_);
	ans->sales = sales_;
	ans->next = NULL;

	return ans;
}

void LinkedLiset_Init(LinkedList *ls) {
	ls->Count = 0;
	ls->head = NULL;
	ls->tail = NULL;
}

Game * findNode(LinkedList * ls, Game * target) {
	Game * p = ls->head;
	while (p) {
		if (p == target) {
			return p;
		}
		p = p->next;
	}

	return NULL;
}

Game* findPrev(LinkedList * ls, Game * node) {
	assert(ls&&node);
	Game * p = ls->head;
	while (p) {
		if (p->next == node) {
			return p;
		}

		p = p->next;
	}
	return NULL;
}

void InsertToEmpty(LinkedList *ls, Game *node) {
	if (!ls->head) {
		ls->head = node;
		ls->tail = node;
		ls->Count++;
		node->next = NULL;
	}
}

void InsertToHead(LinkedList * ls, Game * node) {
	assert(ls&&node);
	printf("Adding to head node %s \n", node->name);
	if (ls->head) {
		node->next = ls->head;
		ls->head = node;
		ls->Count++;
		
	}
	else {
		InsertToEmpty(ls, node);
	}
}
void InsertToTail(LinkedList *ls, Game * node) {
	if (ls->tail) {
		ls->tail->next = node;
		node->next = NULL;
		ls->tail = node;
		ls->Count++;
		return;
	}

	InsertToEmpty(ls, node);
}
void ListAppend(LinkedList * ls,Game * node,Game * after) {

	assert(ls&&node);

	printf("Adding %s \n", node->name);
	if (!after ||after==ls->tail) {
		InsertToTail(ls, node);
		return;
	}

	if (ls->head) {
		Game * target = findNode(ls, after);

		if (target) {
			node->next = target->next;
			target->next = node;
			ls->Count++;
			return;
		}

		return;
	}

	InsertToEmpty(ls, node);

}

void Remove(LinkedList *ls, Game * node) {
	assert(ls&&node);
	printf("removing %s \n", node->name);
	Game * p = findPrev(ls, node);
	if (node == ls->head) {
		ls->head = node->next;
		ls->Count--;
		if (ls->tail == node) {
			ls->tail = NULL;
		}
	}
	if (p&&findNode(ls,node)) {
		p->next = node->next;
		node->next = NULL;
		ls->Count--;
		return;
	}
}

void ListRelease(LinkedList *ls) {
	Game * p = ls->head;
	while (p) {
		Game * temp = p->next;
		free(p);
		p = temp;
	}
	ls->head = ls->tail = NULL;
	ls->Count = 0;
}
void printLL(LinkedList * ls) {
	Game * p = ls->head;
	printf("printing LinkedLiset with Count : %d \n", ls->Count);
	while (p) {
		printf("	%s, %d\n", p->name, p->sales);
		p = p->next;
	}

	printf("\n");
}

//int main() {
//	LinkedList GameList;
//	LinkedLiset_Init(&GameList);
//
//	Game * LOL = Game_init("LOL", 10000, NULL);
//	ListAppend(&GameList, LOL,NULL);
//
//	printLL(&GameList);
//	
//	Game * Dota2 = Game_init("Dota2", 2000, NULL);
//	ListAppend(&GameList, Dota2, LOL);
//
//	Game * AssassinCreed = Game_init("Assassin", 1000000, NULL);
//	InsertToHead(&GameList, AssassinCreed);
//
//	printLL(&GameList);
//
//	Game * BorderLand = Game_init("BorderLand2", 242345235, NULL);
//	ListAppend(&GameList, BorderLand, LOL);
//
//	printLL(&GameList);
//
//	Remove(&GameList, BorderLand);
//	printLL(&GameList);
//
//	Remove(&GameList, AssassinCreed);
//	printLL(&GameList);
//
//	ListRelease(&GameList);
//	printLL(&GameList);
//
//	printf("=== Program Ended ===\n");
//	printf("Press any to key to Exit !");
//	_getch();
//	return 0;
//}