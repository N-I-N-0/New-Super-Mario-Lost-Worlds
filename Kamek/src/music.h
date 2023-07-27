#ifndef MUSIC_H
#define MUSIC_H 
u8 hijackMusicWithSongName(const char *songName, int themeID, bool hasFast, int channelCount, int trackCount, int *wantRealStreamID);
int hijackSFX(int SFXNum);

static const char* SongNameList [] = {
	"BooWoods_1a", // 100
	"BooWoods_1b",
	"BooWoods_2a",
	"BooWoods_2b",
	"BooWoods_2c",
	"BooWoods_Map", // 105
	"DaisyDunes_1",
	"DaisyDunes_2",
	"DaisyDunes_Map",
	"Observatory_1",
	"Observatory_2", // 110
	"Observatory_Map",
	"AirshipFleet_1",
	"AirshipFleet_2",
	"AirshipFleet_Map",
	"ParatroopaClouds_1", // 115
	"ParatroopaClouds_2",
	"ParatroopaClouds_Map",
	"DKJungle_1",
	"DKJungle_2",
	"DKJungle_Map", // 120
	"CheepCheepLake_1",
	"CheepCheepLake_2",
	"CheepCheepLake_Map",
	"ShyGuyVillage_1",
	"ShyGuyVillage_2", // 125
	"ShyGuyVillage_Map",
	"DaisyDunes_1",
	"DaisyDunes_2",
	"DaisyDunes_Map",
	"Factory_1", // 130
	"Factory_2",
	"Factory_Map",
	"Library_1",
	"Library_2",
	"Library_Map", // 135
	"PeachsCastle_1",
	"PeachsCastle_2",
	"PeachsCastle_Map",
	"BossIncomming_1",
	//...
	NULL
};

static const char* SFXNameList [] = {
	"original",					// 1999, DON'T USE THIS ONE
	"meragon_fire_start",		// 2000
	"meragon_fire",				// 2001
	"meragon_wing",				// 2002
	"tenten_step_l",			// 2003
	"tenten_step_r",			// 2004
	"GoldFlower_hit",			// 2005
	"GoldFlower_out",			// 2006
	"GoldFlower_get",			// 2007
	"GoldFlower_throw",			// 2008
	"FlipPanel",				// 2009
	"barrelFail",				// 2010
	"barrelFly",				// 2011
	"barrelIn",					// 2012
	"barrelStartUp",			// 2013
	"frog_get",			// 2014
	NULL
};

#endif /* MUSIC_H */
