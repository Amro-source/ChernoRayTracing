#include "Renderer.h"

#include "Walnut/Random.h"

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


void Renderer::Render()
{


	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		
		for (uint32_t x = 0; x < m_FinalImage->GetWidth() ; x++)
		{
			
			
			
			glm::vec2 coord = { (float)x /(float) m_FinalImage->GetWidth() ,(float) y /(float) m_FinalImage->GetHeight() };

			coord = coord * 2.0f - 1.0f; //-1 --> 1

			glm::vec4 color = PerPixel(coord);
			
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));

			m_ImageData[x+y* m_FinalImage->GetWidth()]=Utils::ConvertToRGBA(color);



		}

	}


	m_FinalImage->SetData(m_ImageData);



}

glm::vec4 Renderer::PerPixel(glm::vec2 coord)
{
	uint8_t r = (uint8_t)(coord.x * 255.0f);
	uint8_t g = (uint8_t)(coord.y * 255.0f);

	
	//(bx^2+by^2+bz^2) t^2 + (2axbx+2ayby) t + ( ax^2 + ay^2 -r^2)=0
	



	glm::vec3 rayOrigin(0.0f,0.0f,-2.0f);
	glm::vec3 rayDirection(coord.x, coord.y, -1.0f);

	float radius = 0.5f;

	//rayDirection = glm::normalize(rayDirection);
	
	//a ray origin 
	//b ray direction

	// r radius of sphere or circle 

	// t hit distance 

	//float a = rayDirection.x * coord.x + rayDirection.y * coord.y + rayDirection.z * rayDirection.z;
	
	float a = glm::dot(rayDirection, rayDirection);

	float b = 2.0f * glm::dot(rayOrigin, rayDirection);

	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	//quadratic equation b^2-4ac

	float discriminant = b * b - 4.0f * a * c;
	
	if (discriminant >= 0.0f)
		return glm::vec4(1,0,0,1);


	return glm::vec4(0,0,0,1); //returns 0xff000000

	//return 0xff000000 | (g<<8) | r;
	
	//return 0xffff00ff;




}

uint32_t PerPixel(glm::vec2 coord)
{

	return  0xffff00ff ;






}