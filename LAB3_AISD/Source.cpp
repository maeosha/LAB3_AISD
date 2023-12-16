#include <vector>
#include <fstream>
#include <iostream>
#include <random>


enum SortMethod {
	enum_sort_by_inserts = 1,
	enum_shaker_sort = 2,
	enum_comb_sort = 3
};


struct stats {
	size_t comparison_count = 0;
	size_t copy_count = 0;
};


stats sort_by_inserts(std::vector<int>::iterator start, std::vector<int>::iterator end) {
	stats stats;

	for (auto border = start + 1; border < end; border++) {
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
	stats.comparison_count++;
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
	stats.comparison_count++;
	while (step > 1) {
		step = step / 1.247f;
		auto  index = start;
		auto end_elem = start + step - 1;
		for (; index < end - step + 1; index++) {
			auto elem = *(index + step - 1);
			stats.comparison_count++;
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


void average_stats(const size_t size, const int sort_method) {
	size_t average_comparison_count;
	size_t average_copy_count;
	stats sorted_stats;
	std::ofstream fout("stats.txt", std::ios_base::app);

	fout << "(Array size, average comparison count) (Array size, average copy count)" << std::endl;

	
	for (size_t count = 0; count < 50; count++) {
		stats tmp_stats;
		std::vector<int> sort_array;

		for (size_t index = 0; index < size; index++) {
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dist(-1000, 1000);
			sort_array.push_back(dist(gen));
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
	
	average_comparison_count = sorted_stats.comparison_count / 10;
	average_copy_count = sorted_stats.copy_count / 10;

	fout << "(" << size << ", " << average_comparison_count << ") ";
	fout << "(" << size << ", " << average_copy_count << ")" << sort_method << std::endl;

	fout.close();

	std::cout << "completed!";

}

int main() {
	average_stats(1000, 1);
	average_stats(1000, 2);
	average_stats(1000, 3);
}
