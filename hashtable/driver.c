#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#include "hashtable.prototype.h"


int main(void) {

	SNAME *table = SNAME_new();

	assert(!SNAME_get_ptr(table, "key"));

	SNAME_set(table, "key", 5);

	uint64_t value = 0;
	assert(SNAME_get(table, "key", &value));
	assert(value == 5);

	SNAME_delete(table, "key", false);
	assert(!SNAME_get(table, "key", &value));
	assert(!SNAME_get_ptr(table, "key"));

	SNAME_set(table, "key", 77);
	SNAME_set(table, "key2",55);
	assert(SNAME_get(table, "key", &value));
	assert(value == 77);
	assert(SNAME_get(table, "key2", &value));
	assert(value == 55);
	assert(table->size == 2);

	key_node *iter = table->keys;
	size_t count = 0;
	while (iter) {
		count++;
		assert(SNAME_get(table, iter->key, &value));
		iter = iter->next;
	}
	assert(count == table->size);
	SNAME_empty(table, false);
	assert(table->size == 0);

	for (uint64_t i = 0; i < 100; ++i) {
		char key[100];
		sprintf(key, "%ld", i);
		SNAME_set(table, key, i);
	}
	assert(table->size == 100);
	assert(table->_raw_size == 1000);

	__SNAME_grow(table);
	assert(table->size == 100);
	assert(table->_raw_size == 2000);

	SNAME_empty(table, false);
	for (uint64_t i = 0; i < 1000; ++i) {
		char key[100];
		sprintf(key, "%ld", i);
		SNAME_set(table, key, i);
	}
	assert(table->_raw_size == 4000);

	for (uint64_t i = 0; i < 1000; ++i) {
		char key[100];
		sprintf(key, "%ld", i);
		uint64_t value;
		SNAME_get(table, key, &value);
		assert(value == i);
	}

	return 0;
}

