#include "baseApp.h"

//--------------------------------------------------------------
void baseApp::setup(){
	ofBackground(255,255,255);
    ofEnableAlphaBlending();
    
    maskTop.loadImage("maskTop.png");
    maskBottom.loadImage("maskBottom.png");

    topMovie = new ofVideoPlayer();
	middleMovie = new ofVideoPlayer();
    bottomMovie = new ofVideoPlayer();
    
    topMovie->loadMovie("movies/top.mov");
    middleMovie->loadMovie("movies/middle.mov");
    bottomMovie->loadMovie("movies/bottom.mov");
    
    files[0] = "movies/top.mov";
    files[1] = "movies/middle.mov";
    files[2] = "movies/bottom.mov";
    
    maskShaderTop.load("composite");
	maskShaderTop.begin();
	maskShaderTop.setUniformTexture("Tex0", topMovie->getTextureReference(), 0);
	maskShaderTop.setUniformTexture("Tex1", maskTop.getTextureReference(), 1);
	maskShaderTop.end();
    
    maskShaderBottom.load("composite");
	maskShaderBottom.begin();
	maskShaderBottom.setUniformTexture("Tex0", topMovie->getTextureReference(), 0);
	maskShaderBottom.setUniformTexture("Tex1", maskTop.getTextureReference(), 1);
	maskShaderBottom.end();
    
	topMovie->play();
    middleMovie->play();
    bottomMovie->play();

	cam.close();
	cam.setVerbose(true);
	cam.initGrabber(320, 240);
    mytimeThen = 0.0f;  
    myframeRate = 0.0f;  
    myframes = 0.0f; 
    
    millisCounter = -1;
    timeToRecord = 10000; // Time to record in millis
    
    playMovies = true;

    
}

//--------------------------------------------------------------
void baseApp::update(){

    if(!playMovies){ return; }
    
    topMovie->idleMovie();
    middleMovie->idleMovie();
    bottomMovie->idleMovie();
    
    cam.grabFrame();
    if (cam.isFrameNew()){  
        calculateCaptureFramerate();  
    }
    


    
    if(ofGetElapsedTimeMillis() >= (millisCounter + timeToRecord)){
        millisCounter = -1;
    }
   
}

//--------------------------------------------------------------
void baseApp::draw(){

	ofSetHexColor(0xFFFFFF);
    
    if(!playMovies){ return; }
    
    // First draw the bottom layer (middle one)
    bottomMovie->draw(0,0, ofGetWidth(), ofGetHeight());
    
    //then draw a quad for the top layer using our composite shader to set the alpha
	maskShaderBottom.begin();
    
    glActiveTexture(GL_TEXTURE0_ARB);
	middleMovie->getTextureReference().bind();
    
	glActiveTexture(GL_TEXTURE1_ARB);
	maskBottom.getTextureReference().bind();
    
    //draw a quad the size of the frame
	glBegin(GL_QUADS);
    
    //move the mask around with the mouse by modifying the texture coordinates
	glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, 0);
	glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, 0);				
	glVertex2f( 0, 0);
    
	glMultiTexCoord2d(GL_TEXTURE0_ARB, middleMovie->getWidth(), 0);
	glMultiTexCoord2d(GL_TEXTURE1_ARB, maskBottom.getWidth(), 0);		
	glVertex2f( ofGetWidth(), 0);
    
	glMultiTexCoord2d(GL_TEXTURE0_ARB, middleMovie->getWidth(), middleMovie->getHeight());
	glMultiTexCoord2d(GL_TEXTURE1_ARB, maskBottom.getWidth(), maskBottom.getHeight());
	glVertex2f( ofGetWidth(), ofGetHeight());
    
	glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, middleMovie->getHeight());
	glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, maskBottom.getHeight());		
	glVertex2f( 0, ofGetHeight() );
	
	glEnd();
    
    //deactive and clean up
	glActiveTexture(GL_TEXTURE0_ARB);
	maskBottom.getTextureReference().unbind();
	
	glActiveTexture(GL_TEXTURE1_ARB);
	middleMovie->getTextureReference().unbind();
    
    
    maskShaderBottom.end();
    
    //then draw a quad for the top layer using our composite shader to set the alpha
	maskShaderTop.begin();
    
    glActiveTexture(GL_TEXTURE0_ARB);
	topMovie->getTextureReference().bind();
    
	glActiveTexture(GL_TEXTURE1_ARB);
	maskTop.getTextureReference().bind();
    
    //draw a quad the size of the frame
	glBegin(GL_QUADS);
    
    //move the mask around with the mouse by modifying the texture coordinates
	glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, 0);
	glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, 0);				
	glVertex2f( 0, 0);
    
	glMultiTexCoord2d(GL_TEXTURE0_ARB, topMovie->getWidth(), 0);
	glMultiTexCoord2d(GL_TEXTURE1_ARB, maskTop.getWidth(), 0);		
	glVertex2f( ofGetWidth(), 0);
    
	glMultiTexCoord2d(GL_TEXTURE0_ARB, topMovie->getWidth(), topMovie->getHeight());
	glMultiTexCoord2d(GL_TEXTURE1_ARB, maskTop.getWidth(), maskTop.getHeight());
	glVertex2f( ofGetWidth(), ofGetHeight());
    
	glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, topMovie->getHeight());
	glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, maskTop.getHeight());		
	glVertex2f( 0, ofGetHeight() );
	
	glEnd();
    
    //deactive and clean up
	glActiveTexture(GL_TEXTURE1_ARB);
	maskTop.getTextureReference().unbind();
	
	glActiveTexture(GL_TEXTURE0_ARB);
	topMovie->getTextureReference().unbind();
    
    
    maskShaderTop.end();
    if(millisCounter > 0){
        cam.draw(0,0);
    }

}

//--------------------------------------------------------------
void baseApp::keyPressed  (int key){
    switch(key)
    {
        case 'r':
            millisCounter = ofGetElapsedTimeMillis();
            break;
        case 'n':            

            topMovie->stop();
            topMovie->close();
            middleMovie->stop();
            middleMovie->close();
            bottomMovie->stop();
            bottomMovie->close();
            delete topMovie;
            delete middleMovie;
            delete bottomMovie;
            
            topMovie = new ofVideoPlayer();
            topMovie->loadMovie(files[(int)ofRandom(0, 3)]);
            topMovie->play();
            middleMovie = new ofVideoPlayer();
            middleMovie->loadMovie(files[(int)ofRandom(0, 3)]);
            middleMovie->play();
            bottomMovie = new ofVideoPlayer();
            bottomMovie->loadMovie(files[(int)ofRandom(0, 3)]);
            bottomMovie->play();
            
            break;
        case 'p':            
            random = (int)ofRandom(0, 3);
            printf("Random: %i\n" , random);
            playMovies = true;
            //            topMovie.loadMovie("movies/top.mov");
            //            middleMovie.loadMovie("movies/middle.mov");
            //            bottomMovie.loadMovie("movies/bottom.mov");
            break;
        default:
            break;
            
    }
}

//--------------------------------------------------------------
void baseApp::keyReleased(int key){

}

//--------------------------------------------------------------
void baseApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void baseApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void baseApp::mousePressed(int x, int y, int button){
	
}


//--------------------------------------------------------------
void baseApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void baseApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void baseApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void baseApp::dragEvent(ofDragInfo dragInfo){ 

}

void baseApp::calculateCaptureFramerate()  
{  
    mytimeNow = ofGetElapsedTimef();  
    if( (mytimeNow-mytimeThen) > 0.05f || myframes == 0.f ) {  
        myfps = myframes / (mytimeNow-mytimeThen);  
        mytimeThen = mytimeNow;  
        myframes = 0;  
        myframeRate = 0.9f * myframeRate + 0.1f * myfps;  
    }  
    myframes++;  
}  
