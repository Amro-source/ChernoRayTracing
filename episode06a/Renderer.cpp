#include "Renderer.h"
#include "Camera.h"
#include "Ray.h"
#include "Walnut/Random.h"
#include "math.h"
#include <execution>
#include <glm/common.hpp>

namespace Utils {


	static  uint32_t  ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;

		return result;

	}



}


void Renderer::OnResize(uint32_t width, uint32_t height)
{
	
	if (m_FinalImage)
	{
		//no resize necessary 
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;
		
		m_FinalImage->Resize(width, height);

	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}
	
	
	delete[] m_ImageData;
	m_ImageData = new uint32_t[width *height];



}


void Renderer::Render(const Camera& camera)
{
	Ray ray;

	//const glm::vec3& rayOrigin = camera.GetPosition();
	ray.Origin = camera.GetPosition();



	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		
		for (uint32_t x = 0; x < m_FinalImage->GetWidth() ; x++)
		{
			
			
			
			//glm::vec2 coord = { (float)x /(float) m_FinalImage->GetWidth() ,(float) y /(float) m_FinalImage->GetHeight() };
		

			//coord = coord * 2.0f - 1.0f; //-1 --> 1

			//const glm::vec3& rayDirection = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];
			ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];

			glm::vec4 color = TraceRay(ray);
			
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));

			m_ImageData[x+y* m_FinalImage->GetWidth()]=Utils::ConvertToRGBA(color);



		}

	}


	m_FinalImage->SetData(m_ImageData);



}

//
//I�m glad you got the correct function from the discord server.I can see a few differences between your original function and the
//correct one.Here are some of them :
//
//You normalized the ray direction in your original function, but not in the correct one.This is not necessary, because 
//the ray direction is already a unit vector when you use the pixel coordinates as its x and y components.
//You multiplied the ray origin and direction by each other in your original function, but not in the correct one.This 
//is incorrect, because you need to add them together to get the hit point.Multiplying them will give you a different
//vector that is not on the ray.
//You used the hit point as the sphere color in both functions, but in the correct one you added it to the ray origin
//instead of multiplying it.This is important, because adding the hit point to the ray origin will give you the actual
//position of the hit point in world space, which will affect the color gradient.Multiplying them will give you 
//a different vector that is not related to the position.
//These are some of the errors that I noticed in your original function.I hope this helps you understand what 
//was wrong and how to fix it.

glm::vec4 Renderer::TraceRay(const Ray& ray)
{
	float radius = 0.5f;

	//float a = rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y + rayDirection.z * rayDirection.z;
	float a = glm::dot(ray.Direction, ray.Direction);
	float b = 2.0f * glm::dot(ray.Origin, ray.Direction);
	float c = glm::dot(ray.Origin, ray.Origin) - radius * radius;

	float discriminant = b * b - 4.0f * a * c;
	//If did not hit stop code
	if (discriminant < 0.0f)
		return glm::vec4(0, 0, 0, 1);

	//Did hit, continue
	float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a);
	float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);

	glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));

	//dot(x^,y^)=cos(angle) the dot product between two vectors is basically the cosine of the angle between them
	glm::vec3 hitPoint = ray.Origin + ray.Direction * closestT;
	glm::vec3 normal = glm::normalize(hitPoint);

	float d = glm::max(glm::dot(normal, -lightDir),0.0f);




	
	glm::vec3 sphereColor(1, 0, 1);
	sphereColor = hitPoint;//gradient color for sphere
	sphereColor = normal*0.5f+0.5f;//gradient color for sphere 
	sphereColor *= d;
	//blueish sphere rendered in the scene

	return glm::vec4(sphereColor, 1.0f);

	


}

uint32_t PerPixel(glm::vec2 coord)
{

	return  0xffff00ff ;






}