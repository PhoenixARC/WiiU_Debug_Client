#pragma once

#include <code/code.h>

#include "buffer/BufferBuilder.h"

namespace mc {
    class Tesselator {
    public:
        static Tesselator* getInstance() {
            return code::Func<Tesselator*, 0x3337EF0>()();
        }

        BufferBuilder* getBuilder() {
            return code::Func<BufferBuilder*, 0x3337EE8, Tesselator*>()(this);
        }
		
        void rendersquare(float x, float y, float sX, float sY, int rgb, int a) {
			
                mc::GlStateManager::pushMatrix();
                mc::GlStateManager::lineWidth(5);
				
                mc::BufferBuilder* builder = mc::Tesselator::getInstance()->getBuilder();
                builder->begin(1);
                builder->color(rgb, a);
                builder->vertex(x,      y,      0.0f);
                builder->vertex(x + sX, y,      0.0f);
                builder->vertex(x,      y + sY, 0.0f); 
                builder->vertex(x + sX, y + sY, 0.0f); 
                builder->tex2(4); 
                builder->end();
				
                mc::GlStateManager::popMatrix();
				
            return;
        }
    };
}