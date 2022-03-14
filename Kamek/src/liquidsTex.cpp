u8 liquidSubTypes[80];
void ClearSubTypesArray() {
	for (int i = 0; i < 80; i++) {
		liquidSubTypes[i] = 0;
	}
	return;
}