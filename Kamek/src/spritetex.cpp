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
                CreateActor(BROS_ICEBALL, 0, player->pos, 0, 0);
            break;
    }
}


void collisionCheck(dStageActor_c* cloud, float y, float x) {
    for (int i = 0; i < GetActivePlayerCount(); i++) {
        dAcPy_c* player = dAcPy_c::findByID(i);

        if (player->pos.x > (x - 16.0f) && player->pos.x < (x + 16.0f)) {
            if (player->pos.y > (y - 16.0f) && player->pos.y < (y + 16.0f)) {
                performAction(cloud, player);
            }
        }
    }
}