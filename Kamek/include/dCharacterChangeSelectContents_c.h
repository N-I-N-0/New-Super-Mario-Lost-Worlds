#ifndef __CHARACTER_CHANGE_SELECT_CONTENTS
#define __CHARACTER_CHANGE_SELECT_CONTENTS

class dCharacterChangeSelectContents_c : public dBase_c
{
  m2d::EmbedLayout_c layout;

  u8 unk_208[4];

  u32 *acState;

  u8 unk_210[60];

  nw4r::lyt::Pane *N_cSelectCon_00;
  nw4r::lyt::Pane *N_cSelectCon_01;
  nw4r::lyt::Picture *PbuttonShadow_00;
  nw4r::lyt::Picture *PbuttonShadow_01;
  nw4r::lyt::Picture *P_lui_00;
  nw4r::lyt::Picture *P_lui_01;
  nw4r::lyt::Picture *P_kinoB_00;
  nw4r::lyt::Picture *P_kinoB_01;
  nw4r::lyt::Picture *P_kinoY_00;
  nw4r::lyt::Picture *P_kinoY_01;
  nw4r::lyt::TextBox *T_zanki_01;
  nw4r::lyt::TextBox *T_zanki_00;
  nw4r::lyt::TextBox *T_kakeru_00;
  nw4r::lyt::TextBox *T_kakeru_01;

  u8 unk_284[26];

  u8 hasBeenHit;
  u8 hasHitBack;

  u8 unk_2A0[4];

  u32 playerLives[4];
  float unk_2B4;
}; // Total Size: 0x2B8

#endif