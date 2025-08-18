# Shader Precision Guide

## Overview
This document explains how shader precision has been modified in the HazelT engine and provides guidance on different precision options.

## What was changed
Added explicit precision qualifiers to all shader files in both `Hazelnut/assets/shaders/` and `SandBox/assets/shaders/` directories:

- `squareshader.glsl`
- `FlatColor.glsl` 
- `Texture.glsl`
- `triangleshader.glsl`

## Precision Qualifiers Added

All shaders now include:
```glsl
precision mediump float;
```

This directive is added to both vertex and fragment shader sections for complete precision control.

## Available Precision Options

### 1. High Precision (`precision highp float;`)
- **Range**: Highest precision available
- **Performance**: Slowest on mobile devices
- **Use case**: Desktop applications, high-quality rendering where precision is critical
- **Mobile support**: May not be supported on all mobile GPUs

### 2. Medium Precision (`precision mediump float;`) - **Currently Used**
- **Range**: 16-bit floating point precision
- **Performance**: Good balance between quality and performance
- **Use case**: Most mobile and desktop applications
- **Mobile support**: Widely supported across mobile GPUs

### 3. Low Precision (`precision lowp float;`)
- **Range**: 8-bit precision
- **Performance**: Fastest on mobile devices
- **Use case**: Simple shaders, mobile applications where performance is critical
- **Quality**: May introduce visible artifacts in complex lighting calculations

## How to Modify Precision

To change the precision in any shader file:

1. Locate the precision directive in the shader:
   ```glsl
   precision mediump float;
   ```

2. Replace with desired precision:
   ```glsl
   precision highp float;    // For high precision
   precision lowp float;     // For low precision
   ```

3. Consider the trade-offs:
   - **High precision**: Better quality, slower performance
   - **Medium precision**: Balanced approach (recommended)
   - **Low precision**: Faster performance, potential quality loss

## Specific Variable Precision

You can also specify precision for individual variables:
```glsl
lowp vec4 color;           // Low precision color
mediump vec2 texCoord;     // Medium precision texture coordinates
highp vec3 worldPos;       // High precision world position
```

## Mobile Considerations

- Fragment shaders **require** precision qualifiers on mobile (OpenGL ES)
- Vertex shaders typically default to high precision on mobile
- Desktop OpenGL allows precision qualifiers but they're optional
- Always test on target mobile devices when changing precision

## Performance Impact

- Lower precision = faster rendering, especially on mobile GPUs
- Higher precision = better visual quality but slower performance
- Fragment shader precision has more impact than vertex shader precision
- Consider your target platform when choosing precision levels

## Compatibility Notes

- OpenGL ES requires precision qualifiers in fragment shaders
- Desktop OpenGL (core profile) treats precision qualifiers as hints
- When in doubt, use `mediump` for good compatibility and performance