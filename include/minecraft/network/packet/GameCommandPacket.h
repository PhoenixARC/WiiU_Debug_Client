#pragma once

#include <code/code.h>

#include "Packet.h"

namespace mc {
    class GameCommandPacket : public Packet {
    public:
        GameCommandPacket(int a) {
            code::Func<void, 0x027C456C, GameCommandPacket*, int>()(this, a);
        }

        uint32_t field_0x10;
        uint32_t field_0x14;
        uint32_t field_0x18;
        uint32_t EstimatedSize;
        uint32_t field_0x20;
        uint32_t field_0x24;
        uint32_t field_0x28;
        uint32_t field_0x2C;
        uint32_t field_0x30;
        uint32_t field_0x34;
        uint32_t field_0x38;
        uint32_t field_0x3C;
    };
}