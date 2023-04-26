#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include "Walnut/Random.h"
#include <ctime>
#include <chrono>
#include <random>
#include <iostream>

using namespace Walnut;


class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render %.3fms*", m_LastRenderTime);


		if (ImGui::Button("Render"))
		{

			Render();

		}
		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");
		mViewportHeight = ImGui::GetContentRegionAvail().y;

		mViewportWidth = ImGui::GetContentRegionAvail().x;



		if(m_image)
			ImGui::Image(m_image->GetDescriptorSet(),{ (float)m_image->GetWidth(),(float) m_image->GetHeight()});
		
		
		ImGui::End();
		ImGui::PopStyleVar();
		

	}

	
	void Render()
	{
		Timer timer;

		std::random_device rd;

		// Create a random number generator
		std::mt19937 gen(rd());

		// Create a uniform distribution over the range [0, 100]
		std::uniform_int_distribution<uint32_t> dis(0, 100);


		std::random_device dev;

		std::mt19937 rng(dev());
		// Create a uniform distribution object
		std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 255);


		if (!m_image || mViewportWidth!= m_image->GetWidth() || mViewportHeight!= m_image->GetHeight())
		{
			m_image = std::make_shared<Image>(mViewportWidth,mViewportHeight, ImageFormat::RGBA);

			delete[] m_ImageData;
			m_ImageData = new uint32_t[mViewportWidth * mViewportHeight ];
		}

		for (uint32_t i = 0; i < mViewportWidth * mViewportHeight; i++)
		{

//			m_ImageData[i] = 0xffffffff;
		//	m_ImageData[i] = dis(gen);

//			m_ImageData[i] =dist6(rng);
			m_ImageData[i] = Random::UInt();
			m_ImageData[i] |= 0xff000000;


		}

		m_image->SetData(m_ImageData);

		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	std::shared_ptr<Image> m_image;
	uint32_t mViewportWidth=0, mViewportHeight=0;

	uint32_t* m_ImageData = nullptr;

	float m_LastRenderTime=0.0f;

};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing is Amazing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}