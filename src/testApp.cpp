#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
  loadFrames("test04");

  drawPixels = new unsigned char[frameWidth * frameHeight * 4];

/*
  RecordedTickReader* recordedTickReader = new RecordedTickReader();
  recordedTickReader->setup("longsteady.csv", 2);
  tickReader = recordedTickReader;
/*/
  serial.listDevices();
  serial.setup(0, 9600);

  SerialTickReader* serialTickReader = new SerialTickReader();
  serialTickReader->setup(serial, 2);
  tickReader = serialTickReader;
//*/

  tickInterpreter.setup(tickReader, 2);

  ofxSparkline::Settings settings;
  settings.annotations.label.display = true;
  settings.annotations.label.text = "{V} Raw 0.";
  settings.annotations.label.precision = 2;
  settings.axes.y.range.min = 0;
  settings.axes.y.range.max = 500;
  settings.styles.curve.area.display = true;
  rawSpark0 = ofxSparkline(settings, 200);

  settings = ofxSparkline::Settings();
  settings.annotations.label.display = true;
  settings.annotations.label.text = "{V} Raw 1.";
  settings.annotations.label.precision = 2;
  settings.axes.y.range.min = 0;
  settings.axes.y.range.max = 500;
  settings.styles.curve.area.display = true;
  rawSpark1 = ofxSparkline(settings, 200);

  settings = ofxSparkline::Settings();
  settings.annotations.label.display = true;
  settings.annotations.label.text = "{V} Average 0.";
  settings.annotations.label.precision = 2;
  settings.axes.y.range.min = 0;
  settings.axes.y.range.max = 500;
  settings.styles.curve.area.display = true;
  averageSpark0 = ofxSparkline(settings, 200);

  settings = ofxSparkline::Settings();
  settings.annotations.label.display = true;
  settings.annotations.label.text = "{V} Average 1.";
  settings.annotations.label.precision = 2;
  settings.axes.y.range.min = 0;
  settings.axes.y.range.max = 500;
  settings.styles.curve.area.display = true;
  averageSpark1 = ofxSparkline(settings, 200);

  settings = ofxSparkline::Settings();
  settings.annotations.label.display = true;
  settings.annotations.label.text = "{V} Average difference.";
  settings.annotations.label.precision = 2;
  settings.axes.y.range.min = -100;
  settings.axes.y.range.max = 100;
  settings.styles.curve.area.display = true;
  averageDeltaSpark = ofxSparkline(settings, 200);

  settings = ofxSparkline::Settings();
  settings.annotations.label.display = true;
  settings.annotations.label.text = "{V} Raw velocity 0.";
  settings.annotations.label.precision = 2;
  settings.axes.y.range.min = -100;
  settings.axes.y.range.max = 100;
  settings.styles.curve.area.display = true;
  rawVelocitySpark0 = ofxSparkline(settings, 200);

  settings = ofxSparkline::Settings();
  settings.annotations.label.display = true;
  settings.annotations.label.text = "{V} Raw velocity 1.";
  settings.annotations.label.precision = 2;
  settings.axes.y.range.min = -100;
  settings.axes.y.range.max = 100;
  settings.styles.curve.area.display = true;
  rawVelocitySpark1 = ofxSparkline(settings, 200);

  settings = ofxSparkline::Settings();
  settings.annotations.label.display = true;
  settings.annotations.label.text = "{V} Average velocity 0.";
  settings.annotations.label.precision = 2;
  settings.axes.y.range.min = -100;
  settings.axes.y.range.max = 100;
  settings.styles.curve.area.display = true;
  averageVelocitySpark0 = ofxSparkline(settings, 200);

  settings = ofxSparkline::Settings();
  settings.annotations.label.display = true;
  settings.annotations.label.text = "{V} Average velocity 1.";
  settings.annotations.label.precision = 2;
  settings.axes.y.range.min = -100;
  settings.axes.y.range.max = 100;
  settings.styles.curve.area.display = true;
  averageVelocitySpark1 = ofxSparkline(settings, 200);
}

//--------------------------------------------------------------
void testApp::update(){
  float leftValue = (float)mouseY / frameHeight;
  float rightValue = (float)mouseX / frameWidth;

  for (int w = 0; w < frameWidth; w++) {
    for (int h = 0; h < frameHeight; h++) {
      int frameIndex = ofClamp(ofMap(ofMap(w, 0, frameWidth, leftValue, rightValue), 0, 1, 0, frameCount), 0, frameCount - 1);
      for (int c = 0; c < 4; c++) {
        drawPixels[frameWidth * h * 4 + w * 4 + c] = inputPixels[frameIndex * frameWidth * frameHeight * 4 + frameWidth * h * 4 + w * 4 + c];
      }
    }
  }

  tickInterpreter.update();
  rawSpark0.push_back(tickInterpreter.getRaw(0));
  rawSpark1.push_back(tickInterpreter.getRaw(1));
  averageSpark0.push_back(tickInterpreter.getAverage(0));
  averageSpark1.push_back(tickInterpreter.getAverage(1));
  averageDeltaSpark.push_back(tickInterpreter.getAverage(1) - tickInterpreter.getAverage(0));
  rawVelocitySpark0.push_back(tickInterpreter.getRawVelocity(0));
  rawVelocitySpark1.push_back(tickInterpreter.getRawVelocity(1));
  averageVelocitySpark0.push_back(tickInterpreter.getAverageVelocity(0));
  averageVelocitySpark1.push_back(tickInterpreter.getAverageVelocity(1));
}

//--------------------------------------------------------------
void testApp::draw(){
  ofBackground(255, 128, 0);
  drawImage.setFromPixels(drawPixels, frameWidth, frameHeight, OF_IMAGE_COLOR_ALPHA);
  drawImage.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());

  int currY = 5;
  ofPushMatrix();
  ofScale(3, 3);

  rawSpark0.draw(5, currY);
  currY += rawSpark0.getHeight() + 10;

  rawSpark1.draw(5, currY);
  currY += rawSpark1.getHeight() + 10;

  averageSpark0.draw(5, currY);
  currY += averageSpark0.getHeight() + 10;

  averageSpark1.draw(5, currY);
  currY += averageSpark1.getHeight() + 10;

  averageDeltaSpark.draw(5, currY);
  currY += averageDeltaSpark.getHeight() + 10;

  rawVelocitySpark0.draw(5, currY);
  currY += rawVelocitySpark0.getHeight() + 10;

  rawVelocitySpark1.draw(5, currY);
  currY += rawVelocitySpark1.getHeight() + 10;

  averageVelocitySpark0.draw(5, currY);
  currY += averageVelocitySpark0.getHeight() + 10;

  averageVelocitySpark1.draw(5, currY);
  currY += averageVelocitySpark1.getHeight() + 10;

  ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

void testApp::loadFrames(string folder) {
  frameCount = 0;
  frameWidth = 0;
  frameHeight = 0;

  ofFile file;
  char indexString[5];
  int startIndex = 200;

  sprintf(indexString, "%04d", frameCount + startIndex);
  while (file.doesFileExist(folder + "/frame" + indexString + ".bmp")) {
    frameCount++;
    sprintf(indexString, "%04d", frameCount + startIndex);
  }

  if (frameCount <= 0) return;

  // Determine image size from the first frame.
  ofImage image;
  sprintf(indexString, "%04d", startIndex);
  image.loadImage(folder + "/frame" + indexString + ".bmp");
  frameWidth = image.width;
  frameHeight = image.height;

  if (frameWidth <= 0 || frameHeight <= 0) return;

  /// DEBUG:
  frameCount = 1;

  //cout << "Loading " << frameCount << " frames at " << frameWidth << "x" << frameHeight << "... ";

  if (inputPixels) delete[] inputPixels;
  inputPixels = new unsigned char[frameCount * frameWidth * frameHeight * 4];

  unsigned char* copyPixels;
  for (int frameIndex = 0; frameIndex < frameCount; frameIndex++) {
    sprintf(indexString, "%04d", frameIndex + startIndex);
    image.loadImage(folder + "/frame" + indexString + ".bmp");

    copyPixels = image.getPixels();
    for (int i = 0; i < frameWidth * frameHeight; i++) {
      inputPixels[frameIndex * frameWidth * frameHeight * 4 + i * 4 + 0] = copyPixels[i * 3 + 0];
      inputPixels[frameIndex * frameWidth * frameHeight * 4 + i * 4 + 1] = copyPixels[i * 3 + 1];
      inputPixels[frameIndex * frameWidth * frameHeight * 4 + i * 4 + 2] = copyPixels[i * 3 + 2];
      inputPixels[frameIndex * frameWidth * frameHeight * 4 + i * 4 + 3] = 255;
    }
  }

  //cout << "done." << endl;
}
