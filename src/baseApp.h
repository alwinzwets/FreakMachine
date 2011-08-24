#pragma once

#include "ofMain.h"

class baseApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);	
	
        void calculateCaptureFramerate();

		ofVideoPlayer* 		topMovie;
   		ofVideoPlayer* 		middleMovie;
        ofVideoPlayer* 		bottomMovie;
        
        ofImage             maskTop;
        ofImage             maskBottom;
        ofShader            maskShaderTop;
        ofShader            maskShaderBottom;
    
        ofVideoGrabber      cam;
    
        bool playMovies;
    
        char buf[255];  
        float mytimeNow, mytimeThen, myframeRate, myfps, myframes;
    
        string files[3];
        
        int                 millisCounter;
        int                 timeToRecord, random;
    
};

