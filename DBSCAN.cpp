#include <stdio.h>
#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <string>
#include <iostream>

struct Point
{
	double x;
	double y;
	
	std::string toString() {
		return ("{" + std::to_string(x) + ", " + std::to_string(y) + "}");
	}
};

constexpr int UNDEFINED = 0;
constexpr int NOISE = -1;

auto distFunc = [](const Point& p, const Point& q)
{
	return sqrt(
		pow(p.x - q.x, 2) + pow(p.y - q.y, 2)
	);
};

template<typename DistanceFunction>
std::vector<int> RangeQuery(const std::vector<Point>& DB, const DistanceFunction distFunc, const Point& q, double eps)
{
	std::vector<int> neighbors;
	
	for (uint32_t i = 0; i < DB.size(); ++i)
	{
		if (distFunc(DB[i], q) <= eps)
		{
			neighbors.push_back(i);
		}
	}
	
	return neighbors;
}

// New Function to union two vectors, regular set_union() didn't work
std::vector<int> vector_union(std::vector<int> a, const std::vector<int>& b) {
	std::sort(a.begin(), a.end());
	std::vector<int> sorted_b = b;
	std::sort(sorted_b.begin(), sorted_b.end());
	std::vector<int> result;
	std::set_union(a.begin(), a.end(), sorted_b.begin(), sorted_b.end(), std::back_inserter(result));
	return result;
}

template<typename DistanceFunction>
std::vector<int> DBSCAN(
	const std::vector<Point>& DB,
	DistanceFunction distFunc,
	const double eps,
	const uint32_t minPts
) {
	std::vector<int> labels(DB.size(), UNDEFINED);
	uint32_t C = 0;

	for (uint32_t p = 0U; p < DB.size(); ++p)
	{
		if (labels[p] != UNDEFINED)
		{
			continue;
		}

		std::vector<int> neighbors = RangeQuery(DB, distFunc, DB[p], eps);

		if (neighbors.size() < minPts)
		{
			labels[p] = NOISE;
			continue;
		}

		C += 1;
		labels[p] = C;

		while (!neighbors.empty())
		{
			auto q = neighbors.back();
			neighbors.pop_back();

			if (labels[q] == NOISE)
			{
				labels[q] = C;
			}

			if (labels[q] != UNDEFINED)
			{
				continue;
			}

			labels[q] = C;

			auto neighbors_of_neighbors = RangeQuery(DB, distFunc, DB[q], eps);

			if (neighbors_of_neighbors.size() >= minPts)
			{
				neighbors = vector_union(neighbors, neighbors_of_neighbors);
			}
		}
	}

	return labels;
}

int main(int argc, char const *argv[]) {
	
	std::vector<Point> DB;
	
	// Custom Points passed as args
	if (argc > 1)
	{
		std::cout << "-=: CUSTOM DATA :=-\n";
		DB.clear();
		
		// Check if the right number of args are provided
		if ((argc - 1) % 2 != 0)
		{
			std::cerr << "Error: Please provide an even number of coordinates.\nExample for the Points {100.62, 20} {-36, -89.54}:\n100.62 20 -36 -89.54\n";
			return 1;
		}
		
		for (int i = 1; i < argc; i += 2)
		{
			try {
				double x = std::stod(argv[i]);
				double y = std::stod(argv[i+1]);
				DB.push_back({x, y});
			} catch (const std::exception& e) {
				std::cerr << "Error parsing argument '" << argv[i] << "' or '" << argv[i+1] << "': " << e.what() << '\n';
				return 1;
			}
		}
	}
	
	// Default Example DB
	else
	{	
		std::cout << "-=: EXAMPLE DATA :=-\n";
		DB = {
			{-2.155, -3.195}, {-2.635, -4.605}, {-4.285, -5.025}, {-4.795, -6.825},
			{-3.925, 1.665},
			{1.355, 4.005}, {2.975, 4.665}, {2.615, 3.915}, {1.775, 3.345}, {1.895, 4.575}, {2.255, 2.415}, {2.735, 3.105},
			{5.795, -1.275}, {3.275, -4.485}
		};
	}

	const auto eps = 4.0;
	const auto minPts = 3U;
	
	std::cout << "-=: All Points :=-\n";
	for (Point p : DB)
	{
		std::cout << p.toString() << '\n';
	}
	std::cout << "-=: End :=-\n\n";



	auto labels = DBSCAN(DB, distFunc, eps, minPts);
	
	
	
	std::cout << "-=: After DBSCAN, Cluster IDs :=-\n";
	for (int p : labels)
	{
		std::cout << p << '\n';
	}
	std::cout << "-=: End :=-\n";

	return 0;
}
