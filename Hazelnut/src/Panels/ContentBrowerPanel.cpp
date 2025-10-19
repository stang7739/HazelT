//
// Created by stang on 25-10-5.
//
#include "hzpch.h"
#include "ContentBrowerPanel.h"

#include <imgui.h>

#include "Hazel/Renderer/Texture.h"

namespace Hazel
{
    extern const std::filesystem::path g_AssetPath = "assets";
    ContentBrowerPanel::ContentBrowerPanel():m_CurrentDirectory(g_AssetPath)
    {
        m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
        m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
    }

    void ContentBrowerPanel::OnImGuiRender()
    {
        ImGui::Begin("Content Brower");
        if(m_CurrentDirectory != std::filesystem::path(g_AssetPath))
        {
            if(ImGui::Button("<-"))
            {
                m_CurrentDirectory = m_CurrentDirectory.parent_path();
            }
        }
        static float padding = 16.0f;
        static float thumbnailSize = 64.0f;
        float cellSize = thumbnailSize + padding;

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if(columnCount < 1)
            columnCount = 1;
        ImGui::Columns(columnCount,nullptr,false);
        int _itemID = 0;
        for(auto& directoryEntity: std::filesystem::directory_iterator(m_CurrentDirectory))
        {
            const auto& path = directoryEntity.path();
            auto relativePath = std::filesystem::relative(path,g_AssetPath);
            std::string filenamestring = relativePath.filename().string();
            // ImGui::PushID(filenamestring.c_str());
            Ref<Texture2D> icon = directoryEntity.is_directory() ? m_DirectoryIcon : m_FileIcon;
            // cpp
            ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0,0,0,0));
            std::string id = std::string("##icon_") + std::to_string(_itemID++);
            ImGui::ImageButton(id.c_str(),
                    (ImTextureRef)(ImTextureID)(intptr_t)icon->GetRendererID(),
                    ImVec2(thumbnailSize, thumbnailSize),
                    ImVec2(0.0f, 1.0f),
                    ImVec2(1.0f, 0.0f));
            if(ImGui::BeginDragDropSource())
            {
                const wchar_t* itemPath = relativePath.c_str();
                ImGui::SetDragDropPayload("CONNECTOR_DROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
                ImGui::Text(relativePath.filename().string().c_str());
                ImGui::EndDragDropSource();
            }
            ImGui::PopStyleColor();
            if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                if(directoryEntity.is_directory())
                {
                    m_CurrentDirectory /= path.filename();
                }
            }
            ImGui::TextWrapped(filenamestring.c_str());
            ImGui::NextColumn();
            // ImGui::PopID(); //给每一个控件一个唯一的标签
        }
        ImGui::Columns(1);
        ImGui::SliderFloat("thumbnail size",&thumbnailSize,16,512);
        ImGui::SliderFloat("Padding",&padding,0,32);
        ImGui::End();
    }


}
