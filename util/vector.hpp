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
		for (int i = 0; i < (v.size()-1); ++i)
			out << v.at(i) << ", ";
		out << v.back() << "]";
		return out;
	}
}

template <typename T>
std::ostream& operator<< (std::ostream& out, 
				std::vector< std::vector<T> > const& v) {

	if (v.size() == 0) {
		out << "[]";
		return out;
	}
	if (v.at(0).size() == 0) {
		out << "[";
		for (int i = 0; i < v.size()-1; ++i) {
			out << "[], ";
		}
		out << v.back() << "]";
		return out;
	}
	out << "[";
	for (int j = 0; j < v.at(0).size(); ++j) {
		for (int i = 0; i < v.size()-1; ++i) {
			out << v[i][j] << ", "; 
		}
		out << v.back()[j] << "]" << std::endl << "[";
	}
	return out;
}

