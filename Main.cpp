#include <code/code.h>

#include <code/tcpgecko.h>
#include <code/wups.h>

#include <exports/curl_functions.h>
#include <exports/socket_functions.h>
#include <exports/vpad_functions.h>
#include <exports/gx2_functions.h>
#include <exports/kernel.h>
#include <exports/os_functions.h>
#include <exports/memory.h>

#include <xf/DrawHelper.h>
#include <xf/GUI.h>
#include <xf/Debug.h>




xf::InfoScreen* gui = nullptr;
xf::Debug* debug = nullptr;
xf::Vector<mc::AABB> _abab = {};
bool isGuiInit = false;




void init() {
    InitTCPGecko();
    InitWups();

    InitSocketFunctionPointers();
    InitKernelFunctionPointers();
    InitVPadFunctionPointers();
    InitMemoryFunctionPointers();
    InitOSFunctionPointers();
    InitGX2FunctionPointers();
    InitLibCurlFunctionPointers();

    memoryInitialize();
	
}

void AddAABB(mc::AABB ab)
{
	_abab.push_back(ab);
}
void ClearAABB()
{
	_abab.clear();
}

DECL_HOOK(onFrameInGame, void) {
    xf::GUI::DrawHelper::renderSetup();
    gui->onTick();
	debug->displayCheck();
}


DECL_FUNCTION(void, LevelRenderer_renderEntities, void* c, const mc_boost::shared_ptr<mc::Entity>& entity, void* b, float a) {
    real_LevelRenderer_renderEntities(c, entity, b, a); // Must be Called first
    
    mc::LocalPlayer* player = mc::Minecraft::getInstance()->thePlayer;
	
    mc::GlStateManager::disableCull();
    mc::GlStateManager::disableLighting();
    mc::GlStateManager::enableDepthTest();
    mc::GlStateManager::enableBlend();
    mc::GlStateManager::blendFunc(4, 5);
    mc::GlStateManager::disableFog();
	
	int chunkX = mc::toInt(player->position.x / 16);
	int chunkZ = mc::toInt(player->position.z / 16);
	
	
	//drawChunkGrid({absToLocalCoords({globalGrid.x, globalGrid.y, globalGrid.z}), {globalGrid.x+4.0, globalGrid.y+4.0, globalGrid.z+4.0}});
	if(xf::chunkBoarders)
	{
		//debug->DrawChunkBounds(chunkX, chunkZ);
		for(mc::AABB ab : _abab)
		{
			//debug->sendDebugChat(0x20, L"{%d, %d, %d}//{%d, %d, %d}\n", mc::toInt(ab.min.x), mc::toInt(ab.min.y), mc::toInt(ab.min.z), mc::toInt(ab.max.x), mc::toInt(ab.max.y), mc::toInt(ab.max.z));
	
			debug->DrawArea(ab);
		}
		mc::AABB ba = {xf::Position1, xf::Position2};
		ba.shrink(-1);
		mc::CubeRenderer::drawLineBox(ba, 0xFFFFFF, 0xFF);
	}
	

    mc::GlStateManager::disableBlend();
    mc::GlStateManager::enableCull();
	
	
}


DECL_FUNCTION(int, setArtTools) {
	if(xf::ArtTools)
		return 1;
	return 0;
}
DECL_FUNCTION(int, setDevTools) {
	if(xf::DevTools)
		return 1;
	return 0;
}
DECL_FUNCTION(int, setDevSettings) {
	if(xf::DevSettings)
		return 1;
	return 0;
}


DECL_FUNCTION(void, ServerPlayer_Swing, mc::ServerPlayer* player, mc::InteractionHand::EInteractionHand eHand) {
    real_ServerPlayer_Swing(player, eHand);
	if(xf::DevTools)
	{
		debug->OpenDevContainer(player);
	}
}

DECL_FUNCTION(uint32_t, getArea__NamedAreaRuleDefinition, int i) {
    uint32_t ab = real_getArea__NamedAreaRuleDefinition(i);
    
	
	
		
		
		mc::Vec3* aabb_ptr = (mc::Vec3*)ab;
		mc::Vec3 aabb_pt = *aabb_ptr;
		
		mc::Vec3* aabb_ptr2 = (mc::Vec3*)(ab+24);
		mc::Vec3 aabb_pt2 = *aabb_ptr2;
		
		AddAABB({aabb_pt, aabb_pt2});
		//debug->sendDebugChat(0x20, L"{%d, %d, %d}//{%d, %d, %d}\n", mc::toInt(aabb_pt.x), mc::toInt(aabb_pt.y), mc::toInt(aabb_pt.z), mc::toInt(aabb_pt2.x), mc::toInt(aabb_pt2.y), mc::toInt(aabb_pt2.z));
	
	return ab;
}

DECL_FUNCTION(void, renderHitOutline, uint32_t _ent1, uint32_t _ent2, mc::HitResult* hr, uint32_t i, uint32_t f) {
	
	xf::AimBlock = hr->pos;
     real_renderHitOutline(_ent1, _ent2, hr, i, f);
}

DECL_FUNCTION(void, setBlock, uint32_t _ent1, mc::BlockPos* _ent2, uint32_t _ent3, uint32_t _ent4, uint32_t _ent5) {
	
	//debug->sendDebugChat(0x40, L"BPos %d / %d / %d, State %#p, ID %d, Bool %d", _ent2->x, _ent2->y, _ent2->z, _ent3, _ent4, _ent5);
	debug->Log(0x40, L"BPos %d / %d / %d, State %#p, ID %d, Bool %d", _ent2->x, _ent2->y, _ent2->z, _ent3, _ent4, _ent5);
     real_setBlock(_ent1, _ent2, _ent3, _ent4, _ent5);
}

DECL_HOOK(onFrameInMenu, void) {
	ClearAABB();
	xf::Position1 = {0, 0, 0};
	xf::Position2 = {0, 0, 0};
	
	xf::chunkBoarders = false;
	xf::ArtTools = false;
	xf::DevTools = false;
	xf::DevSettings = false;
	xf::colourblind = false;
	xf::hitboxes = false;
}





int c_main(void*) {
    init();
	
    REPLACE(0x031B8298, LevelRenderer_renderEntities); // chunk renderer
    REPLACE(0x02F5C874, setArtTools); // Art tools
    REPLACE(0x02F5C87C, setDevTools); // Dev tools
    REPLACE(0x02F5C86C, setDevSettings); // Dev Settings
	
    REPLACE(0x02CFC97C,  getArea__NamedAreaRuleDefinition); // Dev Settings
	
    REPLACE(0x031C3020,  renderHitOutline); // Dev Settings
	
    HOOK(0x02D9CAD0, onFrameInGame, 0); //Info overlay
    HOOK(0x02D9C8B0, onFrameInMenu, 0); // Clear World Info
	
	
    REPLACE(0x02547678, setBlock);
	
	
    REPLACE(0x032d8b5c, ServerPlayer_Swing);
	
	return 0;
}
// Gets Called once at startup
void _main() {
    mc::C4JThreadImpl* thread = new mc::C4JThreadImpl(c_main, nullptr, "Setup Thread", 0x200);
    thread->Run();
    thread->SetDeleteOnExit(true);
	
    if (!isGuiInit) {
        gui = _new2(xf::InfoScreen());
        debug = _new2(xf::Debug());
	isGuiInit = true;
	}
	
}