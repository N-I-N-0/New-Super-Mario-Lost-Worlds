struct GEIFS {
	int starCoins, exits;
};
extern "C" GEIFS *GrabExitInfoForFileSelect(GEIFS *out, SaveBlock *save) {
	out->starCoins = 0;
	out->exits = 0;

	for (int i = 0; i < dLevelInfo_c::s_info.sectionCount(); i++) {
		dLevelInfo_c::section_s *section = dLevelInfo_c::s_info.getSectionByIndex(i);

		for (int j = 0; j < section->levelCount; j++) {
			dLevelInfo_c::entry_s *l = &section->levels[j];
			if (l->flags & 2) {
				//OSReport("Checking %d-%d...\n", l->worldSlot+1, l->levelSlot+1);
				u32 cond = save->GetLevelCondition(l->worldSlot, l->levelSlot);
				if ((l->flags & 0x10) && (cond & COND_NORMAL))
					out->exits++;
				if ((l->flags & 0x20) && (cond & COND_SECRET))
					out->exits++;
				if (cond & COND_COIN1)
					out->starCoins++;
				if (cond & COND_COIN2)
					out->starCoins++;
				if (cond & COND_COIN3)
					out->starCoins++;
			}
		}
	}

	OSReport("Done, got %d coins and %d exits\n", out->starCoins, out->exits);

	return out;
}


extern "C" void SetWorldNameForSaveFile(int worldNumber, nw4r::lyt::TextBox* worldName) {
	OSReport("WorldNumber: %d\n", worldNumber);
	dLevelInfo_c::entry_s *level = dLevelInfo_c::s_info.searchByDisplayNum(worldNumber+1, 100);

	const char *worldNameText = dLevelInfo_c::s_info.getNameForLevel(level);
	
	OSReport("WorldNumber: %s\n", worldNameText);
	
	wchar_t convertedWorldName[32];
	mbstowcs(convertedWorldName, worldNameText, 32);

	worldName->SetString(convertedWorldName);
}