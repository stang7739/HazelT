# Early-Z Optimization Implementation

## Overview
This implementation adds RenderNodeDefaultDepthRenderSlot to lwrp.rng and enables early-Z optimization in the Hazel Engine renderer.

## Files Added/Modified

### New Files Created
1. **lwrp.rng** - Lightweight Render Pipeline configuration file
2. **Hazel/src/Hazel/Renderer/RenderGraph.h** - Render graph system header
3. **Hazel/src/Hazel/Renderer/RenderGraph.cpp** - Render graph implementation
4. **Hazel/src/Hazel/Renderer/RenderNodeDefaultDepthRenderSlot.h** - Default depth render slot header
5. **Hazel/src/Hazel/Renderer/RenderNodeDefaultDepthRenderSlot.cpp** - Default depth render slot implementation

### Modified Files
1. **Hazel/src/Hazel/Renderer/RendererAPI.h** - Added early-Z and depth prepass methods
2. **Hazel/src/Platform/OpenGL/OpenGLRenererAPI.h** - Added OpenGL early-Z implementations
3. **Hazel/src/Platform/OpenGL/OpenGLRenererAPI.cpp** - Enhanced with early-Z optimizations
4. **Hazel/src/Hazel/Renderer/RenderCommand.h** - Added static early-Z methods
5. **Hazel/src/Hazel/Renderer/Renderer.h** - Added render graph integration
6. **Hazel/src/Hazel/Renderer/Renderer.cpp** - Implemented render graph initialization
7. **SandBox/src/SandBox2D.cpp** - Added render graph execution and ImGui display

## Key Features Implemented

### 1. RenderNodeDefaultDepthRenderSlot
- **Early-Z Optimization**: Uses GL_LEQUAL depth function for better early fragment rejection
- **Depth Prepass**: Renders depth-only pass first to populate depth buffer
- **Depth Clamping**: Enables GL_DEPTH_CLAMP when available for better early-Z performance
- **State Management**: Properly saves and restores OpenGL depth state

### 2. Enhanced OpenGL Renderer
- **Depth Function**: Changed from GL_LESS to GL_LEQUAL for better early-Z performance
- **Face Culling**: Enabled back-face culling to reduce fragment processing
- **Depth Prepass Methods**: Added BeginDepthPrepass() and EndDepthPrepass() for proper depth-only rendering
- **Optimized Clear**: Enhanced clear operations for depth buffer management

### 3. lwrp.rng Configuration
```json
{
    "renderGraph": {
        "name": "LightweightRenderPipeline",
        "renderNodes": [
            {
                "name": "DefaultDepthRenderSlot",
                "type": "RenderNodeDefaultDepthRenderSlot",
                "enabled": true,
                "settings": {
                    "earlyZEnabled": true,
                    "depthPrepassEnabled": true,
                    "depthFunc": "GL_LEQUAL",
                    "depthClampEnabled": true
                }
            }
        ]
    }
}
```

### 4. Render Graph System
- **Node-Based Architecture**: Extensible render node system for pipeline configuration
- **Configuration Loading**: Support for loading render pipeline from lwrp.rng file
- **Runtime Management**: Dynamic render node execution and state management

## Early-Z Optimization Benefits

1. **Reduced Fragment Processing**: Fragments that fail depth test are rejected early, before expensive fragment shading
2. **Better Memory Bandwidth**: Less texture reads and pixel shader computations for occluded pixels
3. **Improved Performance**: Significant performance gains for scenes with high depth complexity
4. **Power Efficiency**: Reduced GPU power consumption due to less fragment processing

## How It Works

1. **Initialization**: Renderer creates render graph and loads lwrp.rng configuration
2. **Depth Prepass**: RenderNodeDefaultDepthRenderSlot executes first, rendering geometry depth-only
3. **Early-Z Testing**: Subsequent rendering passes benefit from populated depth buffer
4. **Optimized Rendering**: Only visible fragments proceed to expensive color/lighting calculations

## Usage

The system is automatically initialized when the renderer starts. The early-Z optimization can be monitored through:

1. **Console Logs**: Check for "Early-Z optimization enabled" messages
2. **ImGui Panel**: Render graph stats displayed in SandBox2D settings panel
3. **Performance**: Measure frame time improvements in depth-complex scenes

## Performance Considerations

- **Scene Dependency**: Benefits vary based on scene depth complexity and overdraw
- **Geometry Sorting**: Front-to-back sorting maximizes early-Z effectiveness
- **Shader Complexity**: Greater benefits with more expensive fragment shaders
- **Hardware Support**: Modern GPUs have better early-Z hardware support

## Future Enhancements

1. **Automatic Geometry Sorting**: Implement front-to-back sorting for opaque objects
2. **Shader Variants**: Create depth-only shader variants for the prepass
3. **Occlusion Culling**: Add hierarchical Z-buffer for more aggressive culling
4. **Statistics**: Add detailed performance metrics and overdraw visualization