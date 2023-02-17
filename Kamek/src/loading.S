.text
.align 4
.set sp, 1

.extern ContinueFromLoadingCRSIN
.global aI_kinoko_3, aI_fireflower_2, aI_iceflower_0, aI_star_0, aI_propeller_0, aI_penguin, aI_hammer_0, aI_gold_0, aI_spike_0, aI_boomerang_0, aI_frog_0, aI_cloud_0, aI_wand_0

#include <asm_setup.S>

.global LoadThisShit
LoadThisShit:
	lis r5, LoadListArray@h
	ori r5, r5, LoadListArray@l
	b ContinueFromLoadingCRSIN

LoadListArray: 
	.long aMario_0								# "Mario"
	.long aLuigi_0								# "Luigi"
	.long aKinopio_0							# "Kinopio"
	.long aYoshi_1								# "Yoshi"
	.long aP_rcha_2								# "P_rcha"
	.long aL_rcha_0								# "L_rcha"
	.long aK_rcha_0								# "K_rcha"
	.long aY_rcha_0								# "Y_rcha"
	.long aY_texgreen_0							# "Y_TexGreen"
	.long aY_texred_0							# "Y_TexRed"
	.long aY_texyellow_0						# "Y_TexYellow"
	.long aY_texblue_0							# "Y_TexBlue"
	.long aObj_coin_3							# "obj_coin"
	.long aBalloon_0							# "balloon"
	.long aI_kinoko_3							# "I_kinoko"
	.long aI_fireflower_2						# "I_fireflower"
	.long aI_iceflower_0						# "I_iceflower"
	.long aI_star_0								# "I_star"
	.long aI_propeller_0						# "I_propeller"
	.long aI_penguin							# "I_penguin"
	.long aI_yoshi_egg_0						# "I_yoshi_egg"
	.long aBlock_tsuta							# "block_tsuta"
	.long aTeresa_0								# "teresa"
	.long aJump_step							# "jump_step"
	.long aIce_4								# "ice"
	.long aIce_piece_0							# "ice_piece"
	.long aObj_dokan_6							# "obj_dokan"
	.long aObj_door_3							# "obj_door"
	.long aObj_kusa_0							# "obj_kusa"
	.long aObj_hana								# "obj_hana"
	.long aObj_hana_dai_0						# "obj_hana_daishizen"
	.long aBlock_jump_0							# "block_jump"
	.long aObj_chikuwa__0						# "obj_chikuwa_block"
	.long aLift_rakka_a_0						# "lift_rakka_ashiba"
	.long aMask_3								# "Mask"
	.long aI_hammer_0							# "I_hammer"
	.long aHammerM_0							# "hammerM"
	.long aBros_0								# "bros"
	.long aGabon_rock_0							# "gabon_rock"
	.long aI_gold_0								# "I_gold"
	.long aI_spike_0							# "I_spike"
	.long aSpikeM_0								# "spikeM"
	.long aI_boomerang_0						# "I_boomerang"
	.long aBoomerangM_0							# "boomerangM"
	.long aI_frog_0								# "I_frog"
	.long aFrogM_0								# "frogM"
	.long aI_cloud_0							# "I_cloud"
	.long aCloudM_0								# "cloudM"
	.long aI_wand_0								# "I_wand"
	

aMario_0:
	.string "Mario"                          # DATA XREF: .data.10.d_basesNP.rel:5:off_8098C40C↓o

aLuigi_0:
	.string "Luigi"                          # DATA XREF: .data.10.d_basesNP.rel:5:8098C410↓o

aKinopio_0:
	.string "Kinopio"                      # DATA XREF: .data.10.d_basesNP.rel:5:8098C414↓o

aYoshi_1:
	.string "Yoshi"                          # DATA XREF: .data.10.d_basesNP.rel:5:8098C418↓o

aP_rcha_2:
	.string "P_rcha"                        # DATA XREF: .data.10.d_basesNP.rel:5:8098C41C↓o

aL_rcha_0:
	.string "L_rcha"                        # DATA XREF: .data.10.d_basesNP.rel:5:8098C420↓o

aK_rcha_0:
	.string "K_rcha"                        # DATA XREF: .data.10.d_basesNP.rel:5:8098C424↓o

aY_rcha_0:
	.string "Y_rcha"                        # DATA XREF: .data.10.d_basesNP.rel:5:8098C428↓o

aY_texgreen_0:
	.string "Y_TexGreen"                # DATA XREF: .data.10.d_basesNP.rel:5:8098C42C↓o

aY_texred_0:
	.string "Y_TexRed"                    # DATA XREF: .data.10.d_basesNP.rel:5:8098C430↓o

aY_texyellow_0:
	.string "Y_TexYellow"              # DATA XREF: .data.10.d_basesNP.rel:5:8098C434↓o

aY_texblue_0:
	.string "Y_TexBlue"                  # DATA XREF: .data.10.d_basesNP.rel:5:8098C438↓o

aObj_coin_3:
	.string "obj_coin"                    # DATA XREF: .data.10.d_basesNP.rel:5:8098C43C↓o

aBalloon_0:
	.string "balloon"                      # DATA XREF: .data.10.d_basesNP.rel:5:8098C440↓o

aI_kinoko_3:
	.string "I_kinoko"                    # DATA XREF: .data.10.d_basesNP.rel:5:8098C444↓o

aI_fireflower_2:
	.string "I_fireflower"            # DATA XREF: .data.10.d_basesNP.rel:5:8098C448↓o

aI_iceflower_0:
	.string "I_iceflower"              # DATA XREF: .data.10.d_basesNP.rel:5:8098C44C↓o

aI_star_0:
	.string "I_star"                        # DATA XREF: .data.10.d_basesNP.rel:5:8098C450↓o

aI_propeller_0:
	.string "I_propeller"              # DATA XREF: .data.10.d_basesNP.rel:5:8098C454↓o

aI_penguin:
	.string "I_penguin"                    # DATA XREF: .data.10.d_basesNP.rel:5:8098C458↓o

aI_yoshi_egg_0:
	.string "I_yoshi_egg"              # DATA XREF: .data.10.d_basesNP.rel:5:8098C45C↓o

aBlock_tsuta:
	.string "block_tsuta"                # DATA XREF: .data.10.d_basesNP.rel:5:8098C460↓o

aTeresa_0:
	.string "teresa"                        # DATA XREF: .data.10.d_basesNP.rel:5:8098C464↓o

aJump_step:
	.string "jump_step"                    # DATA XREF: .data.10.d_basesNP.rel:5:8098C468↓o

aIce_4:
	.string "ice"                              # DATA XREF: .data.10.d_basesNP.rel:5:8098C46C↓o

aIce_piece_0:
	.string "ice_piece"                  # DATA XREF: .data.10.d_basesNP.rel:5:8098C470↓o

aObj_dokan_6:
	.string "obj_dokan"                  # DATA XREF: .data.10.d_basesNP.rel:5:8098C474↓o

aObj_door_3:
	.string "obj_door"                    # DATA XREF: .data.10.d_basesNP.rel:5:8098C478↓o

aObj_kusa_0:
	.string "obj_kusa"                    # DATA XREF: .data.10.d_basesNP.rel:5:8098C47C↓o

aObj_hana:
	.string "obj_hana"                      # DATA XREF: .data.10.d_basesNP.rel:5:8098C480↓o

aObj_hana_dai_0:
	.string "obj_hana_daishizen"      # DATA XREF: .data.10.d_basesNP.rel:5:8098C484↓o

aBlock_jump_0:
	.string "block_jump"                # DATA XREF: .data.10.d_basesNP.rel:5:8098C488↓o

aObj_chikuwa__0:
	.string "obj_chikuwa_block"       # DATA XREF: .data.10.d_basesNP.rel:5:8098C48C↓o

aLift_rakka_a_0:
	.string "lift_rakka_ashiba"       # DATA XREF: .data.10.d_basesNP.rel:5:8098C490↓o

aMask_3:
	.string "Mask"                            # DATA XREF: .data.10.d_basesNP.rel:5:8098C494↓o

aI_hammer_0:
	.string "I_hammer"

aHammerM_0:
	.string "hammerM"

aBros_0:
	.string "bros"

aGabon_rock_0:
	.string "gabon_rock"

aPower_cloud_0:
	.string "power_cloud"

aI_gold_0:
	.string "I_gold"

aI_spike_0:
	.string "I_spike"

aSpikeM_0:
	.string "spikeM"

aI_boomerang_0:
	.string "I_boomerang"

aBoomerangM_0:
	.string "boomerangM"

aI_frog_0:
	.string "I_frog"

aFrogM_0:
	.string "frogM"

aI_cloud_0:
	.string "I_cloud"

aCloudM_0:
	.string "cloudM"

aI_wand_0:
	.string "I_wand"