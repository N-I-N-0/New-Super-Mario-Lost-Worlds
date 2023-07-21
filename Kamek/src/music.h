#ifndef MUSIC_H
#define MUSIC_H 
u8 hijackMusicWithSongName(const char *songName, int themeID, bool hasFast, int channelCount, int trackCount, int *wantRealStreamID);
int hijackSFX(int SFXNum);

static const char* SongNameList [] = {
	"SPACE", // 100
	"STAR_HAVEN", // 101
	"TAMASHI", // 102
	"DRIPDROP", // 103
	"SPEEDYCOMET", // 104
	"TRAIN", // 105
	"LAVA", // 106
    "BONUS", // 107
	"VOLCANO", // 108
	"CHAMPION", // 109
	"FINAL", // 110
	"SHIP", // 111
	"FINALA", // 112
	"CAVE", // 113
	"FLIPBOOK", // 114
	"SINGALONG", // 115
	"BOSS", // 116
	"BOSS_ARENA", // 117
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
	NULL	
};

#endif /* MUSIC_H */
