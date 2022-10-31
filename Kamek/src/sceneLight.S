.text
#include <asm_setup.S>

.align 4
.extern aLightMainscene, aLightChikascen, aLightWaterscen, aLightFirescene, aLightSabakusce, aLightKaigansce, aLightDaishizen, aLightYukidaysc, aLightSorascene, aLightIwabascen, aLightToridesce
.extern aLightShiroscen, aLightObakescen, aLightKurayamic, aLightObakeouts, aLightShinkaisc, aLightSabakuchi, aLightKorichika, aLightFire2scen, aLightKoopascen, aLightFire3scen, aLightKumorisor, aLightToride2sc

.extern aLightMainsce_0, aLightChikasc_0, aLightWatersc_0, aLightFiresce_0, aLightSabakus_0, aLightKaigans_0, aLightDaishiz_0, aLightYukiday_0, aLightSorasce_0, aLightIwabasc_0, aLightTorides_0
.extern aLightShirosc_0, aLightObakesc_0, aLightKurayam_0, aLightObakeou_0, aLightShinkai_0, aLightSabakuc_0, aLightKorichi_0, aLightFire2sc_0, aLightKoopasc_0, aLightFire3sc_0, aLightKumoris_0, aLightToride2_0

.extern aFogMainscene_b, aFogChikascene_, aFogWaterscene_, aFogFirescene_b, aFogSabakuscene, aFogKaiganscene, aFogDaishizensc, aFogYukidayscen, aFogSorascene_b, aFogIwabascene_, aFogToridescene
.extern aFogShiroscene_, aFogObakescene_, aFogKurayamichi, aFogObakeoutsce, aFogShinkaiscen, aFogSabakuchika, aFogKorichikasc, aFogFire2scene_, aFogKoopascene_, aFogFire3scene_, aFogKumorisoras, aFogToride2scen

.global useNewLightList
useNewLightList:
	lis r3, newSceneLightList@h
	ori r3, r3, newSceneLightList@l
	blr

.global useNewLightMapList
useNewLightMapList:
	lis r3, newSceneLightMapList@h
	ori r3, r3, newSceneLightMapList@l
	blr

.global useNewFogList
useNewFogList:
	lis r3, newSceneFogList@h
	ori r3, r3, newSceneFogList@l
	blr



.global newUseNewLightList
newUseNewLightList:
	lis r5, newSceneLightList@h
	ori r5, r5, newSceneLightList@l
	lwzx r5, r5, r28
	blr

.global newUseNewLightMapList
newUseNewLightMapList:
	lis r5, newSceneLightMapList@h
	ori r5, r5, newSceneLightMapList@l
	lwzx r5, r5, r28
	blr

.global newUseNewFogList
newUseNewFogList:
	lis r5, newSceneFogList@h
	ori r5, r5, newSceneFogList@l
	lwzx r5, r5, r28
	blr


.data
.global newSceneLightList
newSceneLightList:
	.long aLightMainscene
	.long aLightChikascen
	.long aLightWaterscen
	.long aLightFirescene
	.long aLightSabakusce
	.long aLightKaigansce
	.long aLightDaishizen
	.long aLightYukidaysc
	.long aLightSorascene
	.long aLightIwabascen
	.long aLightToridesce
	.long aLightShiroscen
	.long aLightObakescen
	.long aLightKurayamic
	.long aLightObakeouts
	.long aLightShinkaisc
	.long aLightSabakuchi
	.long aLightKorichika
	.long aLightFire2scen
	.long aLightKoopascen
	.long aLightFire3scen
	.long aLightKumorisor
	.long aLightToride2sc
	.long aLightSilhouette
	.long aLightShyGuyDaishizenScene

.global newSceneLightMapList
newSceneLightMapList:
	.long aLightMainsce_0
	.long aLightChikasc_0
	.long aLightWatersc_0
	.long aLightFiresce_0
	.long aLightSabakus_0
	.long aLightKaigans_0
	.long aLightDaishiz_0
	.long aLightYukiday_0
	.long aLightSorasce_0
	.long aLightIwabasc_0
	.long aLightTorides_0
	.long aLightShirosc_0
	.long aLightObakesc_0
	.long aLightKurayam_0
	.long aLightObakeou_0
	.long aLightShinkai_0
	.long aLightSabakuc_0
	.long aLightKorichi_0
	.long aLightFire2sc_0
	.long aLightKoopasc_0
	.long aLightFire3sc_0
	.long aLightKumoris_0
	.long aLightToride2_0
	.long aLightMapSilhouette
	.long aLightDaishiz_0

.global newSceneFogList
newSceneFogList:
	.long aFogMainscene_b
	.long aFogChikascene_
	.long aFogWaterscene_
	.long aFogFirescene_b
	.long aFogSabakuscene
	.long aFogKaiganscene
	.long aFogDaishizensc
	.long aFogYukidayscen
	.long aFogSorascene_b
	.long aFogIwabascene_
	.long aFogToridescene
	.long aFogShiroscene_
	.long aFogObakescene_
	.long aFogKurayamichi
	.long aFogObakeoutsce
	.long aFogShinkaiscen
	.long aFogSabakuchika
	.long aFogKorichikasc
	.long aFogFire2scene_
	.long aFogKoopascene_
	.long aFogFire3scene_
	.long aFogKumorisoras
	.long aFogToride2scen
	.long aFogSilhouette
	.long aFogDaishizensc


.global aLightSilhouette
aLightSilhouette:
	.string	"light/Silhouette.blight"
	.byte	0
	.byte	0

.global aLightMapSilhouette
aLightMapSilhouette:
	.string	"light/Silhouette.blmap"
	.byte	0
	.byte	0

.global aFogSilhouette
aFogSilhouette:
	.string	"fog/Silhouette.bfog"
	.byte	0
	.byte	0

.global aLightShyGuyDaishizenScene
aLightShyGuyDaishizenScene:
	.string	"light/ShyGuyDaishizenScene.blight"
	.byte	0
	.byte	0
