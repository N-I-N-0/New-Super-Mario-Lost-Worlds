#include <profile.h>



/* SPRITES */

SpriteData sprites[SpriteId::Num] = { 0 };

class SpritesSetter
{
public:
    SpritesSetter();
};

SpritesSetter::SpritesSetter()
{
    for (u32 i = 0; i < 483; i++)
        sprites[i] = originalSprites[i];
}

static SpritesSetter doSetSprites;

/* PROFILES */

Profile* profiles[ProfileId::Num] = { 0 };
extern "C" Profile** ObjectProfileList;

class ProfileSetter
{
public:
    ProfileSetter();
};

ProfileSetter::ProfileSetter()
{
    ObjectProfileList = &profiles[0];

    for (u32 i = 0; i < ORIGINAL_PROFILES; i++) {
        profiles[i] = originalProfiles[i];
    }
}

static ProfileSetter doSetProfiles;

void SetObjectProfileList()
{
    ObjectProfileList = &profiles[0];
}

/* SPRITE FILES */

extern "C" const char** spriteFiles[483];
const char** customSpriteFiles[SpriteId::Num - 483] = { 0 };

/* PROFILE NAMES*/

extern "C" const char* profileNames[ORIGINAL_PROFILES];
const char* customProfileNames[ProfileId::Num - ORIGINAL_PROFILES] = { 0 };

const char* GetProfileName(u32 profileId)
{
    if (profileId < ORIGINAL_PROFILES) {
        return profileNames[profileId];
    }

    return customProfileNames[profileId - ORIGINAL_PROFILES];
}

/* CUSTOM PROFILE CTOR */

Profile::Profile(dActor_c* (*buildFunc)(), u32 id, const SpriteData* spriteData, u16 executeOrderProfileId, u16 drawOrderProfileId, const char* name, const char** files, u32 flags) {
    this->buildFunc = buildFunc;
    this->executeOrderProfileId = executeOrderProfileId;
    this->drawOrderProfileId = drawOrderProfileId;
	this->flags = flags;

	u32 profileId;
	if (spriteData) {
		sprites[id] = *spriteData;
		if (id < 483) {
			spriteFiles[id] = files;
		} else {
			customSpriteFiles[id - 483] = files;
		}
		profileId = spriteData->profileId;
	} else {
		profileId = id;
	}
	
	profiles[profileId] = this;
	if (profileId < ORIGINAL_PROFILES) {
		profileNames[profileId] = name;
	} else {
		customProfileNames[profileId - ORIGINAL_PROFILES] = name;
	}
}
