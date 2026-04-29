#include "TrashTheCacheComponent.hpp"

#include <chrono>
#include <vector>

#include <imgui.h>
#include <imgui_plot.h>

#include "Components/ImguiComponent.h"


using namespace Game;


namespace Game
{
    struct Transform
    {
        float matrix[16] =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        };
    };

    class GameObject3D
    {
    public:
        Transform transform{};
        int ID{ 1 };
    };


    class GameObject3DAlt
    {
    public:
        int ID{ 1 };
        Transform* transform{};
    };
}


TrashTheCacheComponent::TrashTheCacheComponent(bae::GameObject& owner, const bool bIsExercise2) :
    ImguiComponent(owner),
    m_bIsExercise2{ bIsExercise2 },
    m_NrSamples{ m_bIsExercise2 ? 10 : 100 }
{
}


void TrashTheCacheComponent::RenderGUI()
{
    if(m_bIsExercise2)
    {
        ImGui::Begin("Exercise 1");
        ImGui::InputInt("samples", &m_NrSamples, 10);

        if(ImGui::Button("Trash the cache"))
        {
            TimingExercise1();
        }

        if(m_NrSamples < 1)
        {
            m_NrSamples = 10;
        }

        if(!m_TimingsExercise1.empty())
        {
            ImGui::PlotConfig conf;
            conf.values.ys      = m_TimingsExercise1.data();
            conf.values.count   = static_cast<int>(m_TimingsExercise1.size());
            conf.scale.min      = 0;
            conf.scale.max      = static_cast<float>(*std::ranges::max_element(m_TimingsExercise1));
            conf.tooltip.show   = true;
            conf.tooltip.format = "x=%.2f, y=%.2f";
            conf.grid_x.show    = true;
            conf.grid_y.show    = false;
            conf.frame_size     = ImVec2(200, 100);
            conf.line_thickness = 2.f;
            conf.values.color   = ImColor(181, 95, 34); // orange

            ImGui::Plot("Plot 1", conf);
        }

        ImGui::End();
    }
    else
    {
        ImGui::Begin("Exercise 3");
        ImGui::InputInt("samples", &m_NrSamples, 100);

        if(ImGui::Button("Trash the cache with GameObject3D"))
        {
            TimingExercise2();
        }

        if(ImGui::Button("Trash the cache with GameObject3DAlt"))
        {
            TimingExercise2Alt();
        }

        if(m_NrSamples < 1)
        {
            m_NrSamples = 100;
        }

        if(!m_TimingsExercise2.empty())
        {
            ImGui::PlotConfig conf;
            conf.values.ys      = m_TimingsExercise2.data();
            conf.values.count   = static_cast<int>(m_TimingsExercise2.size());
            conf.scale.min      = 0;
            conf.scale.max      = *std::ranges::max_element(m_TimingsExercise2);
            conf.tooltip.show   = true;
            conf.tooltip.format = "x=%.2f, y=%.2f";
            conf.grid_x.show    = false;
            conf.grid_y.show    = true;
            conf.frame_size     = ImVec2(200, 100);
            conf.line_thickness = 2.f;
            conf.values.color   = ImColor(87, 169, 54); // green
            //conf.values.color = ImColor(67, 135, 144); // blue

            ImGui::Plot("plot 2", conf);
        }

        if(!m_TimingsExercise2Alt.empty())
        {
            ImGui::PlotConfig conf;
            conf.values.ys      = m_TimingsExercise2Alt.data();
            conf.values.count   = static_cast<int>(m_TimingsExercise2Alt.size());
            conf.scale.min      = 0;
            conf.scale.max      = *std::ranges::max_element(m_TimingsExercise2Alt);
            conf.tooltip.show   = true;
            conf.tooltip.format = "x=%.2f, y=%.2f";
            conf.grid_x.show    = false;
            conf.grid_y.show    = true;
            conf.frame_size     = ImVec2(200, 100);
            conf.line_thickness = 2.f;
            //conf.values.color = ImColor(87, 169, 54); // green
            conf.values.color = ImColor(67, 135, 144); // blue

            ImGui::Plot("plot 2Alt", conf);
        }

        if(!m_TimingsExercise2.empty() && !m_TimingsExercise2Alt.empty())
        {
            ImGui::Text("Combined:");
            const float max2    = *std::ranges::max_element(m_TimingsExercise2);
            const float max2Alt = *std::ranges::max_element(m_TimingsExercise2Alt);
            const float max     = std::max(max2, max2Alt);

            const float* dataBoth[] = { m_TimingsExercise2.data(), m_TimingsExercise2Alt.data() };

            ImGui::PlotConfig conf;
            //conf.values.ys
            conf.values.ys_list  = dataBoth;
            conf.values.count    = static_cast<int>(m_TimingsExercise2.size());
            conf.values.ys_count = 2;
            conf.scale.min       = 0;
            conf.scale.max       = max;
            conf.tooltip.show    = true;
            conf.tooltip.format  = "x=%.2f, y=%.2f";
            conf.grid_x.show     = true;
            conf.grid_y.show     = false;
            conf.frame_size      = ImVec2(200, 100);
            conf.line_thickness  = 2.f;
            //conf.values.color = ImColor(87, 169, 54); // green
            //conf.values.color = ImColor(67, 135, 144); // blue

            ImGui::Plot("Plot 2 Combined", conf);
        }


        ImGui::End();
    }
}


void TrashTheCacheComponent::TimingExercise1()
{
    m_TimingsExercise1.clear();

    std::vector timings(m_NrSamples + 1, 0.f);
    std::vector buffer(m_BufferSize, 1);

    for(int sampleIndex{}; sampleIndex < m_NrSamples; ++sampleIndex)
    {
        int timingIndex{ 0 };

        for(int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
        {
            const auto beginIteration{ std::chrono::high_resolution_clock::now() };

            for(int i{ 0 }; i < buffer.size(); i += stepsize)
            {
                buffer[i] *= 2;
            }

            const auto timeElapsed{
                std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::high_resolution_clock::now() - beginIteration)
            };
            timings[timingIndex] += timeElapsed.count();

            ++timingIndex;
        }
    }

    // average out all the timings
    for(auto& timing : timings)
    {
        timing /= static_cast<float>(m_NrSamples);
    }

    m_TimingsExercise1 = std::move(timings);
}

void TrashTheCacheComponent::TimingExercise2()
{
    m_TimingsExercise2.clear();

    std::vector timings(m_NrSamples + 1, 0.f);
    std::vector<GameObject3D> buffer(static_cast<size_t>(m_BufferSize));

    for(int sampleIndex{}; sampleIndex < m_NrSamples; ++sampleIndex)
    {
        int timingIndex{ 0 };

        for(int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
        {
            const auto beginIteration{ std::chrono::high_resolution_clock::now() };

            for(int i{ 0 }; i < buffer.size(); i += stepsize)
            {
                buffer[i].ID *= 2;
            }

            const auto timeElapsed = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - beginIteration);

            timings[timingIndex] += timeElapsed.count();

            ++timingIndex;
        }
    }

    // average out all the timings
    for(auto& timing : timings)
        timing /= static_cast<float>(m_NrSamples);

    m_TimingsExercise2 = std::move(timings);
}


void TrashTheCacheComponent::TimingExercise2Alt()
{
    m_TimingsExercise2Alt.clear();

    std::vector timings(m_NrSamples + 1, 0.f);
    std::vector<GameObject3DAlt> buffer{ static_cast<size_t>(m_BufferSize) };

    for(int sampleIndex{}; sampleIndex < m_NrSamples; ++sampleIndex)
    {
        int timingIndex{ 0 };

        for(int stepsize{ 1 }; stepsize <= 1024; stepsize *= 2)
        {
            const auto beginIteration{ std::chrono::high_resolution_clock::now() };

            for(int i{ 0 }; i < buffer.size(); i += stepsize)
            {
                buffer[i].ID *= 2;
            }

            const auto timeElapsed{
                std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::high_resolution_clock::now() - beginIteration)
            };
            timings[timingIndex] += timeElapsed.count();

            ++timingIndex;
        }
    }

    // average out all the timings
    for(auto& timing : timings)
    {
        timing /= static_cast<float>(m_NrSamples);
    }

    m_TimingsExercise2Alt = std::move(timings);
}


