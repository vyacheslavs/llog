#include "dqueue.hpp"

#include <cstring>

std::pair<uint8_t *, size_t> llog::DQueue::allocate(size_t size) {

    vector_type v;

    if (!m_vector_pool.empty()) {
        v = std::move(m_vector_pool.front());
        m_vector_pool.pop_front();
        if (size > v.size())
            v.resize(size);
    } else {
        v.resize(size);
    }
    auto ptr = v.data();
    size_t s = v.size();
    m_vectors.push_back(std::move(v));
    return {ptr, s};
}

void llog::DQueue::commit(size_t size) {

    auto back = m_vectors.back();
    back.resize(size);
    m_size += back.size();
}

uint8_t* llog::DQueue::pop_data(size_t size) {
    if (!available(size)) {
        return {};
    }

    vector_type v;

    size_t remaining = size;

    while (remaining > 0 && !m_vectors.empty()) {
        auto& front = m_vectors.front();
        size_t available = front.size() - m_first_vector_offset;
        if (available <= remaining) {

            if (available == remaining && m_first_vector_offset == 0) { // special case when all chunk can be dumped
                vector_type r = std::move(front);
                m_vectors.pop_front();
                m_size -= available;
                auto ptr = r.data();
                m_vector_pool.push_back(std::move(r));
                return ptr;
            }

            v.insert(v.end(), front.begin() + m_first_vector_offset, front.end());
            remaining -= available;
            m_first_vector_offset = 0;
            vector_type r = std::move(front);
            m_vector_pool.push_back(std::move(r));
            m_vectors.pop_front();

            m_size -= available;

        } else {
            v.insert(v.end(), front.begin() + m_first_vector_offset, front.begin() + m_first_vector_offset + remaining);
            m_first_vector_offset += remaining;
            m_size -= remaining;
            remaining = 0;
        }
    }
    auto ptr = v.data();
    m_vector_pool.push_back(std::move(v));
    return ptr;
}

bool llog::DQueue::available(size_t size) const {
    return m_size >= size;
}

void llog::DQueue::reset() {
    m_vectors.clear();
    m_size = 0;
    m_first_vector_offset = 0;
}
