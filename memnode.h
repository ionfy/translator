#pragma once

#include "stypes.h"
#include "table.h"

template <typename KeyType, typename FieldType>
class MemNode {
	TTable<KeyType, FieldType> table;
	MemNode* parent;
public:
	MemNode(): table(), parent(nullptr) {}
	MemNode(MemNode* node): table(), parent(node) {}

	void insert(KeyType key, FieldType field) {
		MemNode* curr = this;
		while (curr && !curr->table.contain(key))
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
		while (curr && !curr->table.contain(key))
			curr = curr->parent;
		if (curr) {
			return curr->table.get(key);
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

	MemNode* get_parent() {
		return parent;
	}
};

using MemNodeT = MemNode<std::string, Type>;