# HazelT
git submodule update --init --recursive 

imgui docking branch

imgui.h 4315行 取消注释
static inline bool  BeginPopupContextWindow(const char* str_id, ImGuiMouseButton mb, bool over_items) { return BeginPopupContextWindow(str_id, mb | (over_items ? 0 : ImGuiPopupFlags_NoOpenOverItems)); } // OBSOLETED in 1.77 (from June 2020)

工作路径 D:\HazelT\Hazel\HazelInut 

# OpenGLFramebuffer 类流程图

## 流程图说明

```plaintext
+-------------------+      +-----------------+
| OpenGLFramebuffer |      | Utils 命名空间   |
|   (主类)          |      | (辅助函数)       |
+-------------------+      +-----------------+
         |                         |
         v                         |
+-------------------+              |
|    构造函数        |--------------+
| • 初始化规格参数    | 使用辅助函数
+-------------------+ 
         |
         v
+-----------------------+
|     Invalidate()      |
| • 创建FBO和纹理       |
| • 设置颜色/深度附件   |
+-----------------------+
         |
         v
+-------------------+      +-----------------+
|     Resize()      |      |     Bind()      |
| • 调整FBO大小     |      | • 绑定FBO       |
+-------------------+      +-----------------+
         |                         |
         v                         |
+-------------------+      +-----------------+
|    析构函数        |      |    Unbind()     |
| • 清理资源        |      | • 解绑FBO       |
+-------------------+      +-----------------+