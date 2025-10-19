//
// Created by stang on 25-10-5.
//

#ifndef CONTENTBROWERPANEL_H
#define CONTENTBROWERPANEL_H
#include <filesystem>

#include "Hazel/Core/Base.h"


namespace Hazel
{
    class Texture2D;

    class ContentBrowerPanel {
    public:
        ContentBrowerPanel();
        void OnImGuiRender();
    private:
        std::filesystem::path m_CurrentDirectory;
        Ref<Texture2D> m_DirectoryIcon;
        Ref<Texture2D> m_FileIcon;
    };
}


#endif //CONTENTBROWERPANEL_H
