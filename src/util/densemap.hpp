#pragma once

#include <cstdint>
#include <vector>
#include <utility>
#include <stdexcept>

namespace nasal::util {

template <typename K, typename V>
class densemap {
public:
    class iterator {
    private:
        friend class densemap;
        std::uint64_t index_;
        densemap* map_;

        void skip_empty() {
            while (index_ < map_->capacity_ && map_->map_used_[index_] == 0) {
                ++ index_;
            }
        }

    public:
        using value_type = std::pair<K, V>;
        using reference = value_type&;

        iterator(std::uint64_t i, densemap* m) : index_(i), map_(m) {}

        bool operator==(const iterator& other) const {
            return index_ == other.index_ && map_ == other.map_;
        }
        bool operator!=(const iterator& other) const {
            return index_ != other.index_ || map_ != other.map_;
        }
        reference operator*() const {
            return reinterpret_cast<reference>(map_->map_[index_]);
        }
        iterator& operator++() {
            ++ index_;
            skip_empty();
            return *this;
        }
        iterator operator++(int) {
            auto t = *this;
            ++ (*this);
            return t;
        }
    };

    class const_iterator {
    private:
        friend class densemap;
        std::uint64_t index_;
        const densemap* map_;

        void skip_empty() {
            while (index_ < map_->capacity_ && map_->map_used_[index_] == 0) {
                ++ index_;
            }
        }

    public:
        using value_type = std::pair<K, V>;
        using reference = value_type&;

        const_iterator(std::uint64_t i, const densemap* m) : index_(i), map_(m) {}

        bool operator==(const iterator& other) const {
            return index_ == other.index_ && map_ == other.map_;
        }
        bool operator!=(const iterator& other) const {
            return index_ != other.index_ || map_ != other.map_;
        }
        const reference operator*() const {
            return reinterpret_cast<const reference>(map_->map_[index_]);
        }
        iterator& operator++() {
            ++ index_;
            skip_empty();
            return *this;
        }
        iterator operator++(int) {
            auto t = *this;
            ++ (*this);
            return t;
        }
    };

private:
    std::vector<std::pair<K, V>> map_;
    std::vector<std::uint8_t> map_used_;

    std::uint64_t size_;
    std::uint64_t capacity_;

private:
    void expand() {
        capacity_ *= 2;
        
        std::vector<std::pair<K, V>> old_map = std::move(map_);
        std::vector<std::uint8_t> old_map_used = std::move(map_used_);

        map_.resize(capacity_);
        map_used_.resize(capacity_, 0);

        for (std::uint64_t i = 0; i < old_map.size(); i ++) {
            if (old_map_used[i] == 0) {
                continue;
            }
            const std::uint32_t h = std::hash<K>{}(old_map[i].first);
            std::uint32_t index = h & (capacity_ - 1);

            while (map_used_[index] != 0) {
                index ++;
                if (index >= capacity_) {
                    index = 0;
                }
            }

            map_[index] = std::move(old_map[i]);
            map_used_[index] = 1;
        }
    }

public:
    densemap() : size_(0), capacity_(32) {
        map_.resize(capacity_);
        map_used_.resize(capacity_, 0);
    }

    void clear() {
        size_ = 0;
        map_used_.resize(capacity_, 0);
    }

    bool empty() const {
        return size_ == 0;
    }

    const auto& size() const {
        return size_;
    }

    bool contains(const K& key) const {
        const std::uint32_t h = std::hash<K>{}(key);
        std::uint32_t index = h & (capacity_ - 1);

        while (map_used_[index] != 0) {
            if (map_[index].first == key) {
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
        if (size_ >= capacity_ * 0.75) {
            expand();
        }

        const std::uint32_t h = std::hash<K>{}(key);
        std::uint32_t index = h & (capacity_ - 1);

        while (map_used_[index] != 0) {
            index ++;
            if (index >= capacity_) {
                index = 0;
            }
        }

        map_[index] = std::make_pair(key, value);
        map_used_[index] = 1;
        size_ ++;
    }

    auto& operator[](const K& key) {
        const std::uint32_t h = std::hash<K>{}(key);
        std::uint32_t index = h & (capacity_ - 1);

        while (map_used_[index] != 0) {
            if (map_[index].first == key) {
                return map_[index].second;
            }
            index ++;
            if (index >= capacity_) {
                index = 0;
            }
        }

        if (size_ >= capacity_ * 0.75) {
            expand();
            index = h & (capacity_ - 1);
            while (map_used_[index] != 0) {
                index ++;
                if (index >= capacity_) {
                    index = 0;
                }
            }
        }

        map_[index] = std::make_pair(key, V());
        map_used_[index] = 1;
        size_ ++;
        return map_[index].second;
    }

    auto& at(const K& key) {
        const std::uint32_t h = std::hash<K>{}(key);
        std::uint32_t index = h & (capacity_ - 1);

        while (map_used_[index] != 0) {
            if (map_[index].first == key) {
                return map_[index].second;
            }
            index ++;
            if (index >= capacity_) {
                index = 0;
            }
        }
        throw std::out_of_range("dense_map::at");
    }

    iterator begin() { return iterator(0, this); }
    iterator end() { return iterator(capacity_, this); }
    const_iterator begin() const { return const_iterator(0, this); }
    const_iterator end() const { return const_iterator(capacity_, this); }
};

}