#pragma once

#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<< (std::ostream& out, std::vector<T> const& v) {
	if (v.size() == 0) {
		out << "[]";
		return out;
	}
	else {
		out << "[";
		for (unsigned i = 0; i < (v.size()-1); ++i)
			out << v.at(i) << ", ";
		out << v.at(v.size()-1) << "]";
		return out;
	}
}

template <typename T>
std::ostream& operator<< (std::ostream& out, 
				std::vector< std::vector<T> > const& v) {
	for (unsigned i = 0; i < v.size(); ++i)
		out << v[i] << std::endl;
	return out;
}

