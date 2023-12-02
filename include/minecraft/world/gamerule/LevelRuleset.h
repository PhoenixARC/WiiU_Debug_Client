#pragma once

#include <code/code.h>

#include "../../util/BlockPos.h"
#include "../../client/resource/shared_ptr.h"
#include "../../client/resource/vector.h"

namespace mc {
    class LevelRuleset {
	public:

        int LevelRuleset() 
		{
            return code::Func<int, 0x02D035E0, int>()(100);
        }
		
		bool getNamedAreas(int i, mc::AABB* _aabb)
		{
            return code::Func<bool, 0x02D09124,Gamerules*, int, mc::AABB*>()(this, i, _aabb);
		}
		
		int getInt(int i)
		{
            return code::Func<int, 0x02403B9C,Gamerules*, int>()(this, i);
		}

		uint32_t field_0x0;
        uint32_t field_0x4;
        uint32_t field_0x8;
        uint32_t field_0xC;
        uint32_t ActionType;
        uint32_t field_0x14;
        uint32_t field_0x18;
        uint32_t field_0x1C;
        uint32_t field_0x20;
        uint32_t field_0x24;
        uint32_t field_0x28;
        uint32_t field_0x2C;
        uint32_t field_0x30;
        uint32_t field_0x34;
        uint32_t field_0x38;
        uint32_t field_0x3C;
        uint32_t field_0x40;
        uint32_t field_0x44;
        uint32_t field_0x48;
        uint32_t field_0x4C;
        uint32_t field_0x50;
        uint32_t field_0x54;
        uint32_t field_0x58;
        uint32_t field_0x5C;
        uint32_t field_0x60;
        uint32_t field_0x64;
        uint32_t field_0x68;
        uint32_t field_0x6C;
        uint32_t field_0x70;
        uint32_t field_0x74;
        uint32_t field_0x78;
        uint32_t field_0x7C;
        uint32_t field_0x80;
        uint32_t field_0x84;
        uint32_t field_0x88;
		uint32_t field_0x8C;
		uint32_t field_0x90;
		uint32_t field_0x94;
		uint32_t field_0x98;
		uint32_t field_0x9C;
        uint32_t field_0xA0;
        uint32_t field_0xA4;
        uint32_t field_0xA8;
        uint32_t field_0xAC;
        uint32_t field_0xB0;
        uint32_t field_0xB4;
        uint32_t field_0xB8;
        uint32_t field_0xBC;
        uint32_t field_0xC0;
        uint32_t field_0xC4;
        uint32_t field_0xC8;
        uint32_t field_0xCC;
        uint32_t field_0xD0;
        uint32_t field_0xD4;
        uint32_t field_0xD8;
        uint32_t field_0xDC;
        uint32_t field_0xE0;
        uint32_t field_0xE4;
        uint32_t field_0xE8;
        uint32_t field_0xEC;
        uint32_t GenerationOptions;
        uint32_t RuleType;
        uint32_t StringTable;
        uint32_t field_0xFC;
        uint32_t field_0x100;
	};
}