bool isCSMgrStockItem() {
	return (dCourseSelectManager_c::instance->state.getCurrentState() == &dCourseSelectManager_c::StateID_StockItemSelectWait);
}