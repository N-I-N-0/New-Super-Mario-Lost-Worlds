// <<< Green Ring i Green Coins>>>

// First, we're gonna declare the needed headers.
#include <common.h>     // Common functions
#include <game.h>       // Everything related with the game
#include <g3dhax.h>     // 3d models and graphics
#include <sfx.h>        // Sound effects

const char* GreenRingAndCoinFileList [] = { "green_ring", "obj_coin", NULL };

class daGreenCoin_c;

// Now we declare the dEn_c class, which will be the class of our sprite
class daGreenRing_c: public dEn_c { 
    // Common functions:
    int onCreate();     // This is executed when created (if returned 0, it's executed next frame)
    int onDelete();     // This is executed when a sprite is deleted
    int onExecute();    // This is a loop, every frame will be called
    int onDraw();       // This draws the sprite, every frame

    // This is needed for assigning enough RAM to the sprite
    mHeapAllocator_c allocator; 

    // File with the 3D models and animations (.brres)
    nw4r::g3d::ResFile resFile;
    // 3D-bodyModel actual (.brres -> .mdl0-file)
    m3d::mdl_c bodyModel;
    // Actual animation (.brres -> .chr0-file)
    m3d::anmChr_c chrAnimation;

    // -------------------
    // Stardard variables
    // -------------------

    int timer;
    float dying;
    bool damage;
    bool ringCollected;
    Vec initialPos;

    // ------------
    //   My stuff
    // ------------

    // Every time a green coin is collected, this is called
    public: int coinCollected();

    // Cantity of coins collected
    public: int collected;

    // This variable will make our sprite to execute one frame later. If not, it may be unable to find the coins.
    bool runOnce;

    // This is our ring's id
    public: int ringID;

    // Item rewarded
    int reward;

    // Now we declare an array containing all the coins.
    daGreenCoin_c* coins[5];

    // -------- 
    //  More...  
    // -------- 

    // We'll use this from the .yaml to create the sprite.
    static dActor_c* build();

    
    void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate); // This starts the animation.
    void updateModelMatrices();     // This draws the 3D model.
    
	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther); // Is the player colliding with the sprite?
    
	/*
	bool calculateTileCollisions(); // Are we hitting a wall? This is useful to rotate the sprite if hitting a wall.

    // Some important collisions
    void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther); // Is the player colliding with the sprite?
    void spriteCollision(ActivePhysics* apThis, ActivePhysics* apOther); // If the sprite is colliding with... another sprite?
    void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);  // If the sprite is colliding with... Yoshi?



    // Those collisions are about...

    // Mario...
    bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);        // With a star
    bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther);            // Sliding
    bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);            // Drilling with a propeller
    bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther);	    // Doing a hit behind a wall
    bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);      // Ground pounding
    bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);	// Ground pounding with yoshi
    bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);	    // Sliding with a penguin
    bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther);      // Thrown from a pipe
    
    // Other objects...
    bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);		// Sliding (Koopas, barrels)
    bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); // Fire balls
    bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);	// Ice balls
    bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);	            // Hammers (from the hammer-suite)
    bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);	        // Yoshi's fireballs 
	*/
};

// I'm gonna declare now the green coin here, because both the green coin and the green ring will interact together.
class daGreenCoin_c: public dEn_c { 
    // Commun functions:
    int onCreate();     // This is executed when created (if returned 0, it's executed next frame)
    int onDelete();     // This is executed when a sprite is deleted
    int onExecute();    // This is a loop, every frame will be called
    int onDraw();       // This draws the sprite, every frame

    // This is needed for assigning enough RAM to the sprite
    mHeapAllocator_c allocator; 

    // File with the 3D models and animations (.brres)
    nw4r::g3d::ResFile resFile;
    // 3D-bodyModel actual (.brres -> .mdl0-file)
    m3d::mdl_c bodyModel;
    // Actual animation (.brres -> .chr0-file)
    m3d::anmChr_c chrAnimation;

    // -------------------
    // Standard variables
    // -------------------

    int timer;
    float dying;
    bool damage;
    bool isDown;
    Vec initialPos;

    // ------------
    //   My stuff
    // ------------

    public: int makeVisible(); // This will make visible the coin

    // This is the id of the money
    public: int groupId;

    // This is the id of the ring
    public: int ringID;
	public: daGreenRing_c* ring;

    // This tells us if the money is visible or isn't
    public: bool visible;

    // -------
    // More...
    // -------

    // We'll use this from the .yaml to create the sprite.
    static dActor_c* build();

    
    void bindAnimChr_and_setUpdateRate(const char* name, int unk, float unk2, float rate); // This starts the animation.
    void updateModelMatrices();     // This draws the 3D model.
    
	void playerCollision(ActivePhysics* apThis, ActivePhysics* apOther); // Is the player colliding with the sprite?
    
	/*bool calculateTileCollisions(); // Are we hitting a wall? This is useful to rotate the sprite if hitting a wall.

    // Some important collisions
    void spriteCollision(ActivePhysics* apThis, ActivePhysics* apOther); // If the sprite is colliding with... another sprite?
    void yoshiCollision(ActivePhysics* apThis, ActivePhysics* apOther);  // If the sprite is colliding with... Yoshi?



    // Those collisions are about...

    // Mario...
    bool collisionCat3_StarPower(ActivePhysics *apThis, ActivePhysics *apOther);        // With a star
    bool collisionCat5_Mario(ActivePhysics *apThis, ActivePhysics *apOther);            // Sliding
    bool collisionCatD_Drill(ActivePhysics *apThis, ActivePhysics *apOther);            // Drilling with a propeller
    bool collisionCat8_FencePunch(ActivePhysics *apThis, ActivePhysics *apOther);	    // Doing a hit behind a wall
    bool collisionCat7_GroundPound(ActivePhysics *apThis, ActivePhysics *apOther);      // Ground pounding
    bool collisionCat7_GroundPoundYoshi(ActivePhysics *apThis, ActivePhysics *apOther);	// Ground pounding with yoshi
    bool collisionCatA_PenguinMario(ActivePhysics *apThis, ActivePhysics *apOther);	    // Sliding with a penguin
    bool collisionCat11_PipeCannon(ActivePhysics *apThis, ActivePhysics *apOther);      // Thrown from a pipe
    
    // Other objects...
    bool collisionCat9_RollingObject(ActivePhysics *apThis, ActivePhysics *apOther);		// Sliding (Koopas, barrels)
    bool collisionCat1_Fireball_E_Explosion(ActivePhysics *apThis, ActivePhysics *apOther); // Fire balls
    bool collisionCat2_IceBall_15_YoshiIce(ActivePhysics *apThis, ActivePhysics *apOther);	// Ice balls
    bool collisionCat13_Hammer(ActivePhysics *apThis, ActivePhysics *apOther);	            // Hammers (from the hammer-suite)
    bool collisionCat14_YoshiFire(ActivePhysics *apThis, ActivePhysics *apOther);	        // Yoshi's fireballs
	*/
};

//  Here starts the real code!

//  # ------ #
//  #  RING  #
//  # ------ #

// We'll access this from the .yaml
dActor_c *daGreenRing_c::build() {
    void *buffer = AllocFromGameHeap1(sizeof(daGreenRing_c));
    return new(buffer) daGreenRing_c;
}

const SpriteData GreenRingSpriteData = { ProfileId::GreenRing, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile GreenRingProfile(&daGreenRing_c::build, SpriteId::GreenRing, GreenRingSpriteData, ProfileId::GreenRing, ProfileId::GreenRing, "Green Ring", GreenRingAndCoinFileList);

// When the green ring is created...
int daGreenRing_c::onCreate() {
	OSReport("onCreate\n");

    // Everything will be executed one frame later
    if (this->runOnce == false) {
        this->runOnce = true;
        return false;
    }

    // Those settings are inserted from Reggie 
    this->ringID = this->settings >> 28 & 0xF;
    this->reward = this->settings >> 24 & 0xF;

    // This is an iterator pointer (so we can always access it), and it will always be a daGreenCoin_c class
    daGreenCoin_c *iter = 0;

    // Now, this gets the green coins. 
    while(iter = (daGreenCoin_c*)fBase_c::search(GreenCoin, iter)) {
        // If the coin found is linked to the same ring than our (there might be different rings), then...
        if (iter->ringID == this->ringID) {
            // Add this found money to our coins array, to the position declared in the coin ID.
            this->coins[iter->groupId] = iter;
			iter->ring = this;
        }
    }

    // Do what you always do...

    allocator.link(-1, GameHeaps[0], 0, 0x20);
    nw4r::g3d::ResFile rf(getResource("green_ring", "g3d/green_ring.brres")); // THIS MIGHT NOT BE REAL
    bodyModel.setup(rf.GetResMdl("red_ring"), &allocator, 0x224, 1, 0);
    SetupTextures_MapObj(&bodyModel, 0);

    allocator.unlink();

    // Let's start the physics (ActivePhysics)
    ActivePhysics::Info HitMeBaby;
    HitMeBaby.xDistToCenter = 0.0;
    HitMeBaby.yDistToCenter = 0.0;
    HitMeBaby.xDistToEdge = 15.0;
    HitMeBaby.yDistToEdge = 20.0;
    HitMeBaby.category1 = 0x5;
    HitMeBaby.category2 = 0x0;
    HitMeBaby.bitfield1 = 0x4F; // TODO
    HitMeBaby.bitfield2 = 0xFFBAFFFE; // TODO
    HitMeBaby.unkShort1C = 0;
    HitMeBaby.callback = &dEn_c::collisionCallback;

    this->aPhysics.initWithStruct(this, &HitMeBaby);
    this->aPhysics.addToList();

    // Here we set the scale
    this->scale.x = 1.0;
    this->scale.y = 1.0;
    this->scale.z = 1.0;

    // And the position
    //this->pos.x = 120.0;
    //this->pos.y = 3300.0;

    return true;
}

// Every time a coin is collected...
int daGreenRing_c::coinCollected() {
    // Increase the collected coins var
    this->collected++;

    // If there are less than 5 moneys collected...
    if (this->collected < 5) {
        // Make visible next coin
        this->coins[this->collected]->makeVisible();
    }
    else {
        // REWARD, TODO
    }
}

int daGreenRing_c::onDelete() {
	return true;
}

int daGreenRing_c::onDraw() {
	if(!ringCollected) {
		bodyModel.scheduleForDrawing();
	}

	return true;
}

int daGreenRing_c::onExecute() {
	if(!ringCollected) {
		updateModelMatrices();
		bodyModel._vf1C();
	}
}

void daGreenRing_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}


void daGreenRing_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	timer = 180;
	this->coins[0]->makeVisible();
	aPhysics.removeFromList();
	ringCollected = true;
}














//  # ------ #
//  #  COIN  #
//  # ------ #

// We'll access this function from the .yaml
dActor_c *daGreenCoin_c::build() {
    void *buffer = AllocFromGameHeap1(sizeof(daGreenCoin_c));
    return new(buffer) daGreenCoin_c;
}

const SpriteData GreenCoinSpriteData = { ProfileId::GreenCoin, 0, 0, 0, 0, 0x100, 0x100, 0, 0, 0, 0, 0 };
Profile GreenCoinProfile(&daGreenCoin_c::build, SpriteId::GreenCoin, GreenCoinSpriteData, ProfileId::GreenCoin, ProfileId::GreenCoin, "Green Coin", GreenRingAndCoinFileList);

// We make the coin visible and we activate everything, including models and animations
int daGreenCoin_c::makeVisible() {
    // Make the coin visible (so now the onExecute() function will work)
    this->visible = true;

    // Enable the model
    allocator.link(-1, GameHeaps[0], 0, 0x20);
    nw4r::g3d::ResFile rf(getResource("obj_coin", "g3d/obj_coin.brres")); 
    bodyModel.setup(rf.GetResMdl("obj_coin_green"), &allocator, 0x224, 1, 0);
    SetupTextures_MapObj(&bodyModel, 0);

    allocator.unlink();

    // Start the physics (ActivePhysics)
    ActivePhysics::Info HitMeBaby;
    HitMeBaby.xDistToCenter = 0.0;
    HitMeBaby.yDistToCenter = 0.0;
    HitMeBaby.xDistToEdge = 15.0;
    HitMeBaby.yDistToEdge = 20.0;
    HitMeBaby.category1 = 0x5;
    HitMeBaby.category2 = 0x0;
    HitMeBaby.bitfield1 = 0x4F; // TODO
    HitMeBaby.bitfield2 = 0xFFBAFFFE; // TODO
    HitMeBaby.unkShort1C = 0;
    HitMeBaby.callback = &dEn_c::collisionCallback;

    this->aPhysics.initWithStruct(this, &HitMeBaby);
    this->aPhysics.addToList();

    // Here we set the scale
    this->scale.x = 1.0;
    this->scale.y = 1.0;
    this->scale.z = 1.0;

    // And the position
    //this->pos.x = 120.0;
    //this->pos.y = 3300.0;
}

// This will happen firstly
int daGreenCoin_c::onCreate() {

    // We get some vars from Reggie
    this->ringID = this->settings >> 28 & 0xF;
    this->groupId = this->settings >> 24 & 0xF;

    return true;
}

int daGreenCoin_c::onExecute() {
    if (this->visible) {
        // TODO
		updateModelMatrices();
		bodyModel._vf1C();
    }
}

int daGreenCoin_c::onDraw() {
	bodyModel.scheduleForDrawing();

	return true;
}

void daGreenCoin_c::updateModelMatrices() {
	matrix.translation(pos.x, pos.y, pos.z);
	matrix.applyRotationYXZ(&rot.x, &rot.y, &rot.z);

	bodyModel.setDrawMatrix(matrix);
	bodyModel.setScale(&scale);
	bodyModel.calcWorld(false);
}

int daGreenCoin_c::onDelete() {
	return true;
}

void daGreenCoin_c::playerCollision(ActivePhysics* apThis, ActivePhysics* apOther) {
	ring->coinCollected();
	this->Delete(1);
}