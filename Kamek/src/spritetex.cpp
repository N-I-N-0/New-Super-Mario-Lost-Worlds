int getBubbleColor() {
	dCourse_c *course = dCourseFull_c::instance->get(GetAreaNum());
	int zoneNum = GetZoneNum();
	dCourse_c::zone_s* zone = course->getZoneByID(zoneNum);
	u16 color = zone->modelShading;
	
	OSReport("Color: %d\n", color);
	return 1;
}