#pragma once

template<typename Key>
class DefaultComparator;

template<typename Key, typename Value, typename Comparator = DefaultComparator<Key>>
class Map;

template<typename Key, typename Value>
class Node
{
public:
	Key key;
	Value value;

	friend class Map<Key, Value>;

protected:
	Node<Key, Value>* m_parent;
	
	Node<Key, Value>* m_left;
	Node<Key, Value>* m_right;

	size_t m_height;

public:
	Node(Node<Key, Value>* _parent, Key _key, Value _value, Node<Key, Value>* _left, Node<Key, Value>* _right) :
		m_parent(_parent), key(_key), value(_value), m_left(_left), m_right(_right), m_height(1) {}
};

template<typename Key>
class DefaultComparator
{
public:
	bool operator()(const Key& a, const Key& b) const
	{
		return a < b;
	}
};