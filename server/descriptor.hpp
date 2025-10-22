#pragma once

#include <memory>
#include "handler_chain.hpp"

namespace llog {

    class DescriptorUsable : public HandlerChainLink {
        public:
            virtual ~DescriptorUsable() = default;
            [[nodiscard]] virtual int fd() const = 0;
            [[nodiscard]] bool alive() const {
                return m_alive;
            }
            void mark_dead() {
                m_alive = false;
            }
        protected:
            bool m_alive {true};
    };

    using DescriptorUsablePtr = std::shared_ptr<DescriptorUsable>;

}
