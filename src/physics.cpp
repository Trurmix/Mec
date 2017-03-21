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

bool show_test_window = false;
bool euler = true;
int waterfallIncrementX = -3;
float timePerFrame = 0.033;
float lenght = 0.5;
glm::vec3 gravity = glm::vec3(0, -9.8, 0);
int maxMesh = 252;
glm::vec3 normal;

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

float *arra = new float[3*maxMesh];
Particle *particlesContainer = new Particle[LilSpheres::maxParticles];

void PhysicsInit() {

	//Grid
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		//particlesContainer[i].pos = glm::vec3(arra[i * 3], arra[i * 3 + 1], arra[i * 3 + 2]) = glm::vec3(-5 + lenght * (i % 14) , 10 - lenght * (i / 14), 0);
		
		arra[3 * i + 0] = particlesContainer[i].pos.x = -5 + lenght * (i % 14);
		arra[3 * i + 1] = particlesContainer[i].pos.y = 10 - lenght * (i / 14);
		arra[3 * i + 2] = particlesContainer[i].pos.z = 0;

		particlesContainer[i].vel = glm::vec3(0.f, 0.f, 0.f);
		
	}
	ClothMesh::updateClothMesh(arra);

	srand(time(NULL));

	

}

void PhysicsUpdate(float dt) {

	glm::vec3 temp;

	//euler
	for (int i = 0; i < maxMesh; i++) 
	{
		particlesContainer[i].pos = particlesContainer[i].pos + particlesContainer[i].vel *dt;
		particlesContainer[i].vel = particlesContainer[i].vel + gravity *dt;

		arra[3 * i + 0] = particlesContainer[i].pos.x;
		arra[3 * i + 1] = particlesContainer[i].pos.y;
		arra[3 * i + 2] = particlesContainer[i].pos.z;
	}

	ClothMesh::updateClothMesh(arra);
	//colisions

	//pla sostre
		
	//pla terra
	normal = glm::vec3(0, 1, 0);
	for (int i = 0; i < maxMesh; i++)
	{
		//if(particlesContainer[i].pos.y <= 0)
		if (particlesContainer[i].pos.y <= 0) {
			
		}
		
	}
	//pla lateral esquerra
	
	//pla lateral dret

	//spring 1-dimensional model
	//TODO
}
void PhysicsCleanup() {
	//TODO
	delete[] arra;
}