#include <IOS.h>

bool isUsingDolphinEmulator;
char dolphinVersion[99];
void getDolphinVersion() {
    int fd = IOS_Open("/dev/dolphin", 0);
    if (fd >= 0) {
        isUsingDolphinEmulator = true;

        ioctlv vec;
        vec.data = new char[99];
        vec.len = 99;

        IOS_ioctlv(fd, 2, 0, 1, (void *)&vec);

        sprintf(dolphinVersion, "DOLPHIN %s", vec.data);
    }
}



void setDiscordPresence(int partySize, int worldNumber, bool isTitleScreen) {
    int fd = IOS_Open("/dev/dolphin", 0);
    if (fd < 0)
		return;

    ioctlv clientVector[1] = { 0 };
    char clientID[] = "1026223612161700002";
    clientVector[0].data = clientID;
    clientVector[0].len = sizeof(clientID);
    int setclient = IOS_ioctlv(fd, 0x7, 1, 0, clientVector);
    OSReport("setclient = %i\n", setclient);
	
    ioctlv statusVector[10] = { 0 };
	char details[] = "nin0.me/NSMLW";
	char largeImageKey[] = "spike";
	char largeImageText[] = "A New Super Mario Bros. Wii Mod";
	
	char smallImageKey[10];
	sprintf(smallImageKey, "%d", worldNumber+1);
	char smallImageText[] = "Current World";
    int partyMax = 4;
    statusVector[0].data = details;
    statusVector[0].len = sizeof(details);
    statusVector[2].data = largeImageKey;
    statusVector[2].len = sizeof(largeImageKey);
    statusVector[3].data = largeImageText;
    statusVector[3].len = sizeof(largeImageText);
	statusVector[8].data = &partySize;
	statusVector[8].len = sizeof(int);
	statusVector[9].data = &partyMax;
	statusVector[9].len = sizeof(int);
	if(!isTitleScreen) {
		char state[] = "Number of players";
		statusVector[1].data = state;
		statusVector[1].len = sizeof(state);
		statusVector[4].data = smallImageKey;
		statusVector[4].len = sizeof(smallImageKey);
		statusVector[5].data = smallImageText;
		statusVector[5].len = sizeof(smallImageText);
	} else {
		char state[] = "Starting the game!";
		char emptyText[] = "";
		statusVector[1].data = state;
		statusVector[1].len = sizeof(state);
		statusVector[4].data = emptyText;
		statusVector[4].len = sizeof(emptyText);
		statusVector[5].data = emptyText;
		statusVector[5].len = sizeof(emptyText);
	}
	u64 number0 = 0;
	statusVector[6].data = &number0;
	statusVector[7].data = &number0;
	
	int setpresence = IOS_ioctlv(fd, 0x8, 10, 0, (void *)statusVector);
    OSReport("setpresence = %i\n", setpresence);

    return;
}