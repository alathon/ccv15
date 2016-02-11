#include <iostream>
#include <sstream>
#include "FlyCapture2.h"
#include "FlyCapture2Defs.h"
#include "ofxGS3.h"

using namespace FlyCapture2;
using namespace std;

ofxgs3::ofxgs3()
{
}

ofxgs3::~ofxgs3()
{
	deinitializeCamera();
}

void ofxgs3::callSettingsDialog()
{
	// TODO:
	//pgrcamguiInitializeSettingsDialog(cameraContext, guiContext);
}

CAMERA_BASE_FEATURE* ofxgs3::getSupportedFeatures(int* featuresCount)
{
	*featuresCount = 7;
	CAMERA_BASE_FEATURE* features = (CAMERA_BASE_FEATURE*)malloc(*featuresCount * sizeof(CAMERA_BASE_FEATURE));
	features[0] = BASE_BRIGHTNESS;
	features[1] = BASE_EXPOSURE;
	features[2] = BASE_SATURATION;
	features[3] = BASE_GAMMA;
	features[4] = BASE_SHUTTER;
	features[5] = BASE_GAIN;
	features[6] = BASE_FRAMERATE;
	return features;
}

void ofxgs3::setCameraFeature(CAMERA_BASE_FEATURE featureCode, int firstValue, int secondValue, bool isAuto, bool isEnabled)
{
	PropertyType deviceProperty = (PropertyType)0xFFFFFFFF;
	//FlyCaptureProperty deviceProperty = (FlyCaptureProperty)0xFFFFFFFF;
	switch (featureCode)
	{
	case BASE_BRIGHTNESS:
		deviceProperty = BRIGHTNESS;
		break;
	case BASE_EXPOSURE:
		deviceProperty = AUTO_EXPOSURE;
		break;
	case BASE_SATURATION:
		deviceProperty = SATURATION;
		break;
	case BASE_GAMMA:
		deviceProperty = GAMMA;
		break;
	case BASE_SHUTTER:
		deviceProperty = SHUTTER;
		break;
	case BASE_GAIN:
		deviceProperty = GAIN;
		break;
	case BASE_FRAMERATE:
		deviceProperty = FRAME_RATE;
		framerate = firstValue;
		break;
	}
	if (deviceProperty != 0xFFFFFFFF)
	{
		// TODO: Use Camera.setProperty() to set property (See Camera.h)
		// Old code: flycaptureSetCameraPropertyEx(cameraContext, deviceProperty, true, isEnabled, isAuto, firstValue, secondValue);
	}
}

void ofxgs3::getCameraFeature(CAMERA_BASE_FEATURE featureCode, int* firstValue, int* secondValue, bool* isAuto, bool* isEnabled, int* minValue, int* maxValue)
{
	*firstValue = 0;
	*secondValue = 0;
	*isAuto = false;
	*isEnabled = false;
	*minValue = 0;
	*maxValue = 0;
	PropertyType deviceProperty = (PropertyType)0xFFFFFFFF;
	//FlyCaptureProperty deviceProperty = (FlyCaptureProperty)0xFFFFFFFF;
	switch (featureCode)
	{
	case BASE_BRIGHTNESS:
		deviceProperty = BRIGHTNESS;
		break;
	case BASE_EXPOSURE:
		deviceProperty = AUTO_EXPOSURE;
		break;
	case BASE_SATURATION:
		deviceProperty = SATURATION;
		break;
	case BASE_GAMMA:
		deviceProperty = GAMMA;
		break;
	case BASE_SHUTTER:
		deviceProperty = SHUTTER;
		break;
	case BASE_GAIN:
		deviceProperty = GAIN;
		break;
	case BASE_FRAMERATE:
		deviceProperty = FRAME_RATE;
		break;
	}
	bool bpPresent, bpOnePush, pbReadOut, pbOnOff, pbAuto, pbManual;
	if (deviceProperty != 0xFFFFFFFF)
	{
		// TODO: Use Camera.getProperty(). See Camera.h
		//*isEnabled = (flycaptureGetCameraProperty(cameraContext, deviceProperty, (long*)firstValue, (long*)secondValue, isAuto) == FLYCAPTURE_OK);
		//flycaptureGetCameraPropertyRangeEx(cameraContext, deviceProperty, &bpPresent, &bpOnePush, &pbReadOut, &pbOnOff, &pbAuto, &pbManual, minValue, maxValue);
	}
	else
		*isEnabled = false;
}

int ofxgs3::getCameraBaseCount()
{
	unsigned int numCams = 0;
	BusManager busMgr;
	Error error;
	error = busMgr.GetNumOfCameras(&numCams);
	if (error != PGRERROR_OK)
	{
		return -1;
	}
	return numCams;
}

void ofxgs3::getNewFrame(unsigned char* newFrame)
{
	Image rawImage;
	Error error;
	error = rawImage.SetColorProcessing(NEAREST_NEIGHBOR);
	if (error != PGRERROR_OK) {
		error.PrintErrorTrace();
		return;
	}

	error = cam.RetrieveBuffer(&rawImage);
	if (error != PGRERROR_OK)
	{
		cout << "Error grabbing frame with GS3 camera!" << endl;
		error.PrintErrorTrace();
		return;
	}

	memcpy(static_cast<void*>(newFrame), rawImage.GetData(), width * height * depth * sizeof(unsigned char));
	/*flycaptureGrabImage2(cameraContext, &fcImage);
	memcpy((void*)newFrame, fcImage.pData, width * height * depth * sizeof(unsigned char));*/
}

void ofxgs3::setCameraType()
{
	cameraType = GS3;
	cameraTypeName = "GS3";
}

void ofxgs3::cameraInitializationLogic()
{
	Error error;
	BusManager busMgr;
	PGRGuid pgrGuid;
	error = busMgr.GetCameraFromSerialNumber(guid.Data1, &pgrGuid);
	if (error != PGRERROR_OK) {
		error.PrintErrorTrace();
		return;
	}

	error = cam.Connect(&pgrGuid);
	if(error != PGRERROR_OK) {
		error.PrintErrorTrace();
		return;
	}

	Format7Info fmt7Info;
    bool supported;
    fmt7Info.mode = MODE_0;
    error = cam.GetFormat7Info( &fmt7Info, &supported );
    if (error != PGRERROR_OK)
    {
		error.PrintErrorTrace();
        return;
    }

    Format7ImageSettings fmt7ImageSettings;
    fmt7ImageSettings.mode = MODE_0;
    fmt7ImageSettings.offsetX = 0;
    fmt7ImageSettings.offsetY = 0;
    fmt7ImageSettings.width = 896; //fmt7Info.maxWidth;
    fmt7ImageSettings.height = 900; //fmt7Info.maxHeight;
    fmt7ImageSettings.pixelFormat = PIXEL_FORMAT_MONO8;

    bool valid;
    Format7PacketInfo fmt7PacketInfo;

    // Validate the settings to make sure that they are valid
    error = cam.ValidateFormat7Settings(
        &fmt7ImageSettings,
        &valid,
        &fmt7PacketInfo );
    if (error != PGRERROR_OK)
    {
		error.PrintErrorTrace();
        return;
    }

    if ( !valid )
    {
        // Settings are not valid
		cout << "Format7 settings are not valid" << endl; 
        return;
    }

    // Set the settings to the camera
    error = cam.SetFormat7Configuration(
        &fmt7ImageSettings,
		//    fmt7PacketInfo.recommendedBytesPerPacket );
		fmt7PacketInfo.maxBytesPerPacket
    );
    
    if (error != PGRERROR_OK)
    {
		error.PrintErrorTrace();
        return;
    }

	cout << "Connected camera with serial number: " << guid.Data1 << endl;

	error = cam.StartCapture();
    if (error != PGRERROR_OK)
    {
		error.PrintErrorTrace();
		return;
    }

	depth = 1;

	Image capt;
	capt.SetColorProcessing(NEAREST_NEIGHBOR);
	error = cam.RetrieveBuffer(&capt);
	
	// Set width/height.
	PixelFormat pxFormat;
	unsigned int rows, cols, stride;
	capt.GetDimensions(&rows, &cols, &stride, &pxFormat);
	if (error != PGRERROR_OK) {
		error.PrintErrorTrace();
		width = 896;
		height = 900;
	} else {
		width = rows;
		height = cols;
	}
	
	// Retrieve frame rate property
    Property frmRate;
    frmRate.type = FRAME_RATE;
	
    error = cam.GetProperty( &frmRate );
    if (error != PGRERROR_OK)
    {
		error.PrintErrorTrace();
		return;
    }
		
	cout << "Proposed framerate is " << framerate << " fps" << endl;
    cout << "Actual frame rate is " << fixed << setprecision(2) << frmRate.absValue << " fps" << endl; 
	
	
	cout << "Trying to set 100FPS" << endl;

	Property fps2;
	fps2.type = FRAME_RATE;
	fps2.absValue = 100;

	error = cam.SetProperty( &fps2 );
	if (error != PGRERROR_OK)
    {
		error.PrintErrorTrace();
		return;
    }

    error = cam.GetProperty( &frmRate );
    if (error != PGRERROR_OK)
    {
		error.PrintErrorTrace();
		return;
    }

	cout << "Proposed framerate is " << framerate << " fps" << endl;
    cout << "Actual frame rate is " << fixed << setprecision(2) << frmRate.absValue << " fps" << endl; 

	cout << "GS3 Camera set to width:" << width << " height:" << height << endl;

	

	Property exp;
	exp.type = AUTO_EXPOSURE;
	exp.autoManualMode = true;
	exp.onOff = false;
	exp.onePush = false;
	exp.absControl = true;
	//exp.absValue = -0.385;
	cam.SetProperty( & exp);

	exp.type = BRIGHTNESS;
	exp.autoManualMode = false;
	exp.onOff = true;
	exp.onePush = false;
	exp.absControl = true;
	exp.absValue = 0;
	cam.SetProperty( & exp);

	exp.type = GAIN;
	exp.absValue = 0.0;
	exp.autoManualMode = false;
	exp.onOff = true;
	exp.onePush = false;
	exp.absControl = true;
	cam.SetProperty( & exp);

	exp.type = SHUTTER;
	//exp.absValue = 9.904;
	exp.autoManualMode = true;
	exp.onOff = false;
	exp.onePush = false;
	exp.absControl = true;
	cam.SetProperty ( & exp);

	exp.type = SHARPNESS;
	exp.autoManualMode = true;
	exp.onOff = false;
	exp.onePush = false;
	exp.absControl = true;
	//exp.absValue = 797;
	cam.SetProperty ( & exp);

	exp.type = GAMMA;
	exp.autoManualMode = true;
	exp.onOff = true;
	exp.onePush = false;
	exp.absControl = true;
	//exp.absValue = 797;
	cam.SetProperty ( & exp);

	exp.type = FRAME_RATE;
	exp.absValue = 100.0;
	exp.absControl = true;
	exp.autoManualMode = false;
	exp.onePush = false;
	exp.onOff = true;
	cam.SetProperty( & exp);
	error = cam.SetProperty(&exp);


	Property bright;
	bright.type = BRIGHTNESS;
	error = cam.GetProperty( &bright );
	if (error != PGRERROR_OK)
    {
		error.PrintErrorTrace();
		return;
    }

	cout << "Brightness: " << bright.absValue << endl;

	bright.type = AUTO_EXPOSURE;
	error = cam.GetProperty( &bright );
	if (error != PGRERROR_OK)
    {
		error.PrintErrorTrace();
		return;
    }

	cout << "Exposure: " << bright.absValue << endl;

	bright.type = GAMMA;
	error = cam.GetProperty( &bright );
	if (error != PGRERROR_OK)
    {
		error.PrintErrorTrace();
		return;
    }

	cout << "Gamma: " << bright.absValue << endl;

	bright.type = SHUTTER;
	error = cam.GetProperty( &bright );
	if (error != PGRERROR_OK)
    {
		error.PrintErrorTrace();
		return;
    }

	cout << "Shutter: " << bright.absValue << endl;

	bright.type = GAIN;
	error = cam.GetProperty( &bright );
	if (error != PGRERROR_OK)
    {
		error.PrintErrorTrace();
		return;
    }

	cout << "Gain: " << bright.absValue << endl;

	for(int i = 0; i < cameraBaseSettings->propertyType.size(); i++) {
		setCameraFeature(cameraBaseSettings->propertyType[i], cameraBaseSettings->propertyFirstValue[i], cameraBaseSettings->propertySecondValue[i], cameraBaseSettings->isPropertyAuto[i],
			cameraBaseSettings->isPropertyOn[i]);
	}
	// TODO: Set width, height, depth, color processing method, and camera features to base values.

	/*
	FlyCaptureInfoEx* tempArInfo = new FlyCaptureInfoEx[_MAX_CAMERAS_];
	unsigned int camNum = _MAX_CAMERAS_;
	flycaptureBusEnumerateCamerasEx(tempArInfo, &camNum);
	for (int i = 0; i<camNum; i++)
	{
		if ((unsigned long)tempArInfo[i].SerialNumber == guid.Data1)
			index = i;
	}
	arInfo = tempArInfo[index];
	flycaptureCreateContext(&cameraContext);
	flycaptureInitialize(cameraContext, index);
	flycaptureStartCustomImage(cameraContext, (unsigned int)cameraPixelMode, left, top, width, height, 100, depth == 1 ? (arInfo.CameraType == FLYCAPTURE_COLOR ? FLYCAPTURE_RAW8 : FLYCAPTURE_MONO8) : FLYCAPTURE_RGB8);
	flycaptureGrabImage2(cameraContext, &fcImage);
	width = fcImage.iCols;
	height = fcImage.iRows;
	depth = (width*height != 0) ? fcImage.iRowInc / fcImage.iRows : 0;
	flycaptureSetColorProcessingMethod(cameraContext, depth == 1 ? FLYCAPTURE_DISABLE : FLYCAPTURE_NEAREST_NEIGHBOR_FAST);
	for (int i = 0; i<cameraBaseSettings->propertyType.size(); i++)
		setCameraFeature(cameraBaseSettings->propertyType[i], cameraBaseSettings->propertyFirstValue[i], cameraBaseSettings->propertySecondValue[i], cameraBaseSettings->isPropertyAuto[i], cameraBaseSettings->isPropertyOn[i]);
	pgrcamguiCreateContext(&guiContext);
	delete tempArInfo;
	*/
}

void ofxgs3::cameraDeinitializationLogic()
{
	Error error;
	error = cam.StopCapture();
    if (error != PGRERROR_OK)
    {
		error.PrintErrorTrace();
        return;
    }      

    // Disconnect the camera
    error = cam.Disconnect();
    if (error != PGRERROR_OK)
    {
		error.PrintErrorTrace();
        return;
    }

	/*
	flycaptureStop(cameraContext);
	flycaptureDestroyContext(cameraContext);
	pgrcamguiDestroyContext(guiContext);
	*/
}


GUID* ofxgs3::getBaseCameraGuids(int* camCount)
{
	*camCount = 0;
	*camCount = this->getCameraBaseCount();
	GUID* guids = (GUID*)malloc(*camCount * sizeof(GUID));
	Error error;
	BusManager busMgr;
	for (unsigned int i=0; i < *camCount; i++)
    {
        unsigned int pSerialNumber;

		error = busMgr.GetCameraSerialNumberFromIndex(i, &pSerialNumber);
		GUID guid;
		cout << "GS3 Serial Number: " << pSerialNumber << endl;
		guid.Data1 = pSerialNumber;
		guid.Data2 = guid.Data3 = 0;
		memset((void*)guid.Data4, 0, 8 * sizeof(unsigned char));
		guids[i] = guid;
	}

	return guids;
	/*
	unsigned int tcamNum = _MAX_CAMERAS_;
	FlyCaptureInfoEx* tArInfo = new FlyCaptureInfoEx[_MAX_CAMERAS_];
	flycaptureBusEnumerateCamerasEx(tArInfo, &tcamNum);
	*camCount = tcamNum;
	GUID* guids = (GUID*)malloc(tcamNum*sizeof(GUID));
	for (int i = 0; i<tcamNum; i++)
	{
		GUID guid2;
		guid2.Data1 = tArInfo[i].SerialNumber;
		guid2.Data2 = guid2.Data3 = 0;
		memset((void*)guid2.Data4, 0, 8 * sizeof(unsigned char));
		guids[i] = guid2;
	}
	delete tArInfo;
	return guids;
	*/
}