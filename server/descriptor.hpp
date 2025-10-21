#pragma once

namespace llog {

    class DescriptorUsable {
    public:
        virtual ~DescriptorUsable() = default;
        [[nodiscard]] virtual int fd() const = 0;
    };

    using DescriptorUsablePtr = std::shared_ptr<DescriptorUsable>;

}
