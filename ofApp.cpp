#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(255);
	ofSetColor(0);

	ofEnableDepthTest();
	ofSetLineWidth(2);

	this->mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->mesh.clear();

	auto noise_param = glm::vec3(ofRandom(360), ofRandom(360), ofRandom(360));

	for (int radius = 50; radius <= 300; radius += 50) {

		auto rotation = glm::vec3(
			ofMap(ofNoise(noise_param.x, radius * 0.0065 - ofGetFrameNum() * 0.02), 0, 1, -90, 90),
			ofMap(ofNoise(noise_param.y, radius * 0.0065 - ofGetFrameNum() * 0.02), 0, 1, -90, 90),
			ofMap(ofNoise(noise_param.z, radius * 0.0065 - ofGetFrameNum() * 0.02), 0, 1, -90, 90));

		int start_index = this->mesh.getNumVertices();
		int index = start_index;
		for (int deg = 0; deg < 360; deg += 1) {

			auto vertex = glm::vec3(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD), 0);

			auto rotation_x = glm::rotate(glm::mat4(), rotation.x * (float)DEG_TO_RAD, glm::vec3(1, 0, 0));
			auto rotation_y = glm::rotate(glm::mat4(), rotation.y * (float)DEG_TO_RAD, glm::vec3(0, 1, 0));
			auto rotation_z = glm::rotate(glm::mat4(), rotation.z * (float)DEG_TO_RAD, glm::vec3(0, 0, 1));

			if (radius % 100 != 0) {

				vertex = glm::vec4(vertex, 0) * rotation_y * rotation_x;
			}

			this->mesh.addVertex(vertex);

			if (deg > 0) {

				this->mesh.addIndex(index - 1);
				this->mesh.addIndex(index);
			}

			index++;
		}

		this->mesh.addIndex(this->mesh.getNumVertices() - 1);
		this->mesh.addIndex(start_index);
	}

	int index_span = 30;
	for (int index = 0; index < this->mesh.getNumVertices() - 360; index += index_span) {

		this->mesh.addIndex(index);
		this->mesh.addIndex(index + 360);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(-70);

	this->mesh.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}