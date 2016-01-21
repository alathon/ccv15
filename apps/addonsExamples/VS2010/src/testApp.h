#ifndef _TEST_APP
#define _TEST_APP

//if standalone mode/non-addon
#define STANDALONE

//main
#include "ofMain.h"
//addon
#include "ofxNCore.h"

class testApp : public ofBaseApp, public TouchListener
{
public:
	testApp(int argc, char *argv[]) 
	{
		debug = false;
		TouchEvents.addListener(this);
		for(int i = 1; i < argc; i++) {
			if(strcmp(argv[i],"-d") == 0) {
				printf("Debug = True.");
				debug = true;
			} else if(strcmp(argv[i],"-xmlPath") == 0) {
				if(argc >= i+1) {
					xmlPrefix = argv[i+1];
					printf("XML Prefix set to %s", xmlPrefix);
					// TODO: Respect xmlPath in all XML paths.
				}
			}
		}
		/*
		if(argc >= 2)
		{
			printf("Command Line Option Passed : %s\n",argv[1]);
			if(strcmp(argv[1],"-d")==0)
			{
				debug = true;
			}
		}*/
	}
	ofxNCoreVision * ccv;
	bool debug;
	char* xmlPrefix;

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();

	//Touch Events
	void TouchDown(Blob b);
	void TouchMoved(Blob b);
	void TouchUp(Blob b);
};

#endif

