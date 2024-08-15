#pragma once
#include <string>
#include <cstdint>

namespace pie {
    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using i32 = std::int32_t;
    enum ModelObjectType {
        Invalid,
        WaveFront,
        Kaydara
    };

    struct ModelInfo {
        std::string format;
    };

    class ModelLoadable {
    };
}
