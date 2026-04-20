#include "table.h"

template <typename KeyType, typename FieldType>
class MemNode {
	TTable<KeyType, FieldType> table;
	MemNode* parent;
public:
	MemNode(): table(), parent(nullptr) {}

	void insert(KeyType key, FieldType field) {
		MemNode* curr = this;
		while (!(curr || curr->table.contain(key)))
			curr = curr->parent;
		if (curr) {
			curr->table.insert(key, field);
		}
		else table.insert(key, field);
	}

	void insert_last(KeyType key, FieldType field) {
		table.insert(key, field);
	}

	FieldType get(KeyType key) {
		MemNode* curr = this;
		while (!(curr || curr->table.contain(key)))
			curr = curr->parent;
		if (curr) {
			curr->table.get(key);
		}
		else throw -2;
	}

	bool contain(KeyType key) {
		MemNode* curr = this;
		while (!(curr || curr->table.contain(key)))
			curr = curr->parent;
		if (curr) {
			return 1;
		}
		else return 0;
	}

	void create() {
		MemNode* node = new MemNode();
		node->table.swap(table);
		node->parent = parent;
		parent = node;
	}

	void drop() {
		if (!parent) throw -1;
		MemNode* node = parent;
		node->table.swap(table);
		parent = node->parent;
		delete node;
	}
};

