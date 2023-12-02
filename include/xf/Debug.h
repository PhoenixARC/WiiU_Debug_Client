#pragma once


#include <minecraft/util/other.h>
#include <minecraft/util/Vec3.h>

#include "DrawHelper.h"

#include <minecraft/client/file/FileOutputStream.h>
#include <minecraft/client/file/FileInputStream.h>

#include <minecraft/client/app/System.h>
#include <minecraft/rendering/CubeRenderer.h>


namespace xf {
		

bool chunkBoarders = false;
bool ArtTools = false;
bool DevTools = false;
bool DevSettings = false;
bool colourblind = false;
bool hitboxes = false;

mc::BlockPos AimBlock = {0, 0, 0};
mc::Vec3 Position1 = {0, 0, 0};
mc::Vec3 Position2 = {0, 0, 0};

int ColInc = 0;
        class Debug {
        public:
			
mc::Vec3 absToLocalCoords(mc::Vec3 absolute)
{
	return {absolute.x - 432.0, absolute.y, absolute.z - 432.0};
}

template<typename... Args>
void Log(uint32_t size, const wchar_t* inputString, Args... args) {
	
	wchar_t* Text = _new<wchar_t>(size);
	mc_swprintf(Text, size, inputString, args...);
	
	code::Func<void, 0x0382F6B8, const wchar_t*>()(Text);
}

template<typename... Args>
void sendDebugChat(uint32_t size, const wchar_t* inputString, Args... args)
{
	wchar_t* Text = _new<wchar_t>(size);
	mc_swprintf(Text, size, inputString, args...);
	
	
	for (mc_boost::shared_ptr<mc::ServerPlayer>& _player : mc::MinecraftServer::getInstance()->getPlayers()->players) 
	{
		_player->listener->send(new mc::ClientboundChatPacket(Text));
    }
}

void CorrectPositions(int i)
{
	switch(i)
	{
		case 1:
		Position1 = {mc::toInt(Position1.x), mc::toInt(Position1.y), mc::toInt(Position1.z)};
		sendDebugChat(0x40, L"First Position: %d, %d, %d", mc::toInt(Position1.x), mc::toInt(Position1.y), mc::toInt(Position1.z));
		break;
		case 2:
		Position2 = {mc::toInt(Position2.x), mc::toInt(Position2.y), mc::toInt(Position2.z)};
		sendDebugChat(0x40, L"Second Position: %d, %d, %d", mc::toInt(Position2.x), mc::toInt(Position2.y), mc::toInt(Position2.z));
		break;
	}
}

void DrawChunkBounds(int ChunkX, int ChunkZ)
{
    mc::BufferBuilder* builder = mc::Tesselator::getInstance()->getBuilder();
    mc::GlStateManager::disableTexture();
    mc::GlStateManager::lineWidth(20.0f);
    mc::GlStateManager::pushMatrix();
    builder->begin(MC_GL_LINES);
	
	// surrounding chunk corners
	builder->color(0x8B0000, 0xFF);
	
	 mc::CubeRenderer::drawChunkCorners(builder, ChunkX+1, ChunkZ);
	 mc::CubeRenderer::drawChunkCorners(builder, ChunkX+1, ChunkZ+1);
	 mc::CubeRenderer::drawChunkCorners(builder, ChunkX+1, ChunkZ-1);
	 mc::CubeRenderer::drawChunkCorners(builder, ChunkX, ChunkZ+1);
	 mc::CubeRenderer::drawChunkCorners(builder, ChunkX-1, ChunkZ);
	 mc::CubeRenderer::drawChunkCorners(builder, ChunkX-1, ChunkZ-1);
	 mc::CubeRenderer::drawChunkCorners(builder, ChunkX-1, ChunkZ+1);
	 mc::CubeRenderer::drawChunkCorners(builder, ChunkX, ChunkZ-1);
	
	
    builder->color(0x0000cd, 0xFF);
	 // corners
	 
	 mc::CubeRenderer::drawChunkCorners (builder, ChunkX, ChunkZ);
	 mc::CubeRenderer::drawChunkSections(builder, ChunkX, ChunkZ);
	 mc::CubeRenderer::drawChunkblocks  (builder, ChunkX, ChunkZ);
	 
    //drawLineBox({{ChunkX*16, 256, ChunkZ*16}, {(ChunkX*16)+16, 0, (ChunkZ*16)+16}}, 0xfcff49, 0xFF);
	
	
	
	
    builder->end();
    mc::GlStateManager::popMatrix();
}
void DrawArea(mc::AABB _aabb)
{
	uint16_t colour = 0xFF00FF;
	
	code::Random* rnd = _new2(code::Random());
	rnd->SetLehmer32(mc::toInt(_aabb.max.z));
	ColInc = rnd->next(0,6);
	
	switch(ColInc)
	{
		case 0: 
			colour = 0xFF00FF;
			break;
		case 1: 
			colour = 0xFF8888;
			break;
		case 2: 
			colour = 0x008800;
			break;
		case 3: 
			colour = 0xFF0088;
			break;
		case 4: 
			colour = 0xFF0000;
			break;
		case 5: 
			colour = 0x8800FF;
			break;
		case 6: 
			colour = 0x880088;
			break;
	}
	
	//xf::GUI::DrawHelper::DisplayBox3D(_aabb.min.x, _aabb.min.y, _aabb.min.z, _aabb.max.z, _aabb.max.y, _aabb.max.z, colour, 0xFF / 2, true);
	mc::CubeRenderer::drawLineBox(_aabb, colour, 0xFF);
}

void displayCheck() {
    START_BUTTONCHECK((button & VPAD_BUTTON_Y) && (button & VPAD_BUTTON_LEFT) , chunkBoarders = !chunkBoarders, button)
    ADD_BUTTONCHECK((button & VPAD_BUTTON_Y) && (button & VPAD_BUTTON_RIGHT) , ArtTools = !ArtTools)
    ADD_BUTTONCHECK((button & VPAD_BUTTON_L)  && (button & VPAD_BUTTON_R), DevTools = !DevTools; hitboxes = !hitboxes; mc::Minecraft::getInstance()->options->SetrenderDebug(DevTools))
	ADD_BUTTONCHECK((button & VPAD_BUTTON_Y) && (button & VPAD_BUTTON_DOWN), Position1 = mc::Minecraft::getInstance()->thePlayer->position; CorrectPositions(1))
    ADD_BUTTONCHECK((button & VPAD_BUTTON_Y)  && (button & VPAD_BUTTON_UP), Position2 = mc::Minecraft::getInstance()->thePlayer->position; CorrectPositions(2))
    ADD_BUTTONCHECK((button & VPAD_BUTTON_ZL)  && (button & VPAD_BUTTON_ZR), FillArea({Position1, Position2}))
    //ADD_BUTTONCHECK((button & VPAD_BUTTON_Y) && (button & VPAD_BUTTON_DOWN), mc::Minecraft::getInstance()->options->FOV += 1)
    //ADD_BUTTONCHECK((button & VPAD_BUTTON_Y)  && (button & VPAD_BUTTON_UP), mc::Minecraft::getInstance()->options->FOV -= 1)
    END_BUTTONCHECK()
}

void SetBlock(int x, int y, int z, int ID)
{
	mc::BlockPos Bpos = mc::BlockPos(x, y, z);
	
	mc::BlockPos* PosPtr = &Bpos;
	uint32_t State = mc::Block::byId(ID)->defBlockState;
	
	mc::Minecraft::getInstance()->thePlayer->lvl->setBlock(PosPtr, State, 1, true);
}

float min(float a, float b) {
	return (a > b) ? b : a;
}

float max(float a, float b) {
	return (a > b) ? a : b;
}

void FillArea(mc::AABB _aabb)
{
	mc::LocalPlayer* lPlayer = mc::Minecraft::getInstance()->thePlayer;
	
	mc_boost::shared_ptr<mc::ItemInstance> item;
    lPlayer->getCarriedItem(item);
	
	int ItemID = item->item->getId();
	if(mc::Item::isItemABlock(ItemID))
	{
	
		float startX = min(toFloat(_aabb.min.x), toFloat(_aabb.max.x));
		float endX   = max(toFloat(_aabb.min.x), toFloat(_aabb.max.x)) + 1;
		float startY = min(toFloat(_aabb.min.y), toFloat(_aabb.max.y));
		float endY   = max(toFloat(_aabb.min.y), toFloat(_aabb.max.y)) + 1;
		float startZ = min(toFloat(_aabb.min.z), toFloat(_aabb.max.z));
		float endZ   = max(toFloat(_aabb.min.z), toFloat(_aabb.max.z)) + 1;

		int lastBlockGiven = -1;

		for	 (int y = startY; y < endY; y++) {
		    for     (int z = startZ; z < endZ; z++) {
				for (int x = startX; x < endX; x++) {
				SetBlock(x, y, z, ItemID);
			    }
			}
		}
	}
	else
	{
		sendDebugChat(0x40, L"Please select a block...");
	}
}


void OpenDevContainer(mc::ServerPlayer* player)
{
		// Creates a Container with the Name "Menu" and sets its size to 27
		mc::SimpleContainer* cont = new mc::SimpleContainer(0, 0, L"DevUtils", 27);
		mc_boost::shared_ptr<mc::SimpleContainer> shared;
		// I don't know why this is needded, but it is
		mc::SimpleContainer::toShared(shared, cont);
    
		shared->setItem(0, new mc::ItemInstance(mc::Item::byId( 166), 99));
		shared->setItem(1, new mc::ItemInstance(mc::Item::byId( 137), 99));
		shared->setItem(2, new mc::ItemInstance(mc::Item::byId( 211), 99));
		shared->setItem(3, new mc::ItemInstance(mc::Item::byId( 210), 99));
		shared->setItem(4, new mc::ItemInstance(mc::Item::byId( 422), 99));
		shared->setItem(5, new mc::ItemInstance(mc::Item::byId(  51), 99));
		shared->setItem(6, new mc::ItemInstance(mc::Item::byId(2255), 99));
		
		shared->setItem(7, new mc::ItemInstance(mc::Item::byId(  49), 99));
		shared->setItem(8, new mc::ItemInstance(mc::Item::byId( 259), 99));
		shared->setItem(9, new mc::ItemInstance(mc::Item::byId( 120), 99));
		shared->setItem(10, new mc::ItemInstance(mc::Item::byId(381), 99));
		shared->setItem(11, new mc::ItemInstance(mc::Item::byId(368), 99));
		shared->setItem(12, new mc::ItemInstance(mc::Item::byId(326), 99));
    
		// Opens Container for Player (Must be ServerPlayer if you want it to work with other Players)
		player->openContainer(&shared->container);
}
    
};

}