// Copyright (C) 2021 RoadrunnerWMC

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#include <game.h>
#include <dCourse.h>


#define ONCREATE_RESULT_RUN_ME_AGAIN_NEXT_FRAME 0
#define ONCREATE_RESULT_SUCCESS 1
#define ONCREATE_RESULT_PLEASE_DELETE_ME 2

// Values match the respective course block indices
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_AREA_SETTINGS 0
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_BOUNDS 2
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_BGA 4
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_BGB 5
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_ENTRANCES 6
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_ZONES 9
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_LOCATIONS 10
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATHS 12
#define UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATH_NODES 13

// Set: set the field to the specified value
#define UNIVERSAL_RECONFIGURATOR_APPLICATION_MODE_SET 0
// Add: add the specified value to the field
#define UNIVERSAL_RECONFIGURATOR_APPLICATION_MODE_ADD 1

// Single: apply once and then never again (until the zone is reloaded, at least)
#define UNIVERSAL_RECONFIGURATOR_USAGE_MODE_SINGLE 0
// Multi: apply every time the event is activated
#define UNIVERSAL_RECONFIGURATOR_USAGE_MODE_MULTI 1
// Only While: same as Multi, but also restore the previous value every time the event is deactivated
#define UNIVERSAL_RECONFIGURATOR_USAGE_MODE_ONLY_WHILE 2


class daUniversalReconfigurator_c : public dStageActor_c {
public:

    u8 itemId;
    u8 applicationMode : 1;
    u8 eventId : 7;
    u8 itemSubId;
    u8 fieldId;
    s16 fieldValue;
    s16 storedFieldValue;
    u8 itemType        : 4;
    u8 isActive        : 1;
    u8 isDoneForever   : 1;
    u8 usageMode       : 2;
    u8 isSecondTimeRunningOnCreate : 1;
    u8 areaId : 3;

    void *itemPtr;

    static dActor_c *build();

    dCourse_c::rail_s *getPathFromCourse(dCourse_c *course, int pathId);
    bool populateItemPtr();
    s32 getEntranceFieldValue();
    void setEntranceFieldValue(s32 value);
    s32 getLocationFieldValue();
    void setLocationFieldValue(s32 value);
    s32 getPathFieldValue();
    void setPathFieldValue(s32 value);
    s32 getPathNodeFieldValue();
    void setPathNodeFieldValue(s32 value);
    s32 getFieldValue();
    void setFieldValue(s32 value);

    int onCreate();
    int onExecute();
};

dActor_c *daUniversalReconfigurator_c::build() {
    return new(AllocFromGameHeap1(sizeof(daUniversalReconfigurator_c))) daUniversalReconfigurator_c;
};

int daUniversalReconfigurator_c::onCreate() {
    // Run onCreate twice so we get access to all spritedata nybbles
    if (!isSecondTimeRunningOnCreate) {
        isSecondTimeRunningOnCreate = true;
        return ONCREATE_RESULT_RUN_ME_AGAIN_NEXT_FRAME;
    }

    // Spritedata:
    // ABCC DDEE FFFF __GG
    // A = item type
    // B =...
    //     value / 3: area ID (0, 1, 2, 3, 4)
    //     value % 3: usage mode (0, 1, 2)
    // CC =...
    //     Top bit: application mode (0, 1)
    //     Lower 7 bits: event ID
    // DD = item ID
    // EE = field ID
    // FFFF = field value
    // GG = item sub-ID (mainly for path nodes)

    // Note: you could also free up the second bit in nybble 3 if you rename event 64
    // to "event 0," since 0 isn't a meaningful event ID value for this actor anyway

    itemType = spriteFlagNum >> 12;
    int nybble2 = (spriteFlagNum >> 8) & 0xf;
    areaId = nybble2 / 3;
    usageMode = nybble2 % 3;
    applicationMode = (spriteFlagNum >> 7) & 1;
    eventId = spriteFlagNum & 0x7f;
    itemId = settings >> 24;
    fieldId = (settings >> 16) & 0xff;
    fieldValue = settings & 0xffff;
    itemSubId = currentLayerID;

    if (areaId > 4) areaId = 4;

    storedFieldValue = 0;
    isActive = false;
    isDoneForever = false;

    if (!populateItemPtr())
        return ONCREATE_RESULT_PLEASE_DELETE_ME;

    return ONCREATE_RESULT_SUCCESS;
};

#define PATHS_BLOCK_INDEX 12
dCourse_c::rail_s *daUniversalReconfigurator_c::getPathFromCourse(dCourse_c *course, int pathId) {
    for (int i = 0; i < course->blockCounts[PATHS_BLOCK_INDEX]; i++) {
        dCourse_c::rail_s *path = (dCourse_c::rail_s*)(course->blocks[PATHS_BLOCK_INDEX]);
        if (path->entryID == pathId)
            return path;
    }
    return NULL;
};

bool daUniversalReconfigurator_c::populateItemPtr() {
    dCourse_c *course = dCourseFull_c::instance->get((areaId == 0) ? GetAreaNum() : (areaId - 1));
    dCourse_c::rail_s *tmp_path;

    switch (itemType) {
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_ENTRANCES:
            itemPtr = course->getNextGotoByID(itemId);
            break;
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_LOCATIONS:
            itemPtr = course->getRectByID(itemId);
            break;
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATHS:
            itemPtr = getPathFromCourse(course, itemId);
            break;
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATH_NODES:
            tmp_path = getPathFromCourse(course, itemId);
            if (tmp_path == NULL) return false;
            if (itemSubId >= tmp_path->nodeCount) return false;
            itemPtr = &course->railNode[tmp_path->startNode + itemSubId];
            break;
        default:
            OSReport("Universal Reconfigurator: unsupported item type: %d\n", itemType);
            return false;
    }

    return itemPtr != NULL;
}

s32 daUniversalReconfigurator_c::getEntranceFieldValue() {
    dCourse_c::nextGoto_s *entrance = (dCourse_c::nextGoto_s*)itemPtr;

    if ((fieldId >> 4) == 0) {
        switch (fieldId) {
            case 0:  return entrance->xPos;
            case 1:  return entrance->yPos;
            case 2:  return entrance->cameraXPos;
            case 3:  return entrance->cameraYPos;
            case 4:  return entrance->entryID;
            case 5:  return entrance->destArea;
            case 6:  return entrance->destEntrance;
            case 7:  return entrance->type;
            case 8:  return entrance->unk1;
            case 9:  return entrance->zoneID;
            case 10: return entrance->layerID;
            case 11: return entrance->pathID;
            case 12: return entrance->unk2;
            default: return 0;
        }

    } else {
        int flagId = fieldId & 0xf;
        return (entrance->flags >> (15 - flagId)) & 1;
    }
};

void daUniversalReconfigurator_c::setEntranceFieldValue(s32 value) {
    dCourse_c::nextGoto_s *entrance = (dCourse_c::nextGoto_s*)itemPtr;

    if ((fieldId >> 4) == 0) {
        switch (fieldId) {
            case 0:  entrance->xPos = value; break;
            case 1:  entrance->yPos = value; break;
            case 2:  entrance->cameraXPos = value; break;
            case 3:  entrance->cameraYPos = value; break;
            case 4:  entrance->entryID = value; break;
            case 5:  entrance->destArea = value; break;
            case 6:  entrance->destEntrance = value; break;
            case 7:  entrance->type = value; break;
            case 8:  entrance->unk1 = value; break;
            case 9:  entrance->zoneID = value; break;
            case 10: entrance->layerID = value; break;
            case 11: entrance->pathID = value; break;
            case 12: entrance->unk2 = value; break;
        }

    } else {
        int flagId = fieldId & 0xf;
        u16 mask = (u16)1 << (15 - flagId);

        if (value == 0) {
            entrance->flags &= ~mask;
        } else {
            entrance->flags |= mask;
        }
    }
};

s32 daUniversalReconfigurator_c::getLocationFieldValue() {
    dCourse_c::rect_s *location = (dCourse_c::rect_s*)itemPtr;

    switch (fieldId) {
        case 0:  return location->xPos;
        case 1:  return location->yPos;
        case 2:  return location->xSize;
        case 3:  return location->ySize;
        case 4:  return location->entryID;
        case 5:  return location->unused[0];
        case 6:  return location->unused[1];
        case 7:  return location->unused[2];
        default: return 0;
    }
};

void daUniversalReconfigurator_c::setLocationFieldValue(s32 value) {
    dCourse_c::rect_s *location = (dCourse_c::rect_s*)itemPtr;

    switch (fieldId) {
        case 0: location->xPos = value; break;
        case 1: location->yPos = value; break;
        case 2: location->xSize = value; break;
        case 3: location->ySize = value; break;
        case 4: location->entryID = value; break;
        case 5: location->unused[0] = value; break;
        case 6: location->unused[1] = value; break;
        case 7: location->unused[2] = value; break;
    }
};

s32 daUniversalReconfigurator_c::getPathFieldValue() {
    dCourse_c::rail_s *path = (dCourse_c::rail_s*)itemPtr;

    if ((fieldId >> 4) == 0) {
        switch (fieldId) {
            case 0:  return path->entryID;
            case 1:  return path->unknown;
            case 2:  return path->startNode;
            case 3:  return path->nodeCount;
            default: return 0;
        }

    } else {
        int flagId = fieldId & 0xf;
        return (path->flags >> (15 - flagId)) & 1;
    }
};

void daUniversalReconfigurator_c::setPathFieldValue(s32 value) {
    dCourse_c::rail_s *path = (dCourse_c::rail_s*)itemPtr;

    if ((fieldId >> 4) == 0) {
        switch (fieldId) {
            case 0: path->entryID = value; break;
            case 1: path->unknown = value; break;
            case 2: path->startNode = value; break;
            case 3: path->nodeCount = value; break;
        }

    } else {
        int flagId = fieldId & 0xf;
        u16 mask = (u16)1 << (15 - flagId);

        if (value == 0) {
            path->flags &= ~mask;
        } else {
            path->flags |= mask;
        }
    }
};

s32 daUniversalReconfigurator_c::getPathNodeFieldValue() {
    dCourse_c::railNode_s *pathNode = (dCourse_c::railNode_s*)itemPtr;

    switch (fieldId) {
        case 0:  return pathNode->xPos;
        case 1:  return pathNode->yPos;
        case 2:  return ((u16*)(&pathNode->speed))[0];
        case 3:  return ((u16*)(&pathNode->speed))[1];
        case 4:  return ((u16*)(&pathNode->accel))[0];
        case 5:  return ((u16*)(&pathNode->accel))[1];
        case 6:  return pathNode->unk1;
        case 7:  return pathNode->unk2;
        default: return 0;
    }
};

void daUniversalReconfigurator_c::setPathNodeFieldValue(s32 value) {
    dCourse_c::railNode_s *pathNode = (dCourse_c::railNode_s*)itemPtr;

    switch (fieldId) {
        case 0: pathNode->xPos = value; break;
        case 1: pathNode->yPos = value; break;
        case 2: ((u16*)(&pathNode->speed))[0] = value; break;
        case 3: ((u16*)(&pathNode->speed))[1] = value; break;
        case 4: ((u16*)(&pathNode->accel))[0] = value; break;
        case 5: ((u16*)(&pathNode->accel))[1] = value; break;
        case 6: pathNode->unk1 = value; break;
        case 7: pathNode->unk2 = value; break;
    }
};

s32 daUniversalReconfigurator_c::getFieldValue() {
    switch (itemType) {
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_ENTRANCES: return getEntranceFieldValue();
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_LOCATIONS: return getLocationFieldValue();
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATHS: return getPathFieldValue();
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATH_NODES: return getPathNodeFieldValue();
        default: return 0;
    }
};

void daUniversalReconfigurator_c::setFieldValue(s32 value) {
    switch (itemType) {
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_ENTRANCES: setEntranceFieldValue(value); break;
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_LOCATIONS: setLocationFieldValue(value); break;
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATHS: setPathFieldValue(value); break;
        case UNIVERSAL_RECONFIGURATOR_ITEM_TYPE_PATH_NODES: setPathNodeFieldValue(value); break;
    }
};

int daUniversalReconfigurator_c::onExecute() {
    if (isDoneForever)
        return 1;

    bool nowActive = dFlagMgr_c::instance->active(eventId - 1);

    if (nowActive != isActive) {
        // Handle inactive -> active and active -> inactive separately
        if (!isActive && nowActive) {
            // Event was just activated

            storedFieldValue = getFieldValue();
            if (applicationMode == UNIVERSAL_RECONFIGURATOR_APPLICATION_MODE_SET)
                setFieldValue(fieldValue);
            else if (applicationMode == UNIVERSAL_RECONFIGURATOR_APPLICATION_MODE_ADD)
                setFieldValue(storedFieldValue + fieldValue);

            // In single-use mode? Disable ourselves forever
            if (usageMode == UNIVERSAL_RECONFIGURATOR_USAGE_MODE_SINGLE)
                isDoneForever = true;

        } else if (isActive && !nowActive && usageMode == UNIVERSAL_RECONFIGURATOR_USAGE_MODE_ONLY_WHILE) {
            // Event was just deactivated, and we're on "only while" mode.
            // Restore the previous value
            setFieldValue(storedFieldValue);
            storedFieldValue = 0;
        }

        isActive = nowActive;
    }

    return 1;
};


//OVERWRITE_PROFILE(UNIVERSAL_RECONFIGURATOR, UNIVERSAL_RECONFIGURATOR_PROFILE_NUM, &daUniversalReconfigurator_c::build, UNIVERSAL_RECONFIGURATOR_PROFILE_NUM, UNIVERSAL_RECONFIGURATOR_PROFILE_NUM, 0);

//OVERWRITE_SPRITE_DATA(UNIVERSAL_RECONFIGURATOR_SPRITE_NUM, UNIVERSAL_RECONFIGURATOR_PROFILE_NUM, 0, 0, 0, 0, 0, 0, SPRITE_FLAGS_ALWAYS_SPAWNED);

//OVERWRITE_SPRITE_RESOURCES_NULL(UNIVERSAL_RECONFIGURATOR_SPRITE_NUM);




const char* UniversalReconfiguratorFileList [] = { NULL };
const SpriteData UniversalReconfiguratorSpriteData = { ProfileId::UniversalReconfigurator, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile UniversalReconfiguratorProfile(&daUniversalReconfigurator_c::build, SpriteId::UniversalReconfigurator, &UniversalReconfiguratorSpriteData, ProfileId::UniversalReconfigurator, ProfileId::UniversalReconfigurator, "UniversalReconfigurator", UniversalReconfiguratorFileList);
