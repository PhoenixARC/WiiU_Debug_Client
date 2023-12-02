#pragma once

#include <code/code.h>

#include "buffer/BufferBuilder.h"
#include "GlStateManager.h"

namespace mc {
    namespace CubeRenderer {
	
void drawLine(mc::BufferBuilder* builder, mc::Vec3 pos1, mc::Vec3 pos2) {
    builder->vertex(pos1.x, pos1.y, pos1.z, true);
    builder->vertex(pos2.x, pos2.y, pos2.z, true);
}

void drawLineBox(mc::AABB box, uint32_t rgb = 0xFFFFFF, uint32_t a = 0xFF) {
    mc::BufferBuilder* builder = mc::Tesselator::getInstance()->getBuilder();
    mc::GlStateManager::disableTexture();
    mc::GlStateManager::lineWidth(10);
    mc::GlStateManager::pushMatrix();
    builder->begin(MC_GL_LINES);
    builder->color(rgb, a);
    // Bottom
    drawLine(builder, {box.min.x, box.min.y, box.min.z}, {box.max.x, box.min.y, box.min.z});
    drawLine(builder, {box.max.x, box.min.y, box.min.z}, {box.max.x, box.min.y, box.max.z});    
    drawLine(builder, {box.max.x, box.min.y, box.max.z}, {box.min.x, box.min.y, box.max.z});
    drawLine(builder, {box.min.x, box.min.y, box.max.z}, {box.min.x, box.min.y, box.min.z});

    // Top
    drawLine(builder, {box.min.x, box.max.y, box.min.z}, {box.max.x, box.max.y, box.min.z});
    drawLine(builder, {box.max.x, box.max.y, box.min.z}, {box.max.x, box.max.y, box.max.z});    
    drawLine(builder, {box.max.x, box.max.y, box.max.z}, {box.min.x, box.max.y, box.max.z});
    drawLine(builder, {box.min.x, box.max.y, box.max.z}, {box.min.x, box.max.y, box.min.z});

    // Lines from Buttom to Top
    drawLine(builder, {box.min.x, box.min.y, box.min.z}, {box.min.x, box.max.y, box.min.z});
    drawLine(builder, {box.max.x, box.min.y, box.min.z}, {box.max.x, box.max.y, box.min.z});
    drawLine(builder, {box.min.x, box.min.y, box.max.z}, {box.min.x, box.max.y, box.max.z});
    drawLine(builder, {box.max.x, box.min.y, box.max.z}, {box.max.x, box.max.y, box.max.z});
        
    builder->end();
    mc::GlStateManager::popMatrix();
}

void drawChunkCorners(mc::BufferBuilder* builder, int ChunkX, int ChunkZ)
{
	//create real bounds
	mc::AABB box = mc::AABB((double)ChunkX*16, 0.0, (double)ChunkZ*16, (double)ChunkX*16+16, 256.0, (double)ChunkZ*16+16);
	box.shrink(0.5);
	
	//sendDebugChat(0x40, L"{%d, %d, %d}, {%d, %d, %d}", mc::toInt(box.min.x), mc::toInt(box.min.y), mc::toInt(box.min.z), mc::toInt(box.max.x), mc::toInt(box.max.y), mc::toInt(box.max.z));
    drawLine(builder, {box.min.x, box.min.y, box.min.z}, {box.min.x, box.max.y, box.min.z});
    drawLine(builder, {box.max.x, box.min.y, box.min.z}, {box.max.x, box.max.y, box.min.z});
    drawLine(builder, {box.min.x, box.min.y, box.max.z}, {box.min.x, box.max.y, box.max.z});
    drawLine(builder, {box.max.x, box.min.y, box.max.z}, {box.max.x, box.max.y, box.max.z});
}

void drawChunkSections(mc::BufferBuilder* builder, int ChunkX, int ChunkZ)
{
	//create real bounds
	mc::AABB box = mc::AABB((double)ChunkX*16, 0.0, (double)ChunkZ*16, (double)ChunkX*16+16, 0.0, (double)ChunkZ*16+16);
	box.shrink(0.5);
	
	for(int i = 0; i < 128; i += 1) // horizontal side lines
	{
		builder->color(0xccc100, 0xFF);
		if(i % 2 == 0)
			builder->color(0xc100cc, 0xFF);
		if(i % 8 == 0)
			builder->color(0x0000cd, 0xFF);
		
		drawLine(builder, { box.min.x,     (double)i*2,  box.min.z}, {box.max.x, (double)i*2,  box.min.z});
		drawLine(builder, { box.max.x,     (double)i*2,  box.min.z}, {box.max.x, (double)i*2,  box.max.z});    
		drawLine(builder, { box.max.x,     (double)i*2,  box.max.z}, {box.min.x, (double)i*2,  box.max.z});
		drawLine(builder, { box.min.x,     (double)i*2,  box.max.z}, {box.min.x, (double)i*2,  box.min.z});
	}
}

void drawChunkblocks(mc::BufferBuilder* builder, int ChunkX, int ChunkZ)
{
	//create real bounds
	mc::AABB box = mc::AABB((double)ChunkX*16, 0.0, (double)ChunkZ*16, (double)ChunkX*16+16, 256.0, (double)ChunkZ*16+16);
	box.shrink(0.5);
	 
	for(int i = 0; i < 8; i ++) // vertical side lines
	{
		builder->color(0xccc100, 0xFF);
		if(i % 2 == 0)
			builder->color(0x0000cd, 0xFF);
		
		drawLine(builder, { box.min.x+(i*2), box.min.y,  box.min.z},       {(box.min.x)+(i*2), box.max.y,  box.min.z});
		drawLine(builder, { box.min.x,       box.min.y,  box.min.z+(i*2)}, {(box.min.x),       box.max.y,  box.min.z+(i*2)});
		drawLine(builder, { box.min.x+(i*2), box.min.y,  box.max.z},       {(box.min.x)+(i*2), box.max.y,  box.max.z});
		drawLine(builder, { box.max.x,       box.min.y,  box.min.z+(i*2)}, {(box.min.x)+16,    box.max.y,  box.min.z+(i*2)});
	} 
}

void drawChunkGrid(mc::AABB grid)
{
	xf::GUI::DrawHelper::DisplayBox3D(grid.min.x, grid.min.y, grid.min.z, 4, 4, 4, 0xFF0000, 0xFF / 2, true);
}
    
}}