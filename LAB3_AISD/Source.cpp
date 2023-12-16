#include <vector>
#include <fstream>
#include <random>


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

stats shaker_sort(std::vector<int>::iterator& start, std::vector<int>::iterator& end) {
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