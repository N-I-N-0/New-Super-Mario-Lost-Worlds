const char* EmptyFileList [] = {
	NULL,
};

const char* AirshipsFileList [] = {
	"cobKoopaShips",
	NULL,
};

const char* PeachsCastleFileList [] = {
	"cobKoopaShips",
	"cobSign",
	NULL,
};


extern "C" int getLevelInfoWorldNumber(int world, int subWorld);
extern "C" u8 CurrentWorldNumForWorldMap;
extern "C" u8 CurrentWorldNumForWorldMapSub;



char** getFileListForWorld() {
	u8 worldnum = getLevelInfoWorldNumber(CurrentWorldNumForWorldMap, CurrentWorldNumForWorldMapSub);
	
	switch(worldnum) {
		case 8: //Peach's Castle
			return (char**)&PeachsCastleFileList;

		case 16: //Airship Fleet ???
		default:
			return (char**)&AirshipsFileList;
	}
}