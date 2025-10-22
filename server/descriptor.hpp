#pragma once

#include <memory>
#include "handler_chain.hpp"

namespace llog {

    class DescriptorUsable : public HandlerChainLink {
    public:
        virtual ~DescriptorUsable() = default;
        [[nodiscard]] virtual int fd() const = 0;
    };

    using DescriptorUsablePtr = std::shared_ptr<DescriptorUsable>;

}
