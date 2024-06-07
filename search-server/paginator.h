#pragma once

#include <cstdio>
#include <iostream>
#include <vector>

template<typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin,Iterator end) : 
        begin_(begin), end_(end), size_(std::distance(begin_, end_))  {}

    Iterator begin() const  {
        return begin_;
    }

    Iterator end() const {
        return end_;
    }

    size_t size() const {
        return size_;
    }


private:
    Iterator begin_;
    Iterator end_;
    size_t size_;
};

template<typename Iterator>
class Paginator {
public:
    Paginator(Iterator begin, Iterator end, size_t page_size);

    auto begin() const {
        return pages_.begin();
    }

    auto end() const {
        return pages_.end();
    }

    size_t size() const {
        return pages_.size();
    }

private:
    std::vector<IteratorRange<Iterator>>  pages_;
};

template<typename Iterator>
Paginator<Iterator>::Paginator(Iterator begin, Iterator end, size_t page_size) {
    while (begin != end) {
        if (std::distance(begin, end) > static_cast<int>(page_size)) {
            auto next_it = std::next(begin, page_size);
            pages_.push_back({begin, next_it});
            advance(begin, page_size);
        } else {
            pages_.push_back({begin, end});
            begin = end;
        }
    }
}

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}

template<typename Iterator>
std::ostream& operator<<(std::ostream& out, const IteratorRange<Iterator>& range) {
    for (auto it = range.begin(); it != range.end(); ++it) {
        out << *it;
    }
    return out; 
}