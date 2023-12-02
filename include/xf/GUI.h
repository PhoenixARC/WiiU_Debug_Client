#pragma once


#include <minecraft/util/other.h>

#include "DrawHelper.h"

#include <minecraft/client/file/FileOutputStream.h>
#include <minecraft/client/file/FileInputStream.h>

#include <minecraft/client/app/System.h>


#include <xf/Debug.h>


namespace xf {
		
        class InfoScreen {
        public:

            InfoScreen() = default;


            void send() {
                mc::Minecraft* minecraft = mc::Minecraft::getInstance();
                mc::ClientPacketListener* listener = minecraft->getConnection(0);
                mc::LocalPlayer* lPlayer = minecraft->thePlayer;

				drawCoords();
            }

            void onTick() {
                xf::GUI::DrawHelper::renderSetup();
                send();
            }

        protected:
			#define N_DECIMAL_POINTS_PRECISION (2000) // n = 3. Three decimal points.
			int GetDecimal(float f, int mode = 0){
				int integerPart = (int)f;
				int decimalPart = ((int)(f*N_DECIMAL_POINTS_PRECISION)%N_DECIMAL_POINTS_PRECISION);
				if(decimalPart < 0)
					decimalPart = decimalPart * -1;
				if(mode == 1)
					return integerPart;
				else
					return decimalPart;
			}

			void uint32ToBinaryArray(uint32_t number, int binaryArray[32]) 
			{
				for (int i = 31; i >= 0; --i) {
					binaryArray[i] = number & 1;
					number >>= 1;
				}
			}
			uint32_t binaryArrayToUint32(int binaryArray[32]) 
			{
				uint32_t result = 0;

				for (int i = 0; i < 32; ++i) {
					result = (result << 1) | binaryArray[i];
				}

				return result;
			}
			
			

			
			
			template<typename... Args>
			void DrawDebugLine(uint32_t size, const wchar_t* inputString, uint32_t line, mc::Font* font, Args... args)
			{
				wchar_t* Text = _new<wchar_t>(size);
				mc_swprintf(Text, size, inputString, args...);
				int newTextWidth = font->width(Text) + 3;
				
				mc::Tesselator* tess = mc::Tesselator::getInstance();
				
				mc::GlStateManager::disableTexture();
				tess->rendersquare(toFloat((WIDTH) - newTextWidth        - 3),      toFloat(5.0f * (3.0f * line))  - 3, newTextWidth        + 3, toFloat(5.0f * (3.0f * 1)) - 1, 0x050505, 0x50);
				
				xf::GUI::DrawHelper::DisplayText(font, Text,   1.0f, toFloat((WIDTH) - newTextWidth),        toFloat(5.0f * (3.0f * line)));
			}
			
			
			mc::Vec3 absToLocalCoords(mc::Vec3 absolute)
			{
				return {absolute.x - 432.0, absolute.y, absolute.z - 432.0};
			}
			
			
			bool absoluteCoords = false;
			
            void drawCoords() {
                mc::Font* font = mc::Minecraft::getInstance()->defaultFonts;
                mc::LocalPlayer* player = mc::Minecraft::getInstance()->thePlayer;
				mc::Level* lvl = mc::Minecraft::getInstance()->lvl;
				mc::MinecraftServer* server = mc::MinecraftServer::getInstance();
				
				mc::HitResult HR = mc::HitResult(player);
				
				int x = mc::toInt(player->position.x)+432;
				int y = mc::toInt(player->position.y);
				int z = mc::toInt(player->position.z)+432;
				
				int ChunkX = x / 16;
				int ChunkY = y / 16;
				int ChunkZ = z / 16;
				
				int section = y / 16;
				
				int GridX = mc::toInt((mc::toInt(x / 4)*4)-(ChunkX*16));
				int GridY = mc::toInt((mc::toInt(y / 4)*4)-(ChunkY*16));
				int GridZ = mc::toInt((mc::toInt(z / 4)*4)-(ChunkZ*16));
				
				uint32_t AimID = player->lvl->getBlockId(xf::AimBlock.x, xf::AimBlock.y, xf::AimBlock.z);
				uint32_t AimData = player->lvl->getBlockData(xf::AimBlock.x, xf::AimBlock.y, xf::AimBlock.z);
				
				mc::Block* AimItem = mc::Block::byId(AimID);
				mc::BlockItem bItm = mc::BlockItem(AimItem);
	
				mc::Vec3 globalGrid = {(double)GridX, (double)GridY, (double)GridZ};
				mc::Vec3 localblock = {x % 4, y % 4, z % 4};
				
				if(absoluteCoords)
				{
					DrawDebugLine(0x40, L"pos(%d, %d, %d)",                            1, font, x, y, z);
					DrawDebugLine(0x40, L"hitbox(%d, %d, %d // %d, %d, %d)",           2, font, mc::toInt(player->_aabb->min.x+432), mc::toInt(player->_aabb->min.y), mc::toInt(player->_aabb->min.z+432), mc::toInt(player->_aabb->max.x+432), mc::toInt(player->_aabb->max.y), mc::toInt(player->_aabb->max.z+432));
					DrawDebugLine(0x40, L"chunkPos(%d, %d)",                           5, font, ChunkX, ChunkZ);
				
				}
				else
				{
					DrawDebugLine(0x40, L"pos(%d, %d, %d)",                            1, font, mc::toInt(player->position.x), mc::toInt(player->position.y), mc::toInt(player->position.z));
					DrawDebugLine(0x40, L"hitbox(%d, %d, %d // %d, %d, %d)",           2, font, mc::toInt(player->_aabb->min.x), mc::toInt(player->_aabb->min.y), mc::toInt(player->_aabb->min.z), mc::toInt(player->_aabb->max.x), mc::toInt(player->_aabb->max.y), mc::toInt(player->_aabb->max.z));
					DrawDebugLine(0x40, L"chunkPos(%d, %d)",                           5, font, mc::toInt(player->position.x / 16), mc::toInt(player->position.z / 16));
				}
				DrawDebugLine(0x40, L"rot(%d, %d)",                                 3, font, GetDecimal(player->pitch, 1), GetDecimal(player->pitch, 0), GetDecimal(player->yaw, 1), GetDecimal(player->yaw, 0));
                DrawDebugLine(0x40, L"vel(%d.%d, %d.%d, %d.%d)",                    4, font, GetDecimal(player->motion.x, 1), GetDecimal(player->motion.x, 0), GetDecimal(player->motion.y, 1), GetDecimal(player->motion.y, 0), GetDecimal(player->motion.z, 1), GetDecimal(player->motion.z, 0));
				DrawDebugLine(0x40, L"mod(%d, %d, %d)",                             6, font, mc::toInt(player->position.x) % 16, mc::toInt(player->position.y) % 16, mc::toInt(player->position.z) % 16);
				DrawDebugLine(0x80, L"S(%d), G(%d, %d, %d), B(%d, %d, %d)",         7, font, section, mc::toInt(globalGrid.x/4), mc::toInt(globalGrid.y/4), mc::toInt(globalGrid.z/4), mc::toInt(localblock.x), mc::toInt(localblock.y), mc::toInt(localblock.z));
				DrawDebugLine(0x80, L"FOV(%d)",    							  	    8, font, mc::toInt(mc::Minecraft::getInstance()->options->FOV));
				DrawDebugLine(0x80, L"Looking At:(%d, %d, %d) ID<%d:%d>", 			9, font, xf::AimBlock.x, xf::AimBlock.y, xf::AimBlock.z, AimID, AimData);
				DrawDebugLine(0x80, L"Looking At: %ls", 						   10, font, bItm.unk_str.c_str());
				//DrawDebugLine(0x80, L"colourblind(%#p)",    				       9, font, mc::toInt(mc::Minecraft::getInstance()->options->viewDistance));
				//DrawDebugLine(0x40, L"X1(%d, %d, %d, %d, %d, %d, %d, %d // %#p)",  8, font,  binaryArray[0],  binaryArray[1],  binaryArray[2],  binaryArray[3],  binaryArray[4],  binaryArray[5],  binaryArray[6],  binaryArray[7], (t >> 24) & 0xFF);
				//DrawDebugLine(0x40, L"X2(%d, %d, %d, %d, %d, %d, %d, %d // %#p)",  9, font,  binaryArray[8],  binaryArray[9], binaryArray[10], binaryArray[11], binaryArray[12], binaryArray[13], binaryArray[14], binaryArray[15], (t >> 16) & 0xFF);
				//DrawDebugLine(0x40, L"X4(%d, %d, %d, %d, %d, %d, %d, %d // %#p)", 10, font, binaryArray[24], binaryArray[25], binaryArray[26], binaryArray[27], binaryArray[28], binaryArray[29], binaryArray[30], binaryArray[31], (t)       & 0xFF);
				//DrawDebugLine(0x40, L"X3(%d, %d, %d, %d, %d, %d, %d, %d // %#p)", 11, font, binaryArray[16], binaryArray[17], binaryArray[18], binaryArray[19], binaryArray[20], binaryArray[21], binaryArray[22], binaryArray[23], (t >> 8)  & 0xFF);
				
				
            }
			


    
};}