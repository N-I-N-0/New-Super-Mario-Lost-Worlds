#include "courseClear.h"

dCourseClear_c *dCourseClear_c::instance = 0;

int dCourseClear_c::newOnCreate() {
	int orig_val = this->onCreate_orig();

	instance = this;

	return orig_val;
}