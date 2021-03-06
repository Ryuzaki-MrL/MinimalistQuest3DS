#include <stdio.h>

#include "Sprite.h"
#include "sprites.h" // generated by tex3ds

static const SpriteData sprdata[] = {
	{sprites_spr_dummy_0_idx,1,0,0,{0,0,0,0}},
	{sprites_spr_player_0_idx,4,8,8,{1,14,1,14}},
	{sprites_spr_key_0_idx,1,0,0,{2,13,2,13}},
	{sprites_spr_chest_0_idx,2,0,4,{0,15,4,14}},
	{sprites_spr_brickwall_0_idx,1,0,0,{0,15,0,15}},
	{sprites_spr_wall_0_idx,1,0,0,{0,15,0,15}},
	{sprites_spr_puzzleball_0_idx,1,8,8,{1,14,1,14}},
	{sprites_spr_switch_0_idx,2,0,0,{0,15,0,15}},
	{sprites_spr_lever_0_idx,6,0,0,{1,14,3,12}},
	{sprites_spr_conveyor_0_idx,4,0,0,{0,15,0,15}},
	{sprites_spr_gate_0_idx,1,0,0,{0,15,0,31}},
	{sprites_spr_gate2_0_idx,1,0,0,{0,31,0,31}},
	{sprites_spr_weapons_0_idx,4,0,0,{1,14,1,14}},
	{sprites_spr_items_0_idx,3,0,0,{2,13,1,14}},
	{sprites_spr_hpup_0_idx,1,0,0,{1,15,1,14}},
	{sprites_spr_map_0_idx,1,0,0,{2,13,2,13}},
	{sprites_spr_checkpoint_0_idx,2,0,0,{2,13,2,13}},
	{sprites_spr_mush_0_idx,1,8,8,{2,13,2,14}},
	{sprites_spr_skeleton_0_idx,1,8,8,{2,12,1,14}},
	{sprites_spr_slime_0_idx,1,8,8,{3,12,4,14}},
	{sprites_spr_spider_0_idx,1,8,8,{4,11,2,13}},
	{sprites_spr_spirit_0_idx,1,8,8,{2,13,2,13}},
	{sprites_spr_chestcrab_0_idx,1,8,4,{2,13,6,17}},
	{sprites_spr_eventmark_0_idx,1,0,0,{0,15,0,15}},
	{sprites_spr_swordhit_0_idx,1,8,8,{0,15,5,10}},
	{sprites_spr_pickaxehit_0_idx,1,8,8,{1,14,4,12}},
	{sprites_spr_boomeranghit_0_idx,1,8,8,{4,11,4,11}}
};

const SpriteData* spriteGet(Sprite id) {
	return &sprdata[id];
}

uint8_t spriteGetTexture(Sprite id, uint8_t frame) {
	return sprdata[id].texture + (frame % sprdata[id].imgcount);
}
