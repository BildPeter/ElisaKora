
set( APP_NAME   TalkingEliza )


# --------------------- Source Files ---------------------
set( ${APP_NAME}_SOURCE_FILES
        src/main.cpp
        src/ofBitchSkeletonApp.cpp
        ../../shared/ConversationOSC.cpp
        ../../shared/bitcherOSC.cpp
        ../../shared/Tokenizer.cpp
        )

# ------------------------ AddOns ------------
set( OFX_ADDONS_ACTIVE
        ofxQuadWarp
        ofxOpenCv
        ofxSyphon
        ofxXmlSettings
        ofxOsc
        ofxGui
        )

# =========================================================================
# ============================== OpenFrameworks ===========================
# =========================================================================
include( ${OF_DIRECTORY_BY_USER}/addons/ofxCMake/modules/main.cmake )
# =========================================================================
