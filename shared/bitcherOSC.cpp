//
// Created by Peter A. Kolski on 01.02.17.
//

#include "bitcherOSC.h"

void bitcherOSC::setup( string &hostSendingTo, int portSender, int portReciever )
{
    hostSendSendingTo_  = hostSendingTo;
    portSender_         = portSender;
    portReciever_       = portReciever;

    string adressRecieverTMP = "/answer";
    adressRecieverText_     = adressRecieverTMP + "_text";

    string adressSenderTMP  = "/original";
    adressSenderText_       = adressSenderTMP + "_text";

    ofLogVerbose() << logInfo_ << "Host: " << hostSendSendingTo_;
    ofLogVerbose() << logInfo_ << "Port Sending to: " << portSender_;
    ofLogVerbose() << logInfo_ << "Port Recieving from: " << portReciever_;
    ofLogVerbose() << logInfo_ << "Adress Sender: " << adressSenderText_;
    ofLogVerbose() << logInfo_ << "Adress Reciever: " << adressRecieverText_;

    sender_.setup( hostSendSendingTo_, portSender_ );
    reciever_.setup( portReciever_ );
}



string bitcherOSC::ask( string &text )
{
    textSent_ = text;
    sendText( textSent_ );

    recieveOscText();
    textRecieved_ = getAnswer();
    return textRecieved_;
}



void bitcherOSC::sendText( string &text )
{
    ofLogVerbose() << logInfo_ << "Sending port " << portSender_ << " | adr " << adressSenderText_;
    ofLogVerbose() << logInfo_ << "Text " << text;
    ofxOscMessage m;
    m.setAddress( adressSenderText_ );
    m.addStringArg( text );
    sender_.sendMessage( m, false );
}



/// Checking OSC if a message in port got recieved.
/// Containing [string, bool] = [Text, switch ot mutant ]
/// \return     true if recieved message
bool bitcherOSC::recieveOscText()
{
    bool isReceived = false;
    while ( reciever_.hasWaitingMessages() )
    {
        ofxOscMessage _message;
        reciever_.getNextMessage( _message );
        ofLogVerbose() << logInfo_ << " Recieved message from " + _message.getRemoteIp();

        if (    ( _message.getAddress() == adressRecieverText_ )
            &&  ( _message.getNumArgs() > 0 ) )
         {
             textRecieved_ = _message.getArgAsString( 0 );
             switchMutantChatbot_ = _message.getArgAsBool( 1 ); // If the chatbot system should be switched, it's true
             isReceived = true;
             ofLogVerbose() << logInfo_ << "Message from " << adressRecieverText_ << " | " << textRecieved_;
             if ( switchMutantChatbot_ ) { ofLogVerbose() << logInfo_ << "Chatbot switch"; }
         }
        else
        {
            ofLogWarning() << logInfo_ << "Message did't contain anything: " << _message.getAddress();
        }
    }

    return isReceived;
}


void bitcherOSC::startMutant( string txt1, string txt2 )
{
    ofxOscMessage m;
    m.setAddress( "/generate" );
    m.addStringArg( txt1 );
    m.addStringArg( txt2 );
    sender_.sendMessage( m, false );
}



