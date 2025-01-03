#pragma once

#include <utility>

template<typename Map>
class MapIterator
{
public:
	using Key = typename Map::KeyType;
	using Value = typename Map::ValueType;

	using ValueType = typename Map::KeyValueType;

	using PointerType = ValueType*;
	using ReferenceType = ValueType&;

protected:
	PointerType m_ptr;
	Map& m_container;

	bool m_forward;

public:
	MapIterator(const Map& container);
	MapIterator(Map& _container, PointerType _ptr, bool _forward);
	MapIterator(const MapIterator<Map>& _iterator);
	~MapIterator() = default;

	MapIterator<Map> operator=(const MapIterator<Map>& _iterator);

	MapIterator<Map>& operator++();
	MapIterator<Map> operator++(int);
	MapIterator<Map>& operator--();
	MapIterator<Map> operator--(int);

	PointerType operator->();
	ReferenceType operator*();

	bool operator==(const MapIterator<Map>& _iterator);
	bool operator!=(const MapIterator<Map>& _iterator);
};

template<typename Map>
MapIterator<Map>::MapIterator(const Map& _container) :
	MapIterator(_container, nullptr, true) {}

template<typename Map>
MapIterator<Map>::MapIterator(Map& _container, PointerType _ptr, bool _forward) :
	m_container(_container), m_ptr(_ptr), m_forward(_forward) {}

template<typename Map>
MapIterator<Map>::MapIterator(const MapIterator<Map>& _iterator) :
	MapIterator(_iterator.m_container, _iterator.m_ptr, _iterator.m_forward) {}

template<typename Map>
MapIterator<Map> MapIterator<Map>::operator=(const MapIterator<Map>& _iterator)
{
	this->m_forward = _iterator.m_forward;
	this->m_ptr = _iterator.m_ptr;
}

template<typename Map>
MapIterator<Map>& MapIterator<Map>::operator++()
{
	this->m_ptr = (this->m_forward ? m_container.successor(this->m_ptr) : m_container.predecessor(this->m_ptr));
	return *this;
}

template<typename Map>
MapIterator<Map> MapIterator<Map>::operator++(int)
{
	MapIterator copy = *this;
	this->m_ptr = (this->m_forward ? m_container.successor(this->m_ptr) : m_container.predecessor(this->m_ptr));
	return copy;
}

template<typename Map>
MapIterator<Map>& MapIterator<Map>::operator--()
{
	this->m_ptr = (this->m_forward ? m_container.predecessor(this->m_ptr) : m_container.successor(this->m_ptr));
	return *this;
}

template<typename Map>
MapIterator<Map> MapIterator<Map>::operator--(int)
{
	MapIterator copy = *this;
	this->m_ptr = (this->m_forward ? m_container.predecessor(this->m_ptr) : m_container.successor(this->m_ptr));
	return copy;
}

template<typename Map>
typename MapIterator<Map>::PointerType MapIterator<Map>::operator->()
{
	return this->m_ptr;
}

template<typename Map>
typename MapIterator<Map>::ReferenceType MapIterator<Map>::operator*()
{
	return *(this->m_ptr);
}

template<typename Map>
bool MapIterator<Map>::operator==(const MapIterator<Map>& _iterator)
{
	return this->m_ptr == _iterator.m_ptr;
}

template<typename Map>
bool MapIterator<Map>::operator!=(const MapIterator<Map>& _iterator)
{
	return !(*this == _iterator);
}