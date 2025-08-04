# Core3D CPU-Side Shader Control Implementation

## Problem Statement (Chinese)
我要在cpu端的代码控制，不要直接改core3d_dm_fw.frag

**Translation**: "I want CPU-side code control, don't directly modify core3d_dm_fw.frag"

## Solution Overview

This implementation demonstrates **CPU-side parameter control** for 3D shaders instead of hardcoding values directly in the fragment shader. The approach follows graphics programming best practices by using uniforms to control shader behavior from the CPU.

## Key Components

### 1. Shader Files Created
- `SandBox/assets/shaders/core3d_dm_fw.frag` - Fragment shader with CPU-controlled parameters
- `SandBox/assets/shaders/Core3D_DM_FW.glsl` - Complete shader with vertex and fragment parts

### 2. CPU Control Layer
- `SandBox/src/Core3DLayer.h` - Header for 3D layer with parameter controls
- `SandBox/src/Core3DLayer.cpp` - Implementation with real-time parameter adjustment

### 3. Integration
- Modified `SandBox/src/SandBoxApp.cpp` to use Core3DLayer

## CPU-Controlled Parameters

Instead of hardcoding values in the shader, all parameters are controlled from CPU:

### Lighting Parameters
- Light position (`u_LightPosition`)
- Light color (`u_LightColor`)
- View position (`u_ViewPosition`)

### Material Parameters
- Ambient color (`u_MaterialAmbient`)
- Diffuse color (`u_MaterialDiffuse`)
- Specular color (`u_MaterialSpecular`)
- Shininess (`u_MaterialShininess`)

### Core3D Specific Parameters
- **DM (Deformation) Strength** (`u_DeformationStrength`) - Controls vertex deformation
- **FW (Framework) Intensity** (`u_FrameworkIntensity`) - Controls framework effects
- **Core 3D Color** (`u_Core3DColor`) - Base color for 3D rendering

## How It Works

1. **Shader Uniforms**: The fragment shader uses `uniform` variables instead of hardcoded constants
2. **CPU Updates**: The `Core3DLayer::UpdateShaderUniforms()` function sets all parameters from CPU
3. **Real-time Control**: ImGui interface allows real-time parameter adjustment
4. **No Hardcoding**: All effects can be controlled without modifying the fragment shader

## Benefits of CPU-Side Control

1. **Dynamic Adjustment**: Parameters can be changed at runtime
2. **Easy Debugging**: Values can be tweaked without recompiling shaders
3. **Configuration**: Different materials/effects can use the same shader
4. **Performance**: GPU gets optimized uniform values instead of calculations
5. **Maintainability**: Shader logic separated from parameter values

## Example Usage

```cpp
// In Core3DLayer::UpdateShaderUniforms()
m_Core3DShader->SetFloat("u_DeformationStrength", m_ShaderParams.deformationStrength);
m_Core3DShader->SetFloat("u_FrameworkIntensity", m_ShaderParams.frameworkIntensity);
m_Core3DShader->SetFloat3("u_Core3DColor", m_ShaderParams.core3DColor);
```

The ImGui interface provides sliders and color pickers for real-time adjustment of all parameters, demonstrating true CPU-side control of shader behavior.

## Build Notes

This implementation is designed for Windows as part of the Hazel engine. The code follows the existing Hazel architecture and shader management patterns.