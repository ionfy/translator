#pragma once

#include <filesystem>
#include <stack>
#include <algorithm>

template <typename KeyType, typename FieldType>
class TTable {
	struct KeyNode {
		KeyType key;
		FieldType field;
		KeyNode *left;
		KeyNode *right;
		int height;

		KeyNode(KeyType _key, FieldType _field, KeyNode *_left = nullptr, KeyNode *_right = nullptr):
			key(_key), field(_field), left(_left), right(_right), height(0) {}
		KeyNode(const KeyNode& copy):
			key(copy.key), field(copy.field), left(copy.left), right(copy.right), height(copy.height) {}
	};

	int compare(KeyType &lkey, KeyType &rkey) {
		if (lkey < rkey) return 1;
		if (lkey > rkey) return -1;
		return 0;
	}

	KeyNode* root;

	int updateHeight(KeyNode* node) {
		if (!node) return 0;
		int lh = 0, rh = 0;

		if (node->left)
			lh = node->left->height;
		if (node->right)
			rh = node->right->height;
		node->height = std::max(rh, lh) + 1;
		return node->height;
	}

	int getDiff(KeyNode* left, KeyNode* right) {
		return updateHeight(right) - updateHeight(left);
	}

	KeyNode* LRotate(KeyNode* p) {
		KeyNode* q = p->right;
		KeyNode* left = p->left;
		KeyNode* right = q->right;
		KeyNode* mid = q->left;
		q->right = right;
		q->left = p;
		p->left = left;
		p->right = mid;
		updateHeight(p);
		updateHeight(q);
		return q;
	}

	KeyNode* RRotate(KeyNode* q) {
		KeyNode* p = q->left;
		KeyNode* left = p->left;
		KeyNode* right = q->right;
		KeyNode* mid = p->right;
		p->left = left;
		p->right = q;
		q->left = mid;
		q->right = right;
		updateHeight(q);
		updateHeight(p);
		return p;
	}

	void balance(std::stack<KeyNode*> &path) {
		KeyNode* curr;
		while(!path.empty()) {
			curr = path.top();
			path.pop();
			if (getDiff(curr->left, curr->right) > 1) {
				curr = LRotate(curr);
				if (getDiff(curr->left, curr->right) < -1) {
					curr->left = LRotate(curr->left);
					curr = RRotate(curr);
				}
				if (path.empty()) {
					root = curr;
					return;
				}
				if (path.top()->left == curr->left) {
					path.top()->left = curr;
					return;
				}
				else {
					path.top()->right = curr;
					return;
				}
			}
			if (getDiff(curr->left, curr->right) < -1) {
				curr = RRotate(curr);
				if (getDiff(curr->left, curr->right) > 1) {
					curr->right = RRotate(curr->right);
					curr = LRotate(curr);
				}
				if (path.empty()) {
					root = curr;
					return;
				}
				if (path.top()->left == curr->right) {
					path.top()->left = curr;
					return;
				}
				else {
					path.top()->right = curr;
					return;
				}
			}
		}
	}

	KeyNode* copyTree(KeyNode* node) {
		if (node == nullptr) return nullptr;
		KeyNode* ret = new KeyNode(*node);
		ret->left = copyTree(node->left);
		ret->right = copyTree(node->right);
		return ret;
	}

	void clearTree(KeyNode* node) {
		if (node == nullptr) return;
		clearTree(node->left);
		clearTree(node->right);
		delete node;
	}

	class TTableIter {
		std::stack<KeyNode*> nodes;
	
		void addLeft(KeyNode* node) {
			while (node) {
				nodes.push(node);
				node = node->left;
			}
		}
	public:
		TTableIter(KeyNode* node) {
			addLeft(node);
		}

		KeyNode* next() {
			if (nodes.empty()) throw -1;

			KeyNode* node = nodes.top();
			nodes.pop();
			
			if (node->right) addLeft(node->right);

			return node;
		}

		int hasNext() {
			return !nodes.empty();
		}
	};

public:
	TTable(): root(nullptr) {
	}

	~TTable() {
		clearTree(root);
	}

	TTable(const TTable &copy) {
		root = copyTree(copy.root);
	}

	TTable& operator= (const TTable& other) {
		if (this == &other) return *this;
		clearTree(root);
		root = copyTree(other.root);
		return *this;
	}

	TTableIter iter() {
		return TTableIter(root);
	}

	void insert(KeyType key, FieldType field) {
		if (root == nullptr) {
			root = new KeyNode(key, field);
			return;
		}
		std::stack<KeyNode*> path;
		KeyNode *t = root;
		while(1) {
			int comp = compare(t->key, key);
			if (comp == 0) {
				t->field = field;
				break;
			}

			path.push(t);

			if (comp > 0){
				if (t->right == nullptr) {
					t->right = new KeyNode(key, field);
					break;
				}
				t = t->right;
			}
			else {
				if (t->left == nullptr) {
					t->left = new KeyNode(key, field);
					break;
				}
				t = t->left;
			}
		}
		balance(path);
	}

	void remove(KeyType key) {
		if (!root) throw -1;
		KeyNode* t = root;
		std::stack<KeyNode*> path;

		while (t) {
			int comp = compare(t->key, key);
			if (comp == 0) break;
			path.push(t);
			if (comp > 0) t = t->right;
			else t = t->left;
		}

		if (!t) throw -2;
		if (t->left && t->right) {
			path.push(t);
			KeyNode* tl = t->left;
			while (tl->right) {
				path.push(tl);
				tl = tl->right;
			}
			KeyType ktmp = t->key;
			FieldType ftmp = t->field;
			t->key = tl->key;
			t->field = tl->field;
			tl->key = ktmp;
			tl->field = ftmp;
			KeyNode* p = path.top();
			if (t == p) t->left = tl->left;
			else p->right = tl->left;
			delete tl;
		}
		else {
			KeyNode* td = t->left;
			if (!td) td = t->right;

			if (path.empty()) {
				root = td;
			}
			else
			{
				KeyNode* c = path.top();
				if (c->left == t) c->left = td;
				else c->right = td;
			}
			delete t;
		}
		balance(path);
	}

	FieldType get(KeyType key) {
		if (!root) throw -1;
		KeyNode* t = root;

		while (t) {
			int comp = compare(t->key, key);
			if (comp == 0) break;
			if (comp > 0) t = t->right;
			else t = t->left;
		}

		if (!t) throw -2;

		return t->field;
	}

	bool contain(KeyType key) {
		if (!root) return 0;
		KeyNode* t = root;

		while (t) {
			int comp = compare(t->key, key);
			if (comp == 0) break;
			if (comp > 0) t = t->right;
			else t = t->left;
		}

		if (!t) return 0;

		return 1;
	}

	void refresh(TTable& other) {
		TTableIter iter = this->iter();
		TTableIter oiter = other.iter();

		if (!iter.hasNext() || !oiter.hasNext()) return;

		KeyNode* node = iter.next();
		KeyNode* onode = oiter.next();

		while (iter.hasNext() && oiter.hasNext()) {
			int comp = compare(node->key, onode->key);

			if (comp == 0) {
				node->field = onode->field;
				node = iter.next();
				onode = oiter.next();
				continue;
			}

			if (comp > 0) node = iter.next();
			else onode = oiter.next();
		}

		if (compare(node->key, onode->key) == 0) {
			node->field = onode->field;
		}
	}

	void swap(TTable& other) {
		KeyNode temp = root;
		root = other.root;
		other.root = temp;
	}
};
