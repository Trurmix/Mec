#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <cstdio>
#include "GL_framework.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

float *partVerts;
bool show_test_window = false;
bool euler = true;
int waterfallIncrementX = -3;
float timePerFrame = 0.033;

void GUI() {
	{	//FrameRate
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//TODO
	}

	// ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if(show_test_window) {
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}
}

namespace LilSpheres {
	extern const int maxParticles;
	extern void setupParticles(int numTotalParticles, float radius = 0.05f);
	extern void cleanupParticles();
	extern void updateParticles(int startIdx, int count, float* array_data);
	extern void drawParticles(int startIdx, int count);
}
namespace ClothMesh {
	extern void setupClothMesh();
	extern void cleanupClothMesh();
	extern void updateClothMesh(float* array_data);
	extern void drawClothMesh();
}

struct Particle {
	glm::vec3 pos;
	glm::vec3 lastPos;
	glm::vec3 vel;
	glm::vec3 lastVel;
};

float *arra = new float[3*252];
Particle *particlesContainer = new Particle[LilSpheres::maxParticles];

void PhysicsInit() {

	for (int i = 0; i < LilSpheres::maxParticles; ++i) {

		arra[3 * i + 0] = particlesContainer[i].pos.x = -5 + 0.5 * (i % 14);
		arra[3 * i + 1] = particlesContainer[i].pos.y = 10 - 0.5 * (i / 14);
		arra[3 * i + 2] = particlesContainer[i].pos.z = 0;
	}

	ClothMesh::updateClothMesh(arra);
}

void PhysicsUpdate(float dt) {
	//TODO
}
void PhysicsCleanup() {
	//TODO
	delete[] partVerts;
}