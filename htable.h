#pragma once

#include <string>

int char_to_int(char c);

size_t hash(long long item);

size_t hash(std::string item);

template <typename KeyType, typename FieldType>
class THashTable {
	struct KeyNode {
		KeyType key;
		FieldType field;
		bool used;

		KeyNode(): used(0) {}
		KeyNode(KeyType _key, FieldType _field):
			key(_key), field(_field), used(1) {}
		KeyNode(const KeyNode& copy):
			key(copy.key), field(copy.field), used(copy.used) {}
	};

	KeyNode* rows;
	size_t size = 17;
	size_t A = 7;
	size_t B = 11;

	size_t get_hash_pos(size_t key) {
		return (A + B * key) % size;
	}

	size_t get_pos(KeyType key) {
		return (A + B * hash(key)) % size;
	}

	void rehash() {
		size_t old_size = size;
		size *= 2;
		KeyNode* new_rows = new KeyNode[size];
		for (int i = 0; i < old_size; ++i) {
			if (rows[i].used) {
				size_t pos;
				bool any;
				pos = get_pos(rows[i].key);
				any = !new_rows[pos].used;
				for (int i = 0; i < 4; i++) {
					if (any) break;
					pos = get_hash_pos(pos);
					any = !new_rows[pos].used;
				}
			}
		}
		delete[] rows;
		rows = new_rows;
	}

	KeyNode* find(KeyType key) {
		size_t pos;
		bool any;
		pos = get_pos(key);
		any = rows[pos].used && rows[pos].key == key;
		for (int i = 0; i < 4; i++) {
			if (any) break;
			pos = get_hash_pos(pos);
			any = rows[pos].used && rows[pos].key == key;
		}
		if (!any) return nullptr;
		return rows+pos;
	}

public:
	THashTable() {
		rows = new KeyNode[size];
	}

	~THashTable() {
		delete[] rows;
	}

	THashTable(const THashTable &copy): size(copy.size), A(copy.A), B(copy.B) {
		rows = new KeyNode[size];
		std::copy(copy.rows, copy.rows+size, rows);
	}

	THashTable& operator= (const THashTable& other) {
		if (this == &other) return *this;
		delete[] rows;
		size = other.size;
		A = other.A;
		B = other.B;
		rows = new KeyNode[size];
		std::copy(other.rows, other.rows+size, rows);
		return *this;
	}

	void insert(KeyType key, FieldType field) {
		KeyNode* fkey = find(key);
		if (fkey) {
			fkey->field = field;
			return;
		}
		size_t pos;
		bool any;
		for (int j = 0; j < 2; j++) {
			pos = get_pos(key);
			any = !rows[pos].used;
			for (int i = 0; i < 4; i++) {
				if (any) break;
				pos = get_hash_pos(pos);
				any = !rows[pos].used;
			}
			if (!any) rehash();
			else break;
		}
		if (!any) {
			B *= B;
			rehash();
			pos = get_pos(key);
			any = !rows[pos].used;
			for (int i = 0; i < 4; i++) {
				if (any) break;
				pos = get_hash_pos(pos);
				any = !rows[pos].used;
			}
		}
		if (!any) throw -1;
		rows[pos] = KeyNode(key, field);
	}

	void remove(KeyType key) {
		KeyNode* fkey = find(key);
		if (!fkey) {
			throw - 1;
		}
		fkey->used = 0;
	}

	FieldType get(KeyType key) {
		KeyNode* fkey = find(key);
		if (!fkey) {
			throw - 1;
		}
		return fkey->field;
	}

	bool contain(KeyType key) {
		KeyNode* fkey = find(key);
		if (!fkey) {
			return 0;
		}
		return 1;
	}

};
