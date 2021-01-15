#pragma once
#include <string.h>

typedef struct Entity {
	float x, y, width, height;
	char name[50];
	int r, g, b, a;
} Entity;

void initEntity(Entity* entity, float x, float y, float width, float height, int r, int g, int b, int a, char* name) {
	entity->x = x;
	entity->y = y;
	entity->width = width;
	entity->height = height;
	entity->r = r;
	entity->g = g;
	entity->b = b;
	entity->a = a;
	strcpy(entity->name, name);
}