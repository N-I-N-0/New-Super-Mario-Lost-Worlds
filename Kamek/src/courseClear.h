#ifndef COURSECLEAR_H
#define COURSECLEAR_H

class dCourseClear_c : public dBase_c {
public:
	m2d::EmbedLayout_c layout;
	u8 data[0x330 - 0x208];
	dStateWrapper_c<dCourseClear_c> acState;

	static dCourseClear_c *instance;

	int onCreate_orig();
	int newOnCreate();

	int onExecute_orig();
	int newOnExecute();
};

#endif /* COURSECLEAR_H */