#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include "Walnut/Random.h"
#include <ctime>
#include <chrono>
#include <random>
#include <iostream>
#include <memory>
#include "Renderer.h"
#include "Camera.h"

using namespace Walnut;


class ExampleLayer : public Walnut::Layer
{

public:
	ExampleLayer()
		: m_Camera(45.0f, 0.1f, 100.0f) {}

	

	virtual void OnUpdate(float ts) override
	{


		m_Camera.OnUpdate(ts);


	}

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

		auto  image = m_Renderer.GetFinalImage();



		if (m_Renderer.GetFinalImage())
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(),(float)image->GetHeight() },ImVec2(0,1),ImVec2(1,0));


		ImGui::End();
		ImGui::PopStyleVar();


	}


	void Render()
	{
		Timer timer;

		//renderer resize 
		// 
		// 

		
		
		m_Renderer.OnResize(mViewportWidth, mViewportHeight);
		m_Camera.OnResize(mViewportWidth, mViewportHeight);
		m_Renderer.Render(m_Camera);
		
		//renderer render


	
		
		m_LastRenderTime = timer.ElapsedMillis();
	}



   private:
	std::shared_ptr<Image> m_image;
	
	Renderer m_Renderer;


	Camera m_Camera;
	
	uint32_t mViewportWidth = 0, mViewportHeight = 0;

	uint32_t* m_ImageData = nullptr;

	float m_LastRenderTime = 0.0f;

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