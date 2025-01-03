#pragma once

#include <iostream>
#include <vector>
#include <queue>

#include "AVL_Util.h"
#include "MapIterator.h"

template<typename Key, typename Value, typename Comparator>
class Map
{
public:
	using KeyType = Key;
	using ValueType = Value;

	using KeyValueType = Node<Key, Value>;

	using iterator = MapIterator<Map<Key, Value, Comparator>>;

protected:
	Node<Key, Value>* m_root;

	Comparator m_comparator;

	size_t m_size;

	void transplant(Node<Key, Value>* _first, Node<Key, Value>* _second);
	void freeMemory(Node<Key, Value>* _subroot);

	void printDFS(int _choice, Node<Key, Value>* _subroot) const;
	void printBFS(Node<Key, Value>* _subroot) const;

	void insert_repair(Node<Key, Value>* _node);
	void delete_repair(Node<Key, Value>* _node);

	int LR(Node<Key, Value>* _node);
	int RL(Node<Key, Value>* _node);

	int LL(Node<Key, Value>* _node);
	int RR(Node<Key, Value>* _node);

	void deep_copy(Node<Key, Value>* _subroot);

public:
	Map();
	Map(const Map<Key, Value, Comparator>& _tree);
	Map(const std::vector<std::pair<Key, Value>>& _keyvals);
	~Map();

	Map<Key, Value, Comparator>& operator=(const Map<Key, Value, Comparator>& _tree); // TODO

	iterator begin();
	iterator rbegin();

	iterator end();
	iterator rend();

	Node<Key, Value>* max(Node<Key, Value>* _subroot) const;
	Node<Key, Value>* min(Node<Key, Value>* _subroot) const;
	Node<Key, Value>* successor(Node<Key, Value>* _node) const;
	Node<Key, Value>* predecessor(Node<Key, Value>* _node) const;
	Node<Key, Value>* find(Key _key) const;

	Value& operator[](const Key& _key);

	void insert(Key _key, Value _value);
	void insert(std::pair<Key, Value> _keyval);
	void remove(Key _key);
	void erase(Node<Key, Value>* _node);

	void print(int _choice) const;
	bool empty() const;
	void clear();

	Node<Key, Value>* getRoot() const;
	size_t size();
};

template<typename Key, typename Value, typename Comparator>
void Map<Key, Value, Comparator>::transplant(Node<Key, Value>* _first, Node<Key, Value>* _second)
{
	if (_first->m_parent == nullptr)
		this->m_root = _second;

	else
	{
		if (_first == _first->m_parent->m_left)
			_first->m_parent->m_left = _second;

		else
			_first->m_parent->m_right = _second;
	}

	if (_second != nullptr)
		_second->m_parent = _first->m_parent;
}

template<typename Key, typename Value, typename Comparator>
void Map<Key, Value, Comparator>::freeMemory(Node<Key, Value>* _subroot)
{
	if (_subroot == nullptr)
		return;

	freeMemory(_subroot->m_left);
	freeMemory(_subroot->m_right);

	delete _subroot;
}

template<typename Key, typename Value, typename Comparator>
void Map<Key, Value, Comparator>::printDFS(int _choice, Node<Key, Value>* _subroot) const
{
	if (_subroot == nullptr)
		return;

	if (_choice == 1)
		std::cout << _subroot->key << ": " << _subroot->value << "\n";

	this->printDFS(_choice, _subroot->m_left);

	if (_choice == 2)
		std::cout << _subroot->key << ": " << _subroot->value << "\n";

	this->printDFS(_choice, _subroot->m_right);

	if (_choice == 3)
		std::cout << _subroot->key << ": " << _subroot->value << "\n";
}

template<typename Key, typename Value, typename Comparator>
void Map<Key, Value, Comparator>::printBFS(Node<Key, Value>* _subroot) const
{
	typedef std::pair<Node<Key, Value>*, int> nodelevel;

	std::queue<nodelevel> queue;
	int level = 0;

	queue.push({ _subroot, level });

	while (!queue.empty())
	{
		nodelevel current = queue.front();
		queue.pop();

		if (current.second > level)
		{
			level++;
			std::cout << "\n";
		}

		std::cout << current.first->key << ": " << current.first->value << "\n";

		if (current.first->m_left != nullptr)
			queue.push({ current.first->m_left, level + 1 });

		if (current.first->m_right != nullptr)
			queue.push({ current.first->m_right, level + 1 });
	}
}

template<typename Key, typename Value, typename Comparator>
void Map<Key, Value, Comparator>::insert_repair(Node<Key, Value>* _node)
{
	for (; _node != nullptr; _node = _node->m_parent)
	{
		int right_height = _node->m_right == nullptr ? 0 : _node->m_right->m_height;
		int left_height = _node->m_left == nullptr ? 0 : _node->m_left->m_height;

		int balance_factor = right_height - left_height;
		int child_balance_factor = 0;

		_node->m_height = 1 + std::max(right_height, left_height);

		Node<Key, Value>* child = nullptr;
		int balance_factor_after_rotation;

		if (balance_factor == 0)
			return;

		if (balance_factor == 2)
			child = _node->m_right;

		else if (balance_factor == -2)
			child = _node->m_left;

		else continue;

		if (child != nullptr)
		{
			right_height = child->m_right == nullptr ? 0 : child->m_right->m_height;
			left_height = child->m_left == nullptr ? 0 : child->m_left->m_height;

			child_balance_factor = right_height - left_height;
		}

		if (balance_factor == 2)
			if (child_balance_factor == -1)
				balance_factor_after_rotation = this->LR(_node);

			else
				balance_factor_after_rotation = this->LL(_node);

		else
			if (child_balance_factor == 1)
				balance_factor_after_rotation = this->RL(_node);

			else
				balance_factor_after_rotation = this->RR(_node);

		if (!balance_factor_after_rotation)
			return;
	}
}

template<typename Key, typename Value, typename Comparator>
void Map<Key, Value, Comparator>::delete_repair(Node<Key, Value>* _node)
{
	for (; _node != nullptr; _node = _node->m_parent)
	{
		int right_height = _node->m_right == nullptr ? 0 : _node->m_right->m_height;
		int left_height = _node->m_left == nullptr ? 0 : _node->m_left->m_height;

		int balance_factor = right_height - left_height;
		int child_balance_factor = 0;

		_node->m_height = 1 + std::max(right_height, left_height);

		Node<Key, Value>* child = nullptr;
		int balance_factor_after_rotation;

		if (balance_factor == 2)
			child = _node->m_right;

		else if (balance_factor == -2)
			child = _node->m_left;

		else continue;

		if (child != nullptr)
		{
			right_height = child->m_right == nullptr ? 0 : child->m_right->m_height;
			left_height = child->m_left == nullptr ? 0 : child->m_left->m_height;

			child_balance_factor = right_height - left_height;
		}

		if (balance_factor == 2)
			if (child_balance_factor == -1)
				balance_factor_after_rotation = this->LR(_node);

			else
				balance_factor_after_rotation = this->LL(_node);

		else
			if (child_balance_factor == 1)
				balance_factor_after_rotation = this->RL(_node);

			else
				balance_factor_after_rotation = this->RR(_node);

		if (balance_factor_after_rotation)
			return;
	}
}

template<typename Key, typename Value, typename Comparator>
int Map<Key, Value, Comparator>::LR(Node<Key, Value>* _node)
{
	this->RR(_node->m_left);
	return this->LL(_node);
}

template<typename Key, typename Value, typename Comparator>
int Map<Key, Value, Comparator>::RL(Node<Key, Value>* _node)
{
	this->LL(_node->m_right);
	return this->RR(_node);
}

template<typename Key, typename Value, typename Comparator>
int Map<Key, Value, Comparator>::LL(Node<Key, Value>* _node)
{
	Node<Key, Value>* right_child = _node->m_right;

	_node->m_right = right_child->m_left;

	if (right_child->m_left != nullptr)
		right_child->m_left->m_parent = _node;

	right_child->m_parent = _node->m_parent;

	if (_node->m_parent == nullptr)
		this->m_root = right_child;

	else if (_node == _node->m_parent->m_right)
		_node->m_parent->m_right = right_child;

	else
		_node->m_parent->m_left = right_child;

	right_child->m_left = _node;
	_node->m_parent = right_child;

	int right_height = _node->m_right == nullptr ? 0 : _node->m_right->m_height;
	int left_height = _node->m_left == nullptr ? 0 : _node->m_left->m_height;

	_node->m_height = 1 + std::max(right_height, left_height);

	right_height = right_child->m_right == nullptr ? 0 : right_child->m_right->m_height;
	left_height = right_child->m_left == nullptr ? 0 : right_child->m_left->m_height;

	right_child->m_height = 1 + std::max(right_height, left_height);

	return right_height - left_height;
}

template<typename Key, typename Value, typename Comparator>
int Map<Key, Value, Comparator>::RR(Node<Key, Value>* _node)
{
	Node<Key, Value>* left_child = _node->m_left;

	_node->m_left = left_child->m_right;

	if (left_child->m_right != nullptr)
		left_child->m_right->m_parent = _node;

	left_child->m_parent = _node->m_parent;

	if (_node->m_parent == nullptr)
		this->m_root = left_child;

	else if (_node == _node->m_parent->m_right)
		_node->m_parent->m_right = left_child;

	else
		_node->m_parent->m_left = left_child;

	left_child->m_right = _node;
	_node->m_parent = left_child;

	int right_height = _node->m_right == nullptr ? 0 : _node->m_right->m_height;
	int left_height = _node->m_left == nullptr ? 0 : _node->m_left->m_height;

	_node->m_height = 1 + std::max(right_height, left_height);

	right_height = left_child->m_right == nullptr ? 0 : left_child->m_right->m_height;
	left_height = left_child->m_left == nullptr ? 0 : left_child->m_left->m_height;

	left_child->m_height = 1 + std::max(right_height, left_height);

	return right_height - left_height;
}

template<typename Key, typename Value, typename Comparator>
void Map<Key, Value, Comparator>::deep_copy(Node<Key, Value>* _subroot)
{
	std::queue<Node<Key, Value>*> frontier;
	frontier.push(_subroot);

	while (!frontier.empty())
	{
		Node<Key, Value>* node = frontier.front();
		frontier.pop();

		this->insert({ node->key, node->value });

		if (node->m_left != nullptr)
			frontier.push(node->m_left);

		if (node->m_right != nullptr)
			frontier.push(node->m_right);
	}
}

template<typename Key, typename Value, typename Comparator>
Map<Key, Value, Comparator>::Map() :
	m_root(nullptr), m_comparator(Comparator()), m_size(0) {}

template<typename Key, typename Value, typename Comparator>
Map<Key, Value, Comparator>::Map(const Map<Key, Value, Comparator>& _tree)
{
	this->m_size = 0;
	this->m_root = nullptr;
	this->m_comparator = Comparator();
	this->deep_copy(_tree.getRoot());
}

template<typename Key, typename Value, typename Comparator>
Map<Key, Value, Comparator>::Map(const std::vector<std::pair<Key, Value>>& _keyvals) : Map()
{
	this->m_size = 0;
	this->m_root = nullptr;
	this->m_comparator = Comparator();

	for (int i = 0; i < _keyvals.size(); ++i)
		this->insert(_keyvals[i]);
}

template<typename Key, typename Value, typename Comparator>
Map<Key, Value, Comparator>::~Map()
{
	this->clear();
}

template<typename Key, typename Value, typename Comparator>
Map<Key, Value, Comparator>& Map<Key, Value, Comparator>::operator=(const Map<Key, Value, Comparator>& _tree)
{
	if (this == &_tree)
		return *this;

	this->clear();
	this->deep_copy(_tree.getRoot());

	return *this;
}

template<typename Key, typename Value, typename Comparator>
typename Map<Key, Value, Comparator>::iterator Map<Key, Value, Comparator>::begin()
{
	return iterator(*this, this->min(this->m_root), true);
}

template<typename Key, typename Value, typename Comparator>
typename Map<Key, Value, Comparator>::iterator Map<Key, Value, Comparator>::rbegin()
{
	return iterator(*this, this->max(this->m_root), false);
}

template<typename Key, typename Value, typename Comparator>
typename Map<Key, Value, Comparator>::iterator Map<Key, Value, Comparator>::end()
{
	return iterator(*this, nullptr, true);
}

template<typename Key, typename Value, typename Comparator>
typename Map<Key, Value, Comparator>::iterator Map<Key, Value, Comparator>::rend()
{
	return iterator(*this, nullptr, false);
}

template<typename Key, typename Value, typename Comparator>
Node<Key, Value>* Map<Key, Value, Comparator>::max(Node<Key, Value>* _subroot) const
{
	if (_subroot == nullptr)
		return _subroot;

	while (_subroot->m_right != nullptr)
		_subroot = _subroot->m_right;

	return _subroot;
}

template<typename Key, typename Value, typename Comparator>
Node<Key, Value>* Map<Key, Value, Comparator>::min(Node<Key, Value>* _subroot) const
{
	if (_subroot == nullptr)
		return _subroot;

	while (_subroot->m_left != nullptr)
		_subroot = _subroot->m_left;

	return _subroot;
}

template<typename Key, typename Value, typename Comparator>
Node<Key, Value>* Map<Key, Value, Comparator>::successor(Node<Key, Value>* _node) const
{
	if (_node == nullptr)
		return nullptr;

	if (_node->m_right != nullptr)
		return this->min(_node->m_right);

	Node<Key, Value>* _successor = _node->m_parent;

	while (_successor != nullptr && _node == _successor->m_right)
	{
		_node = _successor;
		_successor = _successor->m_parent;
	}

	return _successor;
}

template<typename Key, typename Value, typename Comparator>
Node<Key, Value>* Map<Key, Value, Comparator>::predecessor(Node<Key, Value>* _node) const
{
	if (_node == nullptr)
		return nullptr;

	if (_node->m_left != nullptr)
		return this->max(_node->m_left);

	Node<Key, Value>* _predecessor = _node->m_parent;

	while (_predecessor != nullptr && _node == _predecessor->m_left)
	{
		_node = _predecessor;
		_predecessor = _predecessor->m_parent;
	}

	return _predecessor;
}

template<typename Key, typename Value, typename Comparator>
Value& Map<Key, Value, Comparator>::operator[](const Key& _key)
{
	Node<Key, Value>* node = this->find(_key);

	if (node == nullptr)
	{
		this->insert(_key, 0);
		node = this->find(_key);
	}

	return node->value;
}

template<typename Key, typename Value, typename Comparator>
void Map<Key, Value, Comparator>::insert(Key _key, Value _value)
{
	if (this->find(_key) != nullptr)
		return;

	this->m_size++;

	Node<Key, Value>* new_node = new Node<Key, Value>(nullptr, _key, _value, nullptr, nullptr);

	Node<Key, Value>* first = this->m_root;
	Node<Key, Value>* second = nullptr;

	while (first != nullptr)
	{
		second = first;

		if (this->m_comparator(new_node->key, first->key))
			first = first->m_left;

		else
			first = first->m_right;
	}

	new_node->m_parent = second;

	if (second == nullptr)
		this->m_root = new_node;

	else
		if (this->m_comparator(new_node->key, second->key))
			second->m_left = new_node;

		else
			second->m_right = new_node;

	this->insert_repair(new_node->m_parent);
}

template<typename Key, typename Value, typename Comparator>
void Map<Key, Value, Comparator>::insert(std::pair<Key, Value> _keyval)
{
	this->insert(_keyval.first, _keyval.second);
}

template<typename Key, typename Value, typename Comparator>
Node<Key, Value>* Map<Key, Value, Comparator>::find(Key _key) const
{
	Node<Key, Value>* node = this->m_root;

	while (node != nullptr && node->key != _key)
	{
		if (this->m_comparator(_key, node->key))
			node = node->m_left;

		else
			node = node->m_right;
	}

	return node;
}

template<typename Key, typename Value, typename Comparator>
void Map<Key, Value, Comparator>::remove(Key _key)
{
	this->erase(this->find(_key));
}

template<typename Key, typename Value, typename Comparator>
void Map<Key, Value, Comparator>::erase(Node<Key, Value>* _node)
{
	if (_node == nullptr)
		return;

	if (this->find(_node->key) == nullptr)
		return;

	this->m_size--;

	Node<Key, Value>* deleted_node = _node;
	Node<Key, Value>* start_repair = _node->m_parent;

	if (_node->m_left == nullptr)
		this->transplant(_node, _node->m_right);

	else if (_node->m_right == nullptr)
		this->transplant(_node, _node->m_left);

	else
	{
		Node<Key, Value>* node = this->successor(_node);

		if (node != _node->m_right)
		{
			this->transplant(node, node->m_right);
			node->m_right = _node->m_right;
			_node->m_right->m_parent = node;
		}

		this->transplant(_node, node);
		node->m_left = _node->m_left;
		_node->m_left->m_parent = node;

		start_repair = node->m_parent;
	}

	delete deleted_node;
	this->delete_repair(start_repair);
}

template<typename Key, typename Value, typename Comparator>
void Map<Key, Value, Comparator>::print(int _choice) const
{
	if (this->m_root == nullptr)
		return;

	switch (_choice)
	{
	case 1:
	case 2:
	case 3:
		this->printDFS(_choice, this->m_root);
		break;
	case 4:
		this->printBFS(this->m_root);
		break;
	default:
		this->printDFS(2, this->m_root);
		break;
	}

	std::cout << std::endl;
}

template<typename Key, typename Value, typename Comparator>
bool Map<Key, Value, Comparator>::empty() const
{
	return this->m_root == nullptr;
}

template<typename Key, typename Value, typename Comparator>
void Map<Key, Value, Comparator>::clear()
{
	this->freeMemory(this->m_root);
	this->m_root = nullptr;
	this->m_size = 0;
}

template<typename Key, typename Value, typename Comparator>
Node<Key, Value>* Map<Key, Value, Comparator>::getRoot() const
{
	return this->m_root;
}

template<typename Key, typename Value, typename Comparator>
size_t Map<Key, Value, Comparator>::size()
{
	return this->m_size;
}
