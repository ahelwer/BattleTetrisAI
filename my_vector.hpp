#pragma once

#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<< (std::ostream& out, std::vector<T> const& v) {
	out << "[";
	for (unsigned i = 0; i < v.size()-1; ++i)
		out << v[i] << ", ";
	out << v[v.size()-1] << "]";
	return out;
}

