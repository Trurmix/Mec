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
int updateRange = 20;
bool euler = true;
int waterfallIncrementX = -3;
float timePerFrame = 0.033;
float lenght = 0.5;
float d;
int eixX = 14;
int eixY = 18;
int maxMesh = eixX*eixY;

glm::vec3 normal;
glm::vec3 gravity = glm::vec3(0, -9.8, 0);

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
namespace Sphere {
	extern glm::vec3 centro = { 0.f, 1.f, 0.f };
	extern void setupSphere(glm::vec3 pos = centro, float radius = 1.f);
	extern void cleanupSphere();
	extern void updateSphere(glm::vec3 pos, float radius = 1.f);
	extern void drawSphere();

}

struct OurShpere {
	glm::vec3 pos;
	float rad;
};

struct Particle {
	glm::vec3 pos;
	glm::vec3 lastPos;
	glm::vec3 vel;
	glm::vec3 lastVel;
	float force;
};

float *arra = new float[3*maxMesh];
Particle *particlesContainer = new Particle[LilSpheres::maxParticles];
OurShpere *sphere = new OurShpere();

void PhysicsInit() {
	float random = rand() % 10;
	sphere->pos = glm::vec3(1, 0, 0);
	sphere->rad = 1 ;
	//Grid
	for (int i = 0; i < LilSpheres::maxParticles; ++i) {
		//particlesContainer[i].pos = glm::vec3(arra[i * 3], arra[i * 3 + 1], arra[i * 3 + 2]) = glm::vec3(-5 + lenght * (i % 14) , 10 - lenght * (i / 14), 0);
		
		arra[3 * i + 0] = particlesContainer[i].pos.x = -3 + lenght * (i % 14);
		arra[3 * i + 1] = particlesContainer[i].pos.y = 10;
		arra[3 * i + 2] = particlesContainer[i].pos.z = (14*lenght)/2 + lenght - lenght * (i / 14);

		particlesContainer[i].vel = glm::vec3(0.f, 0.f, 0.f);
		
	}

	//sphere->pos = glm::vec3(random, random, random);//glm::vec3(Sphere::centro.x*random, Sphere::centro.y*random, Sphere::centro.z*random);
	//sphere->rad = abs(rand());
	//if(sphere->pos <= )
	srand(time(NULL));
	ClothMesh::updateClothMesh(arra);
	//Sphere::setupSphere(sphere->pos, sphere->rad);
	

	//float random = rand() % 100;
	//Sphere::setupSphere(Sphere::centro*random, abs(rand()));
	//Sphere::updateSphere(Sphere::centro*random, abs(rand()));

}

void PhysicsUpdate(float dt) {

	glm::vec3 temp;
	glm::vec3 vTangencial;
	float coefElasticity = 0.6f;
	float coefFriction = 0.1f;
	bool collision = false;
	float col;
	glm::vec3 initial;

	//euler
	for (int i = 0; i < maxMesh; i++) 
	{
		initial = particlesContainer[i].pos;
		if (i == 0 || i == 13) {
			particlesContainer[i].pos = initial;
			particlesContainer[i].vel = { 0, 0, 0 };
		}
		else {
			particlesContainer[i].pos = particlesContainer[i].pos + particlesContainer[i].vel *dt;
			particlesContainer[i].vel = particlesContainer[i].vel + gravity *dt;
		}


		// Floor Colision
		normal = glm::vec3(0, 1, 0);
		temp = glm::dot(normal, particlesContainer[i].vel) * normal;
		d = 0;
		if ((glm::dot(normal, particlesContainer[i].pos) + d)*((glm::dot(normal, initial) + d)) < 0) {
			particlesContainer[i].pos = particlesContainer[i].pos - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].pos) + d)*normal;
			particlesContainer[i].vel = particlesContainer[i].vel - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].vel))* normal - coefFriction*(particlesContainer[i].vel - temp);
		}


		
		// Top Colision 
		temp = glm::dot(normal, particlesContainer[i].vel) * normal;
		normal = glm::vec3(0, -1, 0);
		d = 10;
		if ((glm::dot(normal, particlesContainer[i].pos) + d)*((glm::dot(normal, initial) + d)) < 0) {
			particlesContainer[i].pos = particlesContainer[i].pos - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].pos) + d)*normal;
			particlesContainer[i].vel = particlesContainer[i].vel - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].vel))* normal - coefFriction*(particlesContainer[i].vel - temp);
		}
		

		// Right Face Colision
			normal = glm::vec3(-1, 0, 0);
			temp = glm::dot(normal, particlesContainer[i].vel) * normal;
			d = 5;
			if ((glm::dot(normal, particlesContainer[i].pos) + d)*((glm::dot(normal, initial) + d)) < 0) {
				particlesContainer[i].pos = particlesContainer[i].pos - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].pos) + d)*normal;
				particlesContainer[i].vel = particlesContainer[i].vel - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].vel))* normal - coefFriction*(particlesContainer[i].vel - temp);
			}
			

		// Left Face Colision
			normal = glm::vec3(1, 0, 0);
			temp = glm::dot(normal, particlesContainer[i].vel) * normal;
			d = 5;
			if ((glm::dot(normal, particlesContainer[i].pos) + d)*((glm::dot(normal, initial) + d)) < 0) {
				particlesContainer[i].pos = particlesContainer[i].pos - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].pos) + d)*normal;
				particlesContainer[i].vel = particlesContainer[i].vel - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].vel))* normal - coefFriction*(particlesContainer[i].vel - temp);

			}


		// Front Face Colision
			normal = glm::vec3(0, 0, -1);
			temp = glm::dot(normal, particlesContainer[i].vel) * normal;
			d = 5;
			if ((glm::dot(normal, particlesContainer[i].pos) + d)*((glm::dot(normal, initial) + d)) < 0) {
				particlesContainer[i].pos = particlesContainer[i].pos - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].pos) + d)*normal;
				particlesContainer[i].vel = particlesContainer[i].vel - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].vel))* normal - coefFriction*(particlesContainer[i].vel - temp);

			}


		// Back Face Colision
			normal = glm::vec3(0, 0, 1);
			temp = glm::dot(normal, particlesContainer[i].vel) * normal;
			d = 5;
			//int deb = -(particlesContainer[i].pos.x*normal.x) - (particlesContainer[i].pos.y*normal.y) - (particlesContainer[i].pos.z*normal.z);
			//std::cout << deb;
			if ((glm::dot(normal, particlesContainer[i].pos) + d)*((glm::dot(normal, initial) + d)) < 0) {
				particlesContainer[i].pos = particlesContainer[i].pos - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].pos) + d)*normal;
				particlesContainer[i].vel = particlesContainer[i].vel - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].vel))* normal - coefFriction*(particlesContainer[i].vel - temp);

			}
		
		//Shpere Colision
			float A = ((particlesContainer[i].pos.x - initial.x) + (particlesContainer[i].pos.y - initial.y) + (particlesContainer[i].pos.z - initial.z));
			float B = 4 * ((particlesContainer[i].pos.x - initial.x) * (initial.x + sphere->pos.x) + (particlesContainer[i].pos.y - initial.y) * (initial.x + sphere->pos.y) + (particlesContainer[i].pos.z - initial.z) * (initial.x + sphere->pos.z));
			float C = 2 * ((initial.x * sphere->pos.x) + (initial.y * sphere->pos.y) + (initial.z * sphere->pos.z)) + (glm::pow(initial.x, 2) + glm::pow(initial.y, 2) + glm::pow(initial.z, 2) + glm::pow(sphere->pos.x, 2) + glm::pow(sphere->pos.y, 2) + glm::pow(sphere->pos.z, 2));
			//equacio 2n grau
			if ((-B + glm::sqrt(glm::pow(B, 2)) - 4*A*C)/(2*A) < 0 || (-B - glm::sqrt(glm::pow(B, 2)) - 4 * A*C) / (2 * A) < 0) {

				normal = sphere->pos / (glm::sqrt(glm::pow(sphere->pos.x, 2) + glm::pow(sphere->pos.y, 2) + glm::pow(sphere->pos.z, 2)));
				//normal = glm::vec3(particlesContainer[i].pos - sphere->pos);
				temp = glm::dot(normal, particlesContainer[i].vel) * normal;
				d = -(particlesContainer[i].pos.x*normal.x) - (particlesContainer[i].pos.y*normal.y) - (particlesContainer[i].pos.z*normal.z);

				particlesContainer[i].pos = particlesContainer[i].pos - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].pos) + d)*normal;
				particlesContainer[i].vel = particlesContainer[i].vel - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].vel))* normal - coefFriction*(particlesContainer[i].vel - temp);
			}
			/*if (glm::sqrt(glm::pow(particlesContainer[i].pos.x - sphere->pos.x, 2) + glm::pow(particlesContainer[i].pos.y - sphere->pos.y, 2) + glm::pow(particlesContainer[i].pos.z - sphere->pos.z, 2)) < sphere->rad)
				*/

		arra[3 * i + 0] = particlesContainer[i].pos.x;
		arra[3 * i + 1] = particlesContainer[i].pos.y;
		arra[3 * i + 2] = particlesContainer[i].pos.z;
	}
	Sphere::updateSphere(sphere->pos, sphere->rad);
	ClothMesh::updateClothMesh(arra);



	/*
	//spring 2-dimensional model

	for (int i = 0; i < eixX; i++)
	{
		for (int j = 0; j < eixY; j++) {
			
		}
	}
	
	*/


	//TODO
	/*//friction values
	temp = glm::dot(normal, particlesContainer[i].vel) * normal;
	vTangencial = particlesContainer[i].vel - temp;

	//elasticity and friction
	particlesContainer[i].pos = particlesContainer[i].pos - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].pos) + d)*normal;
	particlesContainer[i].vel = particlesContainer[i].vel - (1 + coefElasticity) * (glm::dot(normal, particlesContainer[i].vel))* normal - coefFriction*vTangencial;
	*/
}

void PhysicsCleanup() {

	//TODO
	delete[] arra;
	//delete[] particlesContainer;
}