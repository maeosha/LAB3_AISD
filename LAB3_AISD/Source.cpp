#include <vector>
#include <fstream>
#include <iostream>
#include <random>


enum SortMethod {
	enum_sort_by_inserts = 1,
	enum_shaker_sort = 2,
	enum_comb_sort = 3
};

enum ArrayType {
	random_type = 1,
	sorted_type = 2,
	rsorted_type = 3
};


struct stats {
	size_t comparison_count = 0;
	size_t copy_count = 0;
};


stats sort_by_inserts(std::vector<int>::iterator start, std::vector<int>::iterator end) {
	stats stats;

	for (auto border = start + 1; border < end; ++border) {
		const int key = *border;
		auto border_elem = border - 1;
		while (border_elem >= start) {
			if (*border_elem <= key) {
				break;
			}
			else {
				*(border_elem + 1) = *border_elem;
				*border_elem = key;
				stats.comparison_count += 2;
			}
			if (border_elem != start) {
				border_elem--;
			}
			stats.copy_count++;
		}
	}

	return stats;
}

stats shaker_sort(std::vector<int>::iterator start, std::vector<int>::iterator end) {
	stats stats;
	bool swaped = 0;
	do {
		swaped = 1;
		auto index = start;
		while (index < end - 1) {
			if (*index > *(index + 1)) {
				auto tmp_index = *(index + 1);
				*(index + 1) = *index;
				*index = tmp_index;
				swaped = 0;
				stats.copy_count++;
				stats.comparison_count += 4;
			}
			index++;
		}
		end--;
		index = end - 1;
		while (index > start) {
			if (*index < *(index - 1)) {
				auto tmp_index = *(index - 1);
				*(index - 1) = *index;
				*index = tmp_index;
				swaped = 0;
				stats.comparison_count += 4;
				stats.copy_count++;
			}

			index--;
		}
		start++;
	} while (swaped == 0);
	
	return stats;
}


stats comb_sort(std::vector<int>::iterator start, std::vector<int>::iterator end, const size_t size) {
	stats stats;
	size_t step = size;
	while (step > 1) {
		step = step / 1.247f;
		auto  index = start;
		for (; index < end - step + 1; index++) {
			auto elem = *(index + step - 1);
			if (*index > *(index + step - 1)) {
				auto tmp_elem = *(index + step - 1);
				*(index + step - 1) = *index;
				*index = tmp_elem;
				stats.comparison_count += 3;
				stats.copy_count++;
			}
		}
	}
	return stats;
}


stats get_average_stats(const size_t size, const int sort_method, const int array_type) {
	stats sorted_stats;
	std::vector<int> sort_array;

	for (size_t count = 0; count < 2; count++) {
		stats tmp_stats;
		sort_array.clear();

		switch (array_type) {
		case random_type:
			for (size_t index = 0; index < size; index++) {
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dist(-1000, 1000);
				sort_array.push_back(dist(gen));
				
			}
			break;

		case sorted_type:
			for (size_t index = 0; index < size; index++) {
				sort_array.push_back(index);
			}
			break;

		case rsorted_type:
			for (int index = size; index >= 0; index--) {
				sort_array.push_back(index);
			}
			break;

		}

		auto start = sort_array.begin();
		auto end = sort_array.end();


		switch (sort_method) {
		case enum_sort_by_inserts:
			tmp_stats = sort_by_inserts(start, end);
			break;
		case enum_shaker_sort:
			tmp_stats = shaker_sort(start, end);
			break;

		case enum_comb_sort:
			tmp_stats = comb_sort(start, end, sort_array.size());
			break;
		}
		sort_array.clear();

		sorted_stats.comparison_count += tmp_stats.comparison_count;
		sorted_stats.copy_count += tmp_stats.copy_count;
	}
	
	sorted_stats.comparison_count /= 2;
	sorted_stats.copy_count /= 2;

	return sorted_stats;
}

void start_average_sort(const int sort_method) {
	std::vector<stats> average_stats;
	std::vector<size_t> size;


	for (size_t tmp_size = 1; tmp_size <= 10; tmp_size++) {
		average_stats.push_back(get_average_stats(tmp_size * 1000, sort_method, 3));
		size.push_back(tmp_size);
	}


	for (size_t tmp_size = 25; tmp_size <= 24; tmp_size * 2) {
		average_stats.push_back(get_average_stats(tmp_size * 1000, sort_method, 3));
		size.push_back(tmp_size);
	}


	std::ofstream fout("stats.txt", std::ios_base::app);


	switch (sort_method) {
	case enum_sort_by_inserts:
		fout << "Sort by inserts: " << std::endl;;
		break;
	case enum_shaker_sort:
		fout << "Shaker sort: " << std::endl;;
		break;
	case enum_comb_sort: 
		fout << "Comb sort: " << std::endl;;
		break;
	}

	fout << "(Array size, average comparison count): " << std::endl;
	for (auto index = 0; index < average_stats.size(); index++) {
		fout << "(" << size[index] << ", " << average_stats[index].comparison_count << ")" << std::endl;
	}

	fout << std::endl;

	fout << "(Array size, average copy count): " << std::endl;
	for (auto index = 0; index < average_stats.size(); index++) {
		fout << "(" << size[index] << ", " << average_stats[index].copy_count << ")" << std::endl;
	}

	fout.close();
}


int main() {
	start_average_sort(3);
}
