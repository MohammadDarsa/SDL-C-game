#pragma once
#include <stdio.h>
#include <Entity.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <Constants.h>

typedef struct ObjNode {
	Entity* entity;
	SDL_Rect* mesh;
	struct ObjNode* next;
}ObjNode;

void initMesh(SDL_Rect* mesh, float x, float y, float width, float height) {
	mesh->x = (int)x;
	mesh->y = (int)y;
	mesh->w = (int)width;
	mesh->h = (int)height;
}

void pushObj(ObjNode** headRef, float x, float y, float width, float height, int r, int g, int b, int a, char* name) {

	ObjNode* temp = (ObjNode*)malloc(sizeof(ObjNode));

	if (!temp) {
		fprintf(stderr, "ERROR MESH LIST:: Node couldn't be allocated\n");
		return;
	}

	Entity* entity = (Entity*)malloc(sizeof(Entity));
	if (!entity) {
		fprintf(stderr, "ERROR MESH LIST:: entity couldn't be allocated\n");
		return;
	}
	initEntity(entity, x, y, width, height, r, g, b, a, name);

	SDL_Rect* mesh = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (!mesh) {
		fprintf(stderr, "ERROR MESH LIST:: mesh couldn't be allocated\n");
		return;
	}
	initMesh(mesh, x, y, width, height);

	temp->entity = entity;
	temp->mesh = mesh;

	temp->next = *headRef;
	*headRef = temp;
}

void deleteObjList(ObjNode** headRef) {
	ObjNode* temp = NULL;
	while (*headRef) {
		temp = *headRef;
		*headRef = (*headRef)->next;
		free(temp->entity);
		free(temp->mesh);
		free(temp);
	}
	*headRef = NULL;
}

ObjNode** initHashTable(int n) {
	ObjNode** headRef = (ObjNode**)malloc(sizeof(ObjNode*) * n);
	
	if (!headRef) {
		fprintf(stderr, "ERROR HASH TABLE:: table couldn't be allocated\n");
		return NULL;
	}
	for (int i = 0; i < n; i++)
		headRef[i] = NULL;
	return headRef;
}

ObjNode** deleteTable(ObjNode** tableRef, int n) {
	for (int i = 0; i < n; i++) deleteObjList(&tableRef[i]);
	free(tableRef);
	return NULL;
}

int getIndex(char* key, int n) {
	int hashCode = 0;
	int base = 1;
	int p = 37;
	for (int i = strlen(key) - 1; i >= 0; i--) {
		hashCode += key[i] * base;
		base *= p;

		hashCode %= n;
		base %= n;
	}
	return hashCode;
}

ObjNode* getObject(ObjNode** table, int n, char* key) {
	int index = getIndex(key, n);
	ObjNode* head = table[index];
	while (head) {
		if (!strcmp(head->entity->name, key))
			return head;
		head = head->next;
	}
	return NULL;
}

void insertObject(ObjNode** table, int n, float x, float y, float width, float height, int r, int g, int b, int a, char* name) {
	int index = getIndex(name, n);
	ObjNode* head = getObject(table, n, name);
	if (head) return;
	pushObj(&table[index], x, y, width, height, r, g, b, a, name);
}

void renderObject(ObjNode** table, int n, char* key, SDL_Renderer* renderer) {
	ObjNode* object = getObject(table, n, key);
	SDL_SetRenderDrawColor(renderer, object->entity->r, object->entity->g, object->entity->b, object->entity->a);
	SDL_RenderFillRect(renderer, object->mesh);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void renderTable(ObjNode** table, int n, SDL_Renderer* renderer) {
	for (int i = 0; i < n; i++) {
		ObjNode* object = table[i];
		while (object) {
			SDL_SetRenderDrawColor(renderer, object->entity->r, object->entity->g, object->entity->b, object->entity->a);
			SDL_RenderFillRect(renderer, object->mesh);
			object = object->next;
		}
	}
}

void updateObj(ObjNode** table, int n, float x, float y, float width, float height, int r, int g, int b, int a, char* key) {
	ObjNode* obj = getObject(table, n, key);
	initEntity(obj->entity, obj->entity->x + x, obj->entity->y + y, obj->entity->width + width, obj->entity->height + height, obj->entity->r + r, obj->entity->g + g, obj->entity->b + b, obj->entity->a + a, key);
	initMesh(obj->mesh, obj->entity->x + x, obj->entity->y + y, obj->entity->width + width, obj->entity->height + height);
}