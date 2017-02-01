#include "ofBitchSkeletonApp.h"
#include <fstream>  

//--------------------------------------------------------------
void ofBitchSkeletonApp::setup(){
    setVoice( );

    textCurrent = bitches.getAnswerCurrent();

    auto bufferSize = 512;
    auto sampleRate = 44100;

    analyser.setup( sampleRate, bufferSize * 2, bufferSize, bufferSize / 2, 100 ); // call before ofSoundStreamSetup()
    ofSoundStreamSetup( 1, 1, this, sampleRate, bufferSize, 1 );

    gui2 = new ofxUISuperCanvas( "Talk to Elisa", 400, 300, 200, 100 );
    textInput = gui2->addTextInput("TEXT: ", "Hello Elisa");
    textInput->setAutoUnfocus(false);
    textInput->setAutoClear( false );

    // OSC version
    string host             = "localhost";
    string adressToPython   = "/python_here";
    string adressFromPython = "/transform";

    auto portToPython    = 22222;  // OF side
    auto portFromPython  = 33333;  // Python side

    bitchElisa.setup( host, portToPython, portFromPython, adressToPython, adressFromPython );
    bitchKora.setup( host, portFromPython, portToPython, adressFromPython, adressToPython );
}

//--------------------------------------------------------------
void ofBitchSkeletonApp::update(){
    speak( );
    bitchElisa.update();
    bitchKora.update();
}

//--------------------------------------------------------------
void ofBitchSkeletonApp::draw(){
    auto gap = 10;
    auto shift = 0;
    if ( bitches.getTalkerCurrentID() == 0 )
    {
        shift = 0;
    }
    else {
        shift = ofGetWidth() / 2;
    }

    ofPushStyle( );
        ofDrawRectangle( gap + shift, gap, ofGetWidth( ) / 2 - 2 * gap, ofGetHeight( ) - 2 * gap );
    ofPopStyle( );

    auto scale = 1.0;
    drawVoice( scale );

    drawText( );

    gui2->draw( );
}

void ofBitchSkeletonApp::drawVoice( double scale )
{
    ofPushStyle( );
    ofSetColor( ofColor::red );
    ofDrawRectangle( 0, ofGetHeight() / 2, ofGetWidth( ), ofGetHeight( ) * analyser.getAmplitude() * scale );
    ofPopStyle( );
}

//--------------------------------------------------------------
void ofBitchSkeletonApp::keyPressed(int key){

    if  ( key == ' ' )
    {
        if ( !textInput->isFocused() )
        {
            bitches.next();
            bitches.doConversation();

            reset();
        }
    }

    if ( key == OF_KEY_RETURN )
    {
        textFromInput = textInput->getTextString();
        textInput->setTextString( "" );

        bitches.doConversation( textFromInput, 0 );
        bitchElisa.setTextToSend( textFromInput );

        reset();
    }

}

void ofBitchSkeletonApp::reset()
{
    textCurrent = bitches.getAnswerCurrent();
    textLast    = bitches.getAnswerBefore();
    setVoice();
    shouldSpeak = true;
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void ofBitchSkeletonApp::setVoice()
{
    if ( bitches.getNameSpeaker() == "Elisa" ) { voice = "Allison"; }
    else {
        voice = "Tom";
    }

}
void ofBitchSkeletonApp::speak()
{
    if ( shouldSpeak )
    {
        ofstream finofs ("/Users/nesa/transformer_in", std::ofstream::out); //declare a file stream
        finofs<<textCurrent;
        finofs.close();

/*      string line;
        ifstream myfile ("/Users/nesa/transformer_out");
        if (myfile.is_open())
        {
            while ( getline (myfile,line) )
            {
                cout << line << '\n';
            }
            myfile.close();
        }
 */
        string cmd = "say -v " + voice + " " + textCurrent + " &";   // create the command
        system(cmd.c_str());
        shouldSpeak = false;
        

    }
}

void ofBitchSkeletonApp::drawText()
{
    ofDrawBitmapStringHighlight( bitches.getName( 0 ) , ofGetWidth() / 4 , 100);
    ofDrawBitmapStringHighlight( bitches.getName( 1 ) , ofGetWidth() * 3 / 4, 100);

    ofDrawBitmapStringHighlight( bitches.getAnswerFromID( 0 ), 100, 200 );
    ofDrawBitmapStringHighlight( bitches.getAnswerFromID( 1 ), 600, 200 );
    ofDrawBitmapStringHighlight( "OSC: " + bitchElisa.getTextRecieved(), 100, 220 );
    ofDrawBitmapStringHighlight( "OSC: " + bitchKora.getTextRecieved(), 600, 220 );

    ofDrawBitmapStringHighlight( "voice: " + voice, ofGetWidth() - 180, ofGetHeight() - 20 );
    ofDrawBitmapStringHighlight( "Said to Elisa: " + textFromInput, 10, ofGetHeight() - 20 );
}

void ofBitchSkeletonApp::audioIn( float*input, int bufferSize, int nChannels )
{
    analyser.grabAudioBuffer( input );
}
