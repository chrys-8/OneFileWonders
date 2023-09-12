#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

void print_range(
	const std::ranges::forward_range auto range,
	const char *delim = " ",
	const char *newline = "\n"
) {
	using value_type = std::iter_value_t<std::ranges::iterator_t<decltype(range)>>;
	std::ranges::copy(range, std::ostream_iterator<value_type>(std::cout, delim));
	std::cout << newline;
}

void heapCascadeUp(std::ranges::bidirectional_range auto &&heap)
{
	namespace ranges = std::ranges;

	auto begin    = ranges::begin(heap);
	auto end      = ranges::end(heap);
	auto itActive = end - 1;

	auto index      = [&]() { return ranges::distance(begin, itActive); };
	auto hasParent  = [&]() { return index() > 0; };
	auto nextParent = [&]() { return begin + (index() - 1)/2; };

	auto itParent = nextParent();
	while (*itActive > *itParent) {
		std::iter_swap(itActive, itParent);
		itActive = itParent;
		if (!hasParent()) return;
		itParent = nextParent();
	}
}

void heapSort(std::ranges::bidirectional_range auto &&storage)
{
	namespace ranges = std::ranges;
	for (auto it = ranges::begin(storage); it != ranges::end(storage); it++)
		heapCascadeUp(ranges::subrange(ranges::begin(storage), it + 1));
}

template <typename C, typename Elem_T>
concept BackInsertable = requires (C c, Elem_T e) {
	c.push_back(e);
};

template <typename T, typename V>
requires BackInsertable<T, V> && std::ranges::bidirectional_range<T>
void heapInsert(T &heap, V &&value)
{
	heap.push_back(value);
	heapCascadeUp(heap);
}

void heapCascadeDown(std::ranges::bidirectional_range auto &&heap)
{
	namespace ranges = std::ranges;

	auto begin    = ranges::begin(heap);
	auto end      = ranges::end(heap);
	auto size     = ranges::distance(begin, end);
	auto itActive = begin;

	auto index    = [&]() { return ranges::distance(begin, itActive); };
	auto hasChild = [&]() { return 2*index() + 1 < size; };

	auto maxChild = [&]() {
		auto leftIndex  = 2*index() + 1;
		auto leftChild  = leftIndex < size ? begin + leftIndex : itActive;
		auto rightIndex = 2*index() + 2;
		auto rightChild = rightIndex < size ? begin + rightIndex : itActive;
		return *leftChild > *rightChild ? leftChild : rightChild;
	};

	auto itChild = maxChild();
	while (*itChild > *itActive) {
		std::iter_swap(itChild, itActive);
		itActive = itChild;
		if (!hasChild()) return;
		itChild = maxChild();
	}
}

void heapExtract(std::ranges::bidirectional_range auto &&heap)
{
	namespace ranges = std::ranges;

	auto root = ranges::begin(heap);
	auto last = ranges::end(heap) - 1;
	std::iter_swap(root, last);
	heapCascadeDown(ranges::subrange(root, last));
}

template <typename C>
concept BackExtractable = requires (C c) {
	c.back();
	c.pop_back();
};

template <typename T>
requires BackExtractable<T> && std::ranges::bidirectional_range<T>
auto heapPop(T &&heap)
{
	heapExtract(heap);
	auto back = heap.back();
	heap.pop_back();
	return back;
}

void fn()
{
	auto vec = std::vector<int>{ 9, 34, 13, 5, 2 };

	std::cout << "Input vector: ";
	print_range(vec);

	// make heap
	heapSort(vec);
	std::cout << "Heap result: ";
	print_range(vec);

	// insert into heap
	heapInsert(vec, 58);
	std::cout << "Inserting new value into heap: ";
	print_range(vec);

	// extract from heap
	auto val = heapPop(vec);
	std::cout << "Extracted " << val << " from heap: ";
	print_range(vec);
}

int main()
{
	fn();
}

