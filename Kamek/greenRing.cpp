// <<< Green Ring i Green Coins>>>

// Primerament, declarem els .h necessaris.
#include <common.h>     // Funcions típiques
#include <game.h>       // Conté el relacionat amb el joc
#include <g3dhax.h>     // Models 3d i gràfics
#include <sfx.h>        // Efectes de so

// Ara declarem la classe dEn_c, que serà la del nostre sprite
class daGreenRing_c: public dEn_c { 
    // Funcions bàsiques:
    int onCreate();     // S'executa quan es crea (si es retorna fals, s'executa al frame següent)
    int onDelete();     // S'executa quan es borra un sprite de l'objecte
    int onExecute();    // Fa un loop, s'executa cada frame
    int onDraw();       // Dibuixa l'sprite, s'executa cada frame

    // Necessari per assignar memòria RAM a l'sprite
    mHeapAllocator_c allocator; 

    // Arxiu amb el model 3D i les animacions (.brres)
    nw4r::g3d::ResFile resFile;
    // 3D-bodyModel actual (.brres -> .mdl0-file)
    m3d::mdl_c bodyModel;
    // Animació actual si existeix (.brres -> .chr0-file)
    m3d::anmChr_c chrAnimation;

    // -------------------
    // Variables estàndar
    // -------------------

    int timer;
    float dying;
    bool damage;
    bool isDown;
    Vec initialPos;

    // ------------
    // Coses meves
    // ------------

    // Aquesta funció es cridarà cada cop que s'agafi una moneda verda
    public: int coinCollected();

    // Quantitat de monedes conseguides
    public: int collected;

    // Aquesta variable servirà per fer que l'anell apareixi un frame després. Del contrari, podria no trobar les monedes.
    bool runOnce = false;

    // Això és l'ID del nostre anell
    public: int ringID;

    // Item que es donarà
    int reward;

    // Ara declararem una array amb els grups de monedes
    daGreenCoin_c* coins[5];

    // -------- 
    //  Altres  
    // -------- 

    // Usarem això des del .yaml per carregar l'sprite.
    static daGreenRing_c* build();

    
    void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate); // Això posarà en marxa les animacions
    void updateModelMatrices();     // Dibuixa el model 3D.
    bool calculateTileCollisions(); // Calcula si està tocant una pared. Útil per a fer-lo girar al tocar la pared.

    // Algunes col·lisions importants
    void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther); // Detecta quan el jugador col·lisiona amb aquest sprite
    void spriteCollision(ActivePhysics* apThis, ActivePhysics* apOther); // Detecta si l'sprite col·lisiona amb... un altre sprite?
    void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);  // Detecta si l'sprite col·lisiona amb Yoshi. 



    // Les següents col·lisions son per si l'objecte col·lisiona amb...

    // Mario...
    bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);        // Tenint una estrella
    bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther);            // Lliscant
    bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);            // Baixant ràpid amb un volador
    bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther);	    // Pegant cop darrera una valla
    bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);      // Fent el salt bomba (ground pound)
    bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);	// Salt bomba amb Yoshi
    bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);	    // Lliscant amb el pingüí
    bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther);      // Tirat des d'una tuberia
    
    // Altres objectes...
    bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);		// Un objecte que llisca (tortuga, barril)
    bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); // Bola de foc
    bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);	// Bola de gel
    bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);	            // Martell (hammer-suite)
    bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);	        // Bola de foc tirada per Yoshi 
};

// Degut a que, tant l'anell com la moneda hauran de colaborar junts, els declararem al mateix document. Per això, aquí va la moneda.
class daGreenCoin_c: public dEn_c { 
    // Funcions bàsiques:
    int onCreate();     // S'executa quan es crea (si es retorna fals, s'executa al frame següent)
    int onDelete();     // S'executa quan es borra un sprite de l'objecte
    int onExecute();    // Fa un loop, s'executa cada frame
    int onDraw();       // Dibuixa l'sprite, s'executa cada frame

    // Necessari per assignar memòria RAM a l'sprite
    mHeapAllocator_c allocator; 

    // Arxiu amb el model 3D i les animacions (.brres)
    nw4r::g3d::ResFile resFile;
    // 3D-bodyModel actual (.brres -> .mdl0-file)
    m3d::mdl_c bodyModel;
    // Animació actual si existeix (.brres -> .chr0-file)
    m3d::anmChr_c chrAnimation;

    // -------------------
    // Variables estàndar
    // -------------------

    int timer;
    float dying;
    bool damage;
    bool isDown;
    Vec initialPos;

    // ------------
    // Coses meves
    // ------------

    public: int makeVisible(); // Farà visible la moneda

    // Aquest és l'ID de les monedes
    public: int groupId;

    // Ara declarem l'ID de l'anell on van referits
    public: int ringID;

    // Determina si la moneda serà visible
    public: bool visible;

    // -------
    // Altres
    // -------

    // Usarem això des del .yaml per carregar l'sprite.
    static daGreenCoin_c* build();

    
    void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate); // Això posarà en marxa les animacions
    void updateModelMatrices();     // Dibuixa el model 3D.
    bool calculateTileCollisions(); // Calcula si està tocant una pared. Útil per a fer-lo girar al tocar la pared.

    // Algunes col·lisions importants
    void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther); // Detecta quan el jugador col·lisiona amb aquest sprite
    void spriteCollision(ActivePhysics* apThis, ActivePhysics* apOther); // Detecta si l'sprite col·lisiona amb... un altre sprite?
    void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);  // Detecta si l'sprite col·lisiona amb Yoshi. 



    // Les següents col·lisions son per si l'objecte col·lisiona amb...

    // Mario...
    bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);        // Tenint una estrella
    bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther);            // Lliscant
    bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);            // Baixant ràpid amb un volador
    bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther);	    // Pegant cop darrera una valla
    bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);      // Fent el salt bomba (ground pound)
    bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);	// Salt bomba amb Yoshi
    bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);	    // Lliscant amb el pingüí
    bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther);      // Tirat des d'una tuberia
    
    // Altres objectes...
    bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);		// Un objecte que llisca (tortuga, barril)
    bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); // Bola de foc
    bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);	// Bola de gel
    bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);	            // Martell (hammer-suite)
    bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);	        // Bola de foc tirada per Yoshi 
};

//  Aquí comença el codi de veritat!

//  # ------- #
//  #  ANELL  #
//  # ------- #

// Accedirem a aquesta funció des del .yaml
daGreenRing_c *daGreenRing_c::build() {
    void *buffer = AllocFromGameHeap1(sizeof(daGreenRing_c));
    return new(buffer) daGreenRing_c;
}

// Quan el Green Ring sigui creat...
int daGreenRing_c::onCreate() {

    // Farem que això s'executi un frame més tard
    if (this->runOnce == false) {
        this->runOnce = true;
        return false;
    }

    // Assignem les variables extretes de Reggie 
    this->ringID = this->settings >> 28 & 0xF;
    this->reward = this->settings >> 24 & 0xF;

    // Del contrari, primerament, agafem els 5 grups de monedes.
    // Declarem un iterator pointer (així podrem accedir a ell), que serà sempre de la classe daGreenCoin_c
    daGreenCoin_c *iter = 0;

    // Llavors, el fem buscar els grups de monedes verdes que hi ha. 
    while(iter = (daGreenCoin_c*)fBase_c::search(GreenCoin, iter)) {
        // Si la moneda que s'ha trobat està unida al nostre anell (podrien haver diferents anells), llavors...
        if (iter->ringID == this->ringID) {
            // Afegeix aquesta moneda trobada a l'array de les monedes, a la posició declarada pel seu ID.
            this->coins[iter->groupId] = iter;
        }
    }

    // Fes el de sempre...

    allocator.link(-1, GameHeaps[0], 0, 0x20);
    nw4r::g3d::ResFile rf(getResource("green_ring", "g3d/greenRing"));
    bodyModel.setup(rf.GetResMdl("green_ringA"), &allocator, 0x224, 1, 0);
    SetupTextures_MapObj(&bodyModel, 0);

    allocator.unlink();

    // Ara comencem amb les físiques (ActivePhysics)
    ActivePhysics::Info HitMeBaby;
    HitMeBaby.xDistToCenter = 0.0;
    HitMeBaby.yDistToCenter = 0.0;
    HitMeBaby.xDistToEdge = 15.0;
    HitMeBaby.yDistToEdge = 20.0;
    HitMeBaby.category1 = 0x5;
    HitMeBaby.category2 = 0x0;
    HitMeBaby.bitfield1 = 0x4F; // ARREGLAR
    HitMeBaby.bitfield2 = 0xFFBAFFFE; // ARREGLAR
    HitMeBaby.unkShort1C = 0;
    HitMeBaby.callback = &dEn_c::collisionCallback;

    this->aPhysics.initWithStruct(this, &HitMeBaby);
    this->aPhysics.addToList();

    // Assignem la mida
    this->scale.x = 1.0;
    this->scale.y = 1.0;
    this->scale.z = 1.0;

    // Assignem la posició
    this->pos.x = 120.0;
    this->pos.y = 3300.0;

    return true;
}

// Cada vegada que s'agafa una moneda...
int daGreenRing_c::coinCollected() {
    // Augmenta la quantitat de monedes agafades
    this->collected++;

    // Si s'han agafat menys de 5 monedes...
    if (this->collected < 5) {
        // Fes visible la següent moneda
        this->coins[this->collected]->makeVisible();
    }
    else {
        //Reward
    }
}

//  # -------- #
//  #  MONEDA  #
//  # -------- #

// Accedirem a aquesta funció des del .yaml
daGreenCoin_c *daGreenCoin_c::build() {
    void *buffer = AllocFromGameHeap1(sizeof(daGreenCoin_c));
    return new(buffer) daGreenCoin_c;
}

// Fem la moneda visible i activem tot el relacionat amb models i físiques
int daGreenCoin_c::makeVisible() {
    // Fem visible la moneda (s'activa el que hi ha a l'onExecute())
    this->visible = true;

    // Activa el model
    allocator.link(-1, GameHeaps[0], 0, 0x20);
    nw4r::g3d::ResFile rf(getResource("green_coin", "g3d/greenCoin"));
    bodyModel.setup(rf.GetResMdl("green_coinA"), &allocator, 0x224, 1, 0);
    SetupTextures_MapObj(&bodyModel, 0);

    allocator.unlink();

    // Ara comencem amb les físiques (ActivePhysics)
    ActivePhysics::Info HitMeBaby;
    HitMeBaby.xDistToCenter = 0.0;
    HitMeBaby.yDistToCenter = 0.0;
    HitMeBaby.xDistToEdge = 15.0;
    HitMeBaby.yDistToEdge = 20.0;
    HitMeBaby.category1 = 0x5;
    HitMeBaby.category2 = 0x0;
    HitMeBaby.bitfield1 = 0x4F; // ARREGLAR
    HitMeBaby.bitfield2 = 0xFFBAFFFE; // ARREGLAR
    HitMeBaby.unkShort1C = 0;
    HitMeBaby.callback = &dEn_c::collisionCallback;

    this->aPhysics.initWithStruct(this, &HitMeBaby);
    this->aPhysics.addToList();

    // Assignem la mida
    this->scale.x = 1.0;
    this->scale.y = 1.0;
    this->scale.z = 1.0;

    // Assignem la posició
    this->pos.x = 120.0;
    this->pos.y = 3300.0;
}

// Això passarà la primera vegada
int daGreenCoin_c::onCreate() {

    // Agafem les variables assignades a Reggie
    this->ringID = this->settings >> 28 & 0xF;
    this->groupId = this->settings >> 24 & 0xF;

    return true;
}

int daGreenCoin_c::onExecute() {
    if (this->visible) {
        // Do stuff
    }
}