int getBubbleColor() {
	dCourse_c *course = dCourseFull_c::instance->get(GetAreaNum());
	int zoneNum = GetZoneNum();
	dCourse_c::zone_s* zone = course->getZoneByID(zoneNum);
	u16 color = zone->modelShading;

	//OSReport("Color: %d\n", color);
	return color;
}



extern "C" void dAcPy_vf3F4(void* mario, void* other, int t);

void performAction(dStageActor_c* cloud, dAcPy_c* player) {
    switch (cloud->settings >> 24 & 0xF) {
        case 1:
            dAcPy_vf3F4(player, 0, 0x12);
            break;
        case 2:
            if (player->states2.getCurrentState() != &daPlBase_c::StateID_IceDamage)
                CreateActor(BROS_ICEBALL, 0x10000000, player->pos, 0, 0);
            break;
    }
}


void collisionCheck(dStageActor_c* cloud, float y, float x) {
    for (int i = 0; i < GetActivePlayerCount(); i++) {
        dAcPy_c* player = dAcPy_c::findByID(i);

        if (player->pos.x > (x - 6.0f) && player->pos.x < (x + 6.0f)) {
            if (player->pos.y > (y - 6.0f) && player->pos.y < (y + 6.0f)) {
                performAction(cloud, player);
            }
        }
    }
}

bool shouldDeactivate(dActor_c* iceball) {
    if (iceball->settings >> 28 & 0xF)
        return 1;
    else
        return 0;
}


void FireSnakePlayerCollision(dEn_c* snake, ActivePhysics* apThis, ActivePhysics* apOther) {
    if ((snake->settings >> 24 & 0xF) == 1) {
        dAcPy_c* player = (dAcPy_c*)apOther->owner;
        if (player->states2.getCurrentState() != &daPlBase_c::StateID_IceDamage) {
            CreateActor(BROS_ICEBALL, 0x10000000, player->pos, 0, 0);
        }
    } else {
        snake->dEn_c::playerCollision(apThis, apOther);
    }
}

extern "C" bool origFireSnakeIceballCollision(dEn_c* snake, ActivePhysics* apThis, ActivePhysics* apOther);

bool FireSnakeIceballCollision(dEn_c* snake, ActivePhysics* apThis, ActivePhysics* apOther) {
    if ((snake->settings >> 24 & 0xF) == 1) {
		snake->dEn_c::iceballInvalid(apThis, apOther);
	} else {
		origFireSnakeIceballCollision(snake, apThis, apOther);
	}
}

bool FireSnakeFireCollision(dEn_c* snake, ActivePhysics* apThis, ActivePhysics* apOther) {
    if ((snake->settings >> 24 & 0xF) == 1) {
		origFireSnakeIceballCollision(snake, apThis, apOther);
	} else {
		snake->dEn_c::fireballInvalid(apThis, apOther);
	}
}