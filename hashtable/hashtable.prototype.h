#ifndef CMF_HASHTABLE_PROTO_H
#define CMF_HASHTABLE_PROTO_H

#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h> // proto

typedef char *KEY_TYPE;

uint64_t strhash(KEY_TYPE str)
{
	uint64_t hash = 5381;
	int c;
	while (*str){
		c = *(str++);
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	return hash;
}

struct key_node {
	KEY_TYPE key;
	struct key_node *next;
};

typedef struct key_node key_node;

//#define CMF_HASHTABLE(SNAME, DATA_TYPE, UNINITIALIZER)

typedef uint64_t DATA_TYPE;				//proto
bool UNINITIALIZER(DATA_TYPE *data) {printf("Uninitializing %ld\n", *data);} // proto

// struct definitions

struct SNAME_chain_node {
	KEY_TYPE key;
	DATA_TYPE data;
	struct SNAME_chain_node *next;
};

struct SNAME_data_chain_node {
	KEY_TYPE key;
	DATA_TYPE data;
	struct SNAME_data_chain_node *next;
};

struct SNAME_table {
	key_node *keys;
	size_t size;

	struct SNAME_data_chain_node **_table;
	size_t _raw_size;

};

bool (*SNAME_uninitializer)(DATA_TYPE*) = UNINITIALIZER;

typedef struct SNAME_table SNAME;

/* forward decls*/
SNAME *SNAME_new();
void SNAME_free(SNAME *table, bool uninitialize);
void SNAME_empty(SNAME *table, bool uninitialize);

bool SNAME_get(SNAME *table, const KEY_TYPE key, DATA_TYPE *out);
DATA_TYPE *SNAME_get_ptr(SNAME *table, const KEY_TYPE key);
void SNAME_set(SNAME *table, const KEY_TYPE key, DATA_TYPE value);
bool SNAME_delete(SNAME *table, const KEY_TYPE key, bool uninitialize);
bool __SNAME_grow(SNAME *table);


/* implementation */

SNAME *__SNAME_new(size_t size) {
	SNAME *table = (SNAME *)malloc(sizeof(SNAME));
	memset(table, 0, sizeof(SNAME));
	table->_raw_size = size;
	table->_table = (struct SNAME_data_chain_node **)malloc(sizeof(struct SNAME_data_chain_node*) * table->_raw_size);
	memset(table->_table, 0, sizeof(struct SNAME_data_chain_node*) * table->_raw_size);
	return table;
}

/*** Initializes new table ***/
SNAME *SNAME_new() {
	return __SNAME_new(1000);
}


/*** Deletes all elements in table ***/
void SNAME_empty(SNAME *table, bool uninitialize) {
	while (table->keys){
		SNAME_delete(table, table->keys->key, uninitialize);
	}
}
/*** Frees the table ***/
void SNAME_free(SNAME *table, bool uninitialize) {
	SNAME_empty(table, uninitialize);
	free(table->_table);
	free(table);
}

/*** Deletes element from the table ***/
bool SNAME_delete(SNAME *table, const KEY_TYPE key, bool uninitialize) {

	uint64_t idx = strhash(key) % table->_raw_size;
	struct SNAME_data_chain_node *iter = table->_table[idx];
	struct SNAME_data_chain_node *prev = NULL;

	while (iter){
		if (strcmp(key, iter->key) != 0) {
			prev = iter;
			iter = iter->next;
			continue;
		}
		if (!prev) {
			table->_table[idx] = iter->next;
		} else {
			prev->next = iter->next;
		}
		uninitialize && SNAME_uninitializer(&iter->data);
		free(iter->key);
		free(iter);
		iter = NULL;
	}

	key_node *kiter = table->keys;
	key_node *kprev = NULL;

	while (kiter){
		if (strcmp(key, kiter->key) != 0) {
			kprev = kiter;
			kiter = kiter->next;
			continue;
		}
		if (!kprev) {
			table->keys = kiter->next;
		} else {
			kprev->next = kiter->next;
		}
		free(kiter->key);
		free(kiter);
		kiter = NULL;
	}
	table->size--;
	return true;
}

/*** Gets a pointer to the value of the key ***/
DATA_TYPE *SNAME_get_ptr(SNAME *table, const KEY_TYPE key) {
	uint64_t idx = strhash(key) % table->_raw_size;

	struct SNAME_data_chain_node *iter = table->_table[idx];
	while (iter){
		if (strcmp(key, iter->key) == 0) {
			return &iter->data;
		}
		iter = iter->next;
	}
	return NULL;
}

/*** Gets the value of the key ***/
bool SNAME_get(SNAME *table, const KEY_TYPE key, DATA_TYPE *out) {
	DATA_TYPE *data = SNAME_get_ptr(table, key);
	if(!data) return false;
	*out = *data;
	return true;
}


/*** Sets the key's value  ***/
void SNAME_set(SNAME *table, const KEY_TYPE key, DATA_TYPE value) {
	DATA_TYPE *data = SNAME_get_ptr(table, key);
	if (!data) {
		if (table->size > table->_raw_size/3) __SNAME_grow(table);

		uint64_t idx = strhash(key) % table->_raw_size;
		key_node *head = table->keys;
		table->keys = (key_node*)malloc(sizeof(key_node));
		table->keys->key = (char*)malloc(sizeof(char)*strlen(key)+1);
		strcpy((char*) table->keys->key, key);
		table->keys->next = head;

		struct SNAME_data_chain_node *thead = table->_table[idx];
		table->_table[idx] = (struct SNAME_data_chain_node*)malloc(sizeof(struct SNAME_data_chain_node));
		table->_table[idx]->key = (char*)malloc(sizeof(char)*strlen(key)+1);
		strcpy((char*) table->_table[idx]->key, key);
		table->_table[idx]->next = thead;

		table->size++;
		data = &table->_table[idx]->data;
	}
	*data = value;
}

/*** Double table size and reinsert all elements.  ***/
bool __SNAME_grow(SNAME *table) {
	SNAME *new_table = __SNAME_new(table->_raw_size * 2);

	key_node *iter = table->keys;
	while(table->keys) {

		DATA_TYPE data;
		SNAME_get(table, table->keys->key, &data);
		SNAME_set(new_table, table->keys->key, data);
		SNAME_delete(table, table->keys->key, false);
	}

	struct SNAME_data_chain_node **old_table = table->_table;
	*table = *new_table;
	new_table->_table = old_table;
	new_table->keys = NULL;
	SNAME_free(new_table, false);
}

// end macro

#endif
