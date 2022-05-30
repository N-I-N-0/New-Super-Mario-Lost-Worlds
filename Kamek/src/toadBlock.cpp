extern "C" void ToadBlock_DisappearWithBooLaugh(dStageActor_c*);
void ToadBlock_DisappearWithBooLaugh(dStageActor_c *this_) {
    PlaySound(this_, SE_EMY_CS_TERESA_BEAT_YOU);
    S16Vec nullRot = {0, 0, 0};
    Vec oneVec = {1.0f, 1.0f, 1.0f};
    SpawnEffect("Wm_en_obakedoor", 0, &this_->pos, &nullRot, &oneVec);

    // event_nybbles & 0xff: optional event ID to trigger when the fake block is opened
    int event_id = this_->spriteFlagNum & 0xff;
    if (event_id) {
        dFlagMgr_c::instance->set(event_id - 1, 0, true, false, false);
    }

    this_->Delete(1);
}