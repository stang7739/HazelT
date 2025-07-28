//
// Created by stang on 25-6-7.
//

#ifndef HAZEL_H
#define HAZEL_H
//For use by Hazel application

#include<Hazel/Core/Application.h>
#include <Hazel/Core/Log.h>
#include <Hazel/Core/Layer.h>
#include <Hazel/Core/Input.h>
#include <Hazel/Events/KeyCode.h>
#include <Hazel/Events/MouseEvent.h>
#include <Hazel/Core/Timestep.h>

#include <Hazel/Renderer/OrthographicCameraController.h>

//---------Renderer-----------
#include<Hazel/Renderer/RendererAPI.h>
#include<Hazel/Renderer/RenderCommand.h>
#include<Hazel/Renderer/Renderer.h>
#include<Hazel/Renderer/Shader.h>
#include<Hazel/Renderer/Texture.h>
#include<Hazel/Renderer/SubTexture2D.h>
#include<Hazel/Renderer/Buffer.h>
#include<Hazel/Renderer/VertexArray.h>
#include<Hazel/Renderer/OrthographicCamera.h>
#include <Hazel/Renderer/Renderer2D.h>


//---------deBug-----------
#include <Hazel/Debug/Instrumentor.h>

//---------ui-----------
#include <Platform/OpenGL/OpenGLShader.h>
#include <imgui.h>

//---------Entity Piont-----------
#include<Hazel/Core/EntityPoint.h>
//--------------------------------

#endif //HAZEL_H
