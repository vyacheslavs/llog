#pragma once

#include <cstdint>
#include <vector>
#include <list>
#include "log.hpp"

namespace llog {

    class DQueue {
        public:

            using vector_type = std::vector<uint8_t>;

            DQueue(const DQueue&) = delete;
            DQueue& operator=(const DQueue&) = delete;
            DQueue(DQueue&&) = delete;
            DQueue& operator=(DQueue&&) = delete;

            std::pair<uint8_t*, size_t> allocate(size_t size);
            void commit(size_t size);
            uint8_t* pop_data(size_t size);

            [[nodiscard]] bool available(size_t size) const;

            DQueue() = default;

            void reset();

        private:

            int m_first_vector_offset{0};
            std::list<vector_type> m_vector_pool;
            std::list<vector_type> m_vectors;
            size_t m_size{0};
    };

}
