#pragma once

namespace llog {

    using MessagePtr = std::shared_ptr<class Message>;

    class Message {
        public:
            virtual ~Message() = default;
        private:
            Message() = default;
    };

}