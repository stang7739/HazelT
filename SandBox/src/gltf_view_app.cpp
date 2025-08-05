//
// GLTF View Application - Texture Creation Demo
// Using existing Hazel engine functions without creating new classes
//

#include "Hazel_noEntityPoint.h"

// Simple function to demonstrate texture creation using existing Hazel functions
void CreateAndDisplayTexture()
{
    // Create texture from file using existing Texture2D::Create method
    // This uses the existing texture creation functionality without creating new classes
    Hazel::Ref<Hazel::Texture2D> gltfTexture = Hazel::Texture2D::Create("assets/textures/Ship.png");
    
    // Create a procedural texture using existing Create method with dimensions
    Hazel::Ref<Hazel::Texture2D> proceduralTexture = Hazel::Texture2D::Create(256, 256);
    
    // Example of setting texture data (using existing setData method)
    if (proceduralTexture)
    {
        // Create simple white texture data
        uint32_t whiteColor = 0xffffffff;
        proceduralTexture->setData(&whiteColor, sizeof(uint32_t));
    }
    
    // Bind textures using existing methods
    if (gltfTexture)
    {
        gltfTexture->Bind(0);  // Bind to texture slot 0
    }
    
    if (proceduralTexture)
    {
        proceduralTexture->Bind(1);  // Bind to texture slot 1
    }
    
    // Note: In a real application, these textures would be used in rendering
    // This demonstrates the texture creation process using existing Hazel functions
}

// Alternative texture creation function using different assets
void CreateAlternativeTextures()
{
    // Create different textures using existing assets
    Hazel::Ref<Hazel::Texture2D> checkerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
    Hazel::Ref<Hazel::Texture2D> logoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");
    Hazel::Ref<Hazel::Texture2D> triangleTexture = Hazel::Texture2D::Create("assets/textures/Triangle.png");
    
    // Use existing texture methods to get information
    if (checkerboardTexture)
    {
        uint32_t width = checkerboardTexture->GetWidth();
        uint32_t height = checkerboardTexture->GetHeight();
        // These dimensions can be used for further processing
    }
}

// Example usage function that demonstrates GLTF view application texture usage
void GLTFViewAppDemo()
{
    // This function demonstrates texture creation for a GLTF view application
    // using only existing Hazel engine functions without creating new classes
    
    // Create textures for GLTF model display
    CreateAndDisplayTexture();
    CreateAlternativeTextures();
    
    // Additional texture creation examples
    Hazel::Ref<Hazel::Texture2D> mainTexture = Hazel::Texture2D::Create("assets/textures/Ship.png");
    
    if (mainTexture)
    {
        // Use existing texture binding functionality
        mainTexture->Bind(0);
        
        // Get texture properties using existing methods
        uint32_t textureWidth = mainTexture->GetWidth();
        uint32_t textureHeight = mainTexture->GetHeight();
        
        // Unbind when done
        mainTexture->Unbind();
    }
}