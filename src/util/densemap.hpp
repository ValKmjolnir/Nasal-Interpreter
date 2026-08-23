#pragma once

#include <cstdint>
#include <vector>
#include <utility>
#include <stdexcept>
#include <functional>

namespace nasal::util {

template <typename K, typename V, typename Hash = std::hash<K>>
class densemap {
public:
    class iterator {
    public:
        using value_type = std::pair<K, V>;
        using reference = value_type&;
        using pointer = value_type*;

    private:
        friend class densemap;
        value_type* data_;

    public:

        iterator(value_type* d) : data_(d) {}

        bool operator==(const iterator& other) const {
            return data_ == other.data_;
        }
        bool operator!=(const iterator& other) const {
            return data_ != other.data_;
        }
        reference operator*() const {
            return *data_;
        }
        pointer operator->() const {
            return data_;
        }
        iterator& operator++() {
            ++ data_;
            return *this;
        }
        iterator operator++(int) {
            auto t = *this;
            ++ (*this);
            return t;
        }
    };

    class const_iterator {
    public:
        using value_type = std::pair<K, V>;
        using reference = const value_type&;
        using pointer = const value_type*;

    private:
        friend class densemap;
        const value_type* data_;

    public:

        const_iterator(const value_type* d) : data_(d) {}

        bool operator==(const const_iterator& other) const {
            return data_ == other.data_;
        }
        bool operator!=(const const_iterator& other) const {
            return data_ != other.data_;
        }
        reference operator*() const {
            return *data_;
        }
        pointer operator->() const {
            return data_;
        }
        const_iterator& operator++() {
            ++ data_;
            return *this;
        }
        const_iterator operator++(int) {
            auto t = *this;
            ++ (*this);
            return t;
        }
    };

private:
    std::vector<std::pair<K, V>> entries_;
    std::vector<std::int32_t> slots_;

    static constexpr std::int32_t EMPTY = -1;
    static constexpr std::int32_t DELETED = -2;

    std::uint64_t size_;
    std::uint64_t capacity_;

private:
    void expand() {
        capacity_ *= 2;

        std::vector<std::int32_t> old_slots = std::move(slots_);
        slots_.assign(capacity_, EMPTY);

        for (std::uint64_t i = 0; i < old_slots.size(); i ++) {
            if (old_slots[i] == EMPTY || old_slots[i] == DELETED) {
                continue;
            }
            const std::uint64_t h = Hash{}(entries_[old_slots[i]].first);
            std::uint64_t index = h & (capacity_ - 1);

            while (slots_[index] != EMPTY) {
                index ++;
                if (index >= capacity_) {
                    index = 0;
                }
            }

            slots_[index] = old_slots[i];
        }
    }

public:
    densemap() : size_(0), capacity_(8) {
        slots_.resize(capacity_, EMPTY);
    }

    densemap(densemap&& other) :
        size_(other.size_),
        capacity_(other.capacity_),
        entries_(std::move(other.entries_)),
        slots_(std::move(other.slots_)) {
        other.size_ = 0;
        other.capacity_ = 8;
        other.entries_.clear();
        other.slots_.assign(8, EMPTY);
    }

    densemap& operator=(densemap&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        size_ = other.size_;
        capacity_ = other.capacity_;
        entries_ = std::move(other.entries_);
        slots_ = std::move(other.slots_);
        other.size_ = 0;
        other.capacity_ = 8;
        other.entries_.clear();
        other.slots_.assign(8, EMPTY);
        return *this;
    }

    densemap(const densemap& other) :
        size_(other.size_),
        capacity_(other.capacity_),
        entries_(other.entries_),
        slots_(other.slots_) {}

    densemap& operator=(const densemap& other) {
        if (this == &other) {
            return *this;
        }
        size_ = other.size_;
        capacity_ = other.capacity_;
        entries_ = other.entries_;
        slots_ = other.slots_;
        return *this;
    }

    void clear() {
        size_ = 0;
        entries_.clear();
        slots_.assign(capacity_, EMPTY);
    }

    bool empty() const {
        return size_ == 0;
    }

    const auto& size() const {
        return size_;
    }

    bool contains(const K& key) const {
        const std::uint64_t h = Hash{}(key);
        std::uint64_t index = h & (capacity_ - 1);

        while (slots_[index] != EMPTY) {
            if (slots_[index] == DELETED) {
                index ++;
                if (index >= capacity_) {
                    index = 0;
                }
                continue;
            }
            std::int32_t entry = slots_[index];
            if (entries_[entry].first == key) {
                return true;
            }
            index ++;
            if (index >= capacity_) {
                index = 0;
            }
        }
        return false;
    }

    void insert(const K& key, const V& value) {
        const std::uint64_t h = Hash{}(key);
        std::uint64_t index = h & (capacity_ - 1);

        std::uint64_t deleted_index = UINT64_MAX;
        while (slots_[index] != EMPTY) {
            if (slots_[index] == DELETED) {
                if (deleted_index == UINT64_MAX) {
                    deleted_index = index;
                }
                index ++;
                if (index >= capacity_) {
                    index = 0;
                }
                continue;
            }
            if (entries_[slots_[index]].first == key) {
                entries_[slots_[index]].second = value;
                return;
            }
            index ++;
            if (index >= capacity_) {
                index = 0;
            }
        }

        const std::uint64_t limit = capacity_ - capacity_ / 4;
        if (size_ >= limit) {
            expand();
            index = h & (capacity_ - 1);
            deleted_index = UINT64_MAX;
            while (slots_[index] != EMPTY) {
                index ++;
                if (index >= capacity_) {
                    index = 0;
                }
            }
        }

        index = deleted_index == UINT64_MAX ? index : deleted_index;
        entries_.emplace_back(key, value);
        slots_[index] = entries_.size() - 1;
        size_ ++;
    }

    auto& operator[](const K& key) {
        const std::uint64_t h = Hash{}(key);
        std::uint64_t index = h & (capacity_ - 1);

        std::uint64_t deleted_index = UINT64_MAX;
        while (slots_[index] != EMPTY) {
            if (slots_[index] == DELETED) {
                if (deleted_index == UINT64_MAX) {
                    deleted_index = index;
                }
                index ++;
                if (index >= capacity_) {
                    index = 0;
                }
                continue;
            }
            if (entries_[slots_[index]].first == key) {
                return entries_[slots_[index]].second;
            }
            index ++;
            if (index >= capacity_) {
                index = 0;
            }
        }

        const std::uint64_t limit = capacity_ - capacity_ / 4;
        if (size_ >= limit) {
            expand();
            index = h & (capacity_ - 1);
            deleted_index = UINT64_MAX;
            while (slots_[index] != EMPTY) {
                index ++;
                if (index >= capacity_) {
                    index = 0;
                }
            }
        }

        index = deleted_index == UINT64_MAX ? index : deleted_index;
        entries_.emplace_back(key, V());
        slots_[index] = entries_.size() - 1;
        size_ ++;
        return entries_.back().second;
    }

    auto& at(const K& key) {
        const std::uint64_t h = Hash{}(key);
        std::uint64_t index = h & (capacity_ - 1);

        while (slots_[index] != EMPTY) {
            if (slots_[index] == DELETED) {
                index ++;
                if (index >= capacity_) {
                    index = 0;
                }
                continue;
            }
            if (entries_[slots_[index]].first == key) {
                return entries_[slots_[index]].second;
            }
            index ++;
            if (index >= capacity_) {
                index = 0;
            }
        }
        throw std::out_of_range("dense_map::at");
    }

    const auto& at(const K& key) const {
        const std::uint64_t h = Hash{}(key);
        std::uint64_t index = h & (capacity_ - 1);

        while (slots_[index] != EMPTY) {
            if (slots_[index] == DELETED) {
                index ++;
                if (index >= capacity_) {
                    index = 0;
                }
                continue;
            }
            if (entries_[slots_[index]].first == key) {
                return entries_[slots_[index]].second;
            }
            index ++;
            if (index >= capacity_) {
                index = 0;
            }
        }
        throw std::out_of_range("dense_map::at const");
    }

    void erase(const K& key) {
        const std::uint64_t h = Hash{}(key);
        std::uint64_t index = h & (capacity_ - 1);

        while (slots_[index] != EMPTY) {
            if (slots_[index] == DELETED) {
                index ++;
                if (index >= capacity_) {
                    index = 0;
                }
                continue;
            }
            if (entries_[slots_[index]].first == key) {
                break;
            }
            index ++;
            if (index >= capacity_) {
                index = 0;
            }
        }

        if (slots_[index] == EMPTY || slots_[index] == DELETED) {
            return;
        }

        std::int32_t entry = slots_[index];
        std::int32_t back_entry = entries_.size() - 1;
        slots_[index] = DELETED;
        size_ --;
        if (entry != back_entry) {
            entries_[entry] = std::move(entries_.back());

            const std::uint64_t h2 = Hash{}(entries_[entry].first);
            std::uint64_t index2 = h2 & (capacity_ - 1);

            while (slots_[index2] != EMPTY) {
                if (slots_[index2] == DELETED) {
                    index2 ++;
                    if (index2 >= capacity_) {
                        index2 = 0;
                    }
                    continue;
                }
                if (slots_[index2] == back_entry) {
                    break;
                }
                index2 ++;
                if (index2 >= capacity_) {
                    index2 = 0;
                }
            }

            slots_[index2] = entry;
        }
        entries_.pop_back();
    }

    iterator begin() { return iterator(entries_.data()); }
    iterator end() { return iterator(entries_.data() + entries_.size()); }
    const_iterator begin() const { return const_iterator(entries_.data()); }
    const_iterator end() const { return const_iterator(entries_.data() + entries_.size()); }
    const_iterator cbegin() const { return const_iterator(entries_.data()); }
    const_iterator cend() const { return const_iterator(entries_.data() + entries_.size()); }
};

}