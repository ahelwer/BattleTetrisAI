#pragma once

#include <iostream>
#include <vector>

template <typename T>
std::ostream& operator<< (std::ostream& out, std::vector<T> const& v) {
    int size = v.size();
	if (size == 0) {
		out << "[]";
		return out;
	}
	else {
		out << "[";
		for (int i = 0; i < (size-1); ++i)
			out << v.at(i) << ", ";
		out << v.back() << "]";
		return out;
	}
}

template <typename T>
std::ostream& operator<< (std::ostream& out, 
				std::vector< std::vector<T> > const& v) {

    int firstSize = v.size();
	if (firstSize == 0) {
		out << "[]";
		return out;
	}
    int secondSize = v.at(0).size();
	if (secondSize == 0) {
		out << "[";
		for (int i = 0; i < firstSize-1; ++i) {
			out << "[], ";
		}
		out << v.back() << "]";
		return out;
	}
	out << "[";
	for (int j = 0; j < secondSize; ++j) {
		for (int i = 0; i < firstSize-1; ++i) {
			out << v[i][j] << ", "; 
		}
		out << v.back()[j] << "]" << std::endl << "[";
	}
	return out;
}

/*
template <typename T, typename P>
std::ostream& operator<< (std::ostream& out, std::pair<T, P> const& pair) {
    out << "(" << pair.first << ", " << pair.second << ")";
    return out;
}
*/

