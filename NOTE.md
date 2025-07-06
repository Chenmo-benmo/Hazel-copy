# Chenmobenmo 241012 P7 premake
1. 在 vendor\bin\premake 路径下安装了 premake5
2. 编写 premake5.lua 程序，可以将代码生成VS程序，bin 和 bin-int 文件夹以及 sln、vcxproj 文件都是使用 premake 后生成的产物
3. 在编写 premake5.lua 过程中，第45行相较于 Cherno 添加了一行手动创建 Sandbox 文件夹的操作，否则会报错
   https://github.com/premake/premake-core/wiki/postbuildcommands 查询函数使用方法
4. 编写 GenerateProjects.bat 程序，点击可以直接执行命令行指令，拖进VS或者code可以编写
   call     直接写进命令行
   PAUSE    使命令行不会自动关闭


# Chenmobenmo 241013 P8 事件系统计划
1. 接下来应该为 Hazel 添加鼠标和键盘映射


# Chenmobenmo 241015 P9 事件系统
1. 创建 Hazel_1\src\Hazel\Events 文件夹，在该文件夹内添加 Event.h, ApplicationEvent.h, KeyEvent.h, MouseEvent.h
   Event.h              包含所有事件的基类
   ApplicationEvent.h   包含窗口的几个基础事件
   KeyEvent.h           包含键盘的几个基础事件
   MouseEvent.h         包含鼠标的几个基础事件
2. 将spdlog版本替换成1.2.1


# Chenmobenmo 241016 P10 预编译头
1. 在 Hazel_1\src 下创建 hzpch.h 和 hzpch.cpp 
2. 在 hzpch.h 文件中添加所有会用到的库文件
3. 在 premake5.lua 文件中添加创建和使用预编译头的部分
   pchheader "hzpch.h"                 使用预编译头
   pchsource "Hazel_1/src/hzpch.cpp"   创建预编译头
   相当于 hzpch.cpp /属性/CC++/预编译头/预编译头（创建）预编译头文件（hzpch.h）
         项目（Hazel_1）/属性/CC++/预编译头/预编译头（使用）预编译头文件（hzpch.h）
4. 预编译头添加在所有 .cpp 文件中
5. 使用预编译头可以加快程序运行速度
   仅添加头文件相当于复制头文件内的所有源代码，处理起来非常耗时
   使用预编译头，编译器会将预编译头文件编译成二进制 .pch 形式；使用预编译头，编译器可以直接引用这个预编译的二进制 .pch 文件，而不是再次解析和处理这些头文件，显著减少时间


# Chenmobenmo 241018 P11 窗口抽象和GLFW
1. 添加 GLFW 命令行指令：git submodule add https://github.com/Thecherno/glfw Hazel/vendor/GLFW
2. GLFW\premake5.lua 需要修改才能使用，详见该文件里“解决bug部分”
3. Hazel_1\src\Hazel 下添加 Window.h
4. 创建 Hazel_1\src\Platform\Windows 文件夹，该文件夹内添加 WindowsWindow.h 和 WindowsWindow.cpp
5. premake5.lua 添加

# Chenmobenmo 241028 P12 窗口事件
1. 创建 Hazel_1\src\Hazel\Events 文件夹，该文件夹内添加 ApplicationEvent.h  Event.h  KeyEvent.h  MouseEvent.h
2. 添加对窗口的事件回调函数，包括窗口、键盘、鼠标等事件
3. 回调函数绑定在窗口句柄上，当窗口上发生事件，就会执行对应的回调函数
4. 回调函数返回 Hazel_ + 该动作
e. 创建实例，即创建一个静态变量指针指向自己，十分实用

# Chenmobenmo 241030 P13 Layers
1. 在 Hazel_1\src\Hazel 下添加 Layer.h  Layer.cpp  LayerStack.h  LayerStack.cpp
2. 添加层，层可以决定每一帧的额外操作，并可以添加层或者删除层

# Chenmobenmo 241031 P14 现代OpenGL和Glad
1. 创建 Hazel_1\vendor\Glad 文件夹，下载 Glad 保存进该文件夹
2. 添加 Glad\premake5.lua 
3. 修改 premake5.lua
4. 在程序内添加并初始化 glad，并使 <glad> 替代 <glfw3>

# Chenmobenmo 241101 P15 ImGui
1. 在 Hazel_1\vendor 文件夹下添加 Cherno 的 ImGui
2. 修改 premake5.lua
3. 运行时会出现未识别字符 DC，给报错区域注释掉即可
4. 创建 Hazel_1\src\Hazel\ImGui 文件夹，该文件夹内添加 ImGuiLayer.h ImGuiLayer.cpp
5. 创建 Hazel_1\src\Platform\OpenGL 文件夹，该文件夹内添加 ImGuiOpenGLRenderer.h ImGuiOpenGLRenderer.cpp
6. 详见 https://blog.csdn.net/weixin_61943345/article/details/135583216

# Chenmobenmo 241108 P16 ImGui事件
1. 为 ImGui 层添加附加、更新、事件
2. ImGui 事件增加了鼠标点击、鼠标释放、鼠标移动、鼠标滚轮、键盘按键、键盘释放、字符输入、调整窗口大小
e. 设置回调事件, std::bind，不需要独自创建实例即可使用成员函数，十分实用

# Chenmobenmo 241113 P17 Github和Hazel仓库
1. 优化 premake5.lua

# Chenmobenmo 241113 P18 推送请求(PR)
1. 优化 premake5.lua 
	"{MKDIR} %[bin/" .. outputdir .. "/Sandbox]",
	("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox/")
   拆分为目录和文件比创建文件夹来的更简洁
   ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")

# Chenmobenmo 241117 P19 Input轮询
1. 在 Hazel_1\src\Hazel 内添加 Input.h
2. 在 Hazel_1\src\Platform\Windows 内添加 WindowsInput.h WindowsInput.cpp
e. inline, static, auto[], std::pair

# Chenmobenmo 241119 P20 按键和鼠标代码
1. 在 Hazel_1\src\Hazel 内添加 KeyCodes.h MouseButtonCodes.h
2. 添加属于 Hazel 的按键和鼠标按键的代码

# Chenmobenmo 241121 P21 数学
1. 创建 Hazel_1\src\vendor\glm 文件夹，并添加 OpenGL Mathematics(glm) 数学库
2. 修改 premake5.lua 将 glm 添加进项目中

# Chenmobenmo 241121 P22 ImGui停靠和视口
1. 在 Hazel_1\src\Hazel\ImGui 内添加 ImGuiBuild.cpp
2. 删除 Hazel_1\src\Platform\OpenGL 文件夹内的 ImGuiOpenGLRenderer.h ImGuiOpenGLRenderer.cpp
3. 重大修改 ImGuiLayer.h ImGuiLayer.cpp 
4. 实现了停靠和多视口

# Chenmobenmo 241122 P23 渲染简介
1. Cherno 电脑坏了，没有代码
2. 渲染器需要使用 GPU，GPU 拥有更快的数学运算速度
3. 相比于 OpenGL，Vulcan 和 DirectX12 更加先进和优秀，但也更加复杂

# Chenmobenmo 241123 P24 渲染构架
1. Cherno PPT

# Chenmobenmo 241124 P25 渲染和维护
1. Hazel_1 预处理器添加 IMGUI_API=_declspec(dllexport);
   ImGui   预处理器添加 IMGUI_API=_declspec(dllexport);
   Sandbox 预处理器添加 IMGUI_API=_declspec(dllimport); 用来处理 imgui 静态库不能正确复制进 dll 中
2. Hazel_1 将会是静态库，静态库相对便于编写，而动态库相对便于使用

# Chenmobenmo 241126 P26 静态库和无警告
1. 所有的 premake5 文件都有所更改，将所有动态库部分改为静态库
2. 改动参考 https://blog.csdn.net/weixin_61943345/article/details/135884093
3. Core.h 隐藏动态库定义，并删除除 Core.h 文件外所有的 HAZEL_API

# Chenmobenmo 241128 P27 渲染上下文
1. 创建 Hazel_1\src\Hazel\Renderer 文件夹，并添加 GraphicsContext.h
2. 在 Hazel_1\src\Platform\OpenGL 文件夹下添加 OpenGLContext.h OpenGLContext.cpp
3. 将 OpenGL 创建上下文和控制每一帧刷新的函数抽象到 OpenGLContext 中
e. 基类使用派生类构建后，只能使用派生类里包含的基类的成员函数 (构造函数和删除函数名称不同也也会继承)

# Chenmobenmo 241130 P28 首个三角形
1. 添加 OpenGL 函数实现 VertexArray顶点数组 VertexBuffer缓冲区 IndexBuffer索引缓冲区 绘画三角形
2. 启用英伟达显卡驱动，打印显卡信息

# Chenmobenmo 241201 P29 OpenGL着色器
1. 在 Hazel_1/src/Hazel/Renderer 文件夹中添加 Shader.h Shader.cpp
2. 代码大部分从 https://www.khronos.org/opengl/wiki/Shader_Compilation Example 复制
3. 在缓冲区后面添加着色器
e. 了解着色器的语言书写规律
e. std::vector::data 返回一个指向底层数组的指针 十分实用

# Chenmobenmo 241210 P30 渲染接口抽象
1. 在 Hazel_1/src/Hazel/Renderer 文件夹中添加 Buffer.h Buffer.cpp Renderer.h Renderer.cpp
2. 在 Hazel_1/src/Platform/OpenGL 文件夹中添加 OpenGLBuffer.h OpenGLBuffer.cpp
e. 将 VertexBuffer 和 IndexBuffer 抽象出去，在 Buffer.h 中创建两种缓冲区的基类用来连接 OpenGL，Renderer.h 中创建选择哪种渲染器的筛选
e. 使用静态成员函数 构造派生类实例 并传递给 基类，十分实用
e. std::unique_ptr::reset

# Chenmobenmo 241215 P31 顶点缓冲区布局
1. 在 Hazel_1/src/Hazel/Renderer/Buffer.h 中添加对顶点属性的设置抽象
e. 顶点属性的设置 glVertexAttribPointer 函数的参数意义
e. 迭代器
e. initializer_list 可读列表
e. uint32_t 和 unsigned int 区别

# Chenmobenmo 250427 P32 顶点数组
1. 在 Hazel_1/src/Hazel/Renderer 文件夹中添加 VertexArray.h VertexArray.cpp
2. 在 Hazel_1/src/Platform/OpenGL 文件夹中添加 OpenGLVertexArray.h OpenGLVertexArray.cpp
3. 将 VertexArray 抽象出去，并将 VertexBuffer 和 IndexBuffer 加入到 VertexArray 类中，并用 square test 代码在 Application.cpp 中实现，m_VertexBuffer 和 m_IndexBuffer 不需要出现在 Application.h 中
e. 将 VertexArray 相关代码以及 glEnableVertexAttribArray glVertexAttribPointer 抽象出去
e. 将 VertexBuffer 和 IndexBuffer 加载到 VertexArray 中，使 Application 中不会出现 OpenGL 代码且只使用 VertexArray 操作

# Chenmobemo 250501 P33 渲染流和提交
1. 在 Hazel_1/src/Hazel/Rendere 文件夹中添加 RendererAPI.h RendererAPI.cpp RenderCommand.h RenderCommand.cpp
2. 在 Hazel_1/src/Platform/OpenGL 文件夹中添加 OpenGLRendererAPI.h OpenGLRendererAPI.cpp
3. 将 glClearColor glClear glDrawElements 抽象出去
4. OpenGLRendererAPI 是 RendererAPI 的派生类，RenderCommand 通过静态方法创建 s_RendererAPI 成员并链接 RendererAPI，Renderer 通过 静态内联函数链接 RenderCommand
5. Application 中删除 glad.h 头文件，OpenGL 代码全部抽象出 Application

# Chenmobenmo 250505 P34 相机及其原理
1. 认识相机工作原理
2. (V)视图矩阵 描述相机位置，(P)投影矩阵 描述视野里的物体，(M)模型矩阵 描述物体位置
3. Mat4矩阵中各元素的意义
   M = [
      m00, m01, m02, m03;
      m10, m11, m12, m13;
      m20, m21, m22, m23;
      m30, m31, m32, m33
   ]
   m00、m01、m02 描述X轴方向的变换 m03 描述x轴方向的平移
   m10、m11、m12 描述y轴方向的变换 m13 描述y轴方向的平移
   m20、m21、m22 描述z轴方向的变换 m23 描述z轴方向的平移
   m30、m31、m32 通常为0，用于保持举着你的齐次形式 m33 通常为1，用于保持变换的齐次性
4. TRS 平移，旋转，缩放
   T = [             
      1, 0, 0, tx;   
      0, 1, 0, ty;   
      0, 0, 1, tz;   
      0, 0, 0, 1     
   ]                 

   RX = [                     RY = [                     RZ = [
      1, 0,     0,     0;         cosθ, 0, sinθ, 0;         cosθ, -sinθ, 0, 0;
      0, cosθ, -sinθ,  0;         0,    1, 0,    0;         sinθ,  cosθ, 0, 0;
      0, sinθ,  cosθ,  0;        -sinθ, 0, cosθ, 0;         0,     0,    1, 0;
      0, 0,     0,     1          0,    0, 0,    1          0,     0,    0, 1
   ]                          ]                          ]

   S = [
      sx, 0,  0,  0;
      0,  sy, 0,  0;
      0,  0,  sz, 0;
      0,  0,  0,  1
   ]
5. M = T * R * S  需要注意变换顺序，这里是先缩放后旋转再平移
6. MVP = P * V * M
7. Vclip = vertex * MVP 在顶点着色器中将顶点坐标从模型空间转换到裁剪空间
8. 规划 BeginScene 和 EndScene 
e. 相机实现的原理

# Chenmobenmo 250507 P35 创建正交相机
1. 在 Hazel_1/src/Hazel/Rendere 文件夹中添加 OrthographicCamera.h OrthographicCamera.cpp 
2. 在 OrthographicCamera 中设置相机所需的视图矩阵、投影矩阵，并算出VP矩阵
3. 在 Shader.h 中添加 UploadUniformMat4() 函数用于向着色器传递VP矩阵
4. 在 Application.cpp 中Shader设置相机
5. 将着色器的 Bind UploadUniformMat4 放入 Renderer.h 的 submit 函数中
e. 能控制相机的平移和旋转

# Chenmobenmo 250510 P36 整理代码到Sandbox
1. 将 Application 中的关于渲染的代码全部移动到 Sandbox 的 ExampleLayer 中，Application() 移动到 ExampleLayer()，Application::Run() 移动到 ExampleLayer::OnUpdate()
2. 在 ExampleLayer::OnUpdate() 添加按键控制相机旋转和方向
e. 修改过多，渲染代码注释详见 Application 老代码

# Chenmobenmo 250511 P37 TimeStep和增量时间
1. 创建 Hazel_1/src/Hazel/Core 文件夹，并添加 Timestep.h 文件
2. Timestep 用于存储每帧的时间
3. 在 Application::Run() 函数的循环中添加计算每个循环所耗费时间的方法
4. 通过 每帧的时间 * 速度 可以得到固定的速度，即可脱离刷新率的影响
e. 每帧的时间 * 速度 得到的固定的速度
e. 结构体中 operator float() 重构， 直接可以调用 Timestep ts; ts可以直接返回m_Time 十分实用

# Chenmobenmo 250512 P38 变换
1. 将 Renderer::Submit() 函数添加物体变换矩阵
2. Sandbox 中添加可以同时操作的多个正方形，即用循环规定 不同的物体变换矩阵，缩放后调用 Renderer::Submit() 得到变换后的各个正方形
e. 可以使用 for 创建多个渲染物体

# Chenmobenmo 250515 P39 材质系统
1. 在 Shader.h 中添加 UploadUniformFloat4() 函数用于向着色器传递 vec4 数组，包括 u_Color 片段着色器颜色
2. 将 m_BlueShader BlueShaderVertexSrc BlueShaderFragmentSrc 重命名为 m_FlatColorShader flatColorShaderVertexSrc flatColorShaderFragmentSrc
3. 规划了关于材质以及纹理的未来走向

# Chenmobenmo 250515 P40 着色器抽象和统一变量
1. 在 Hazel_1/src/Platform/OpenGL 文件夹中添加 OpenGLShader.h OpenGLShader.cpp
2. 将 Shader.h 文件复制进 OpenGLShader.h，Shader.cpp 文件复制进 OpenGLShader.cpp，重写 Shader.h Shader.cpp，将 Shader 抽象出去，和 Buffer 中的实现方式一样
3. 在 OpenGLShader 中添加 UploadUniformInt() UploadUniformFloat() UploadUniformFloat2() UploadUniformFloat3() UploadUniformMat3() 函数
e. Renderer::Submit() 中 std::dynamic_pointer_cast<>() 将基类的 std::shared_ptr 转换为派生类的 std::shared_ptr，并在运行时检查类型兼容性，遇到失败行为返回空指针 十分实用

# Chenmobenmo 250516 P41 Ref&Scope和智能指针
1. 在 Core.h 文件中 添加将 Hazel::Scope<> 包装 std::unique_ptr<>，将 Hazel::Ref<> 包装 std::shared_ptr<> 的代码
2. 替换所有的 std::shared_ptr 为 Ref，Log 日志部分不使用 Hazel 部分的包装指针，Ref Scope 仅在 Hazel 内部使用
e. Hazel::Scope 包装 std::unique_ptr，Hazel::Ref 包装 std::shared_ptr，使用 template<> using 实现 十分实用

# Chenmobenmo 250516 P42 纹理
1. 在 Hazel_1/src/Platform/OpenGL 文件夹中添加 Texture.h Texture.cpp
2. 在 Hazel_1/src/Platform/OpenGL 文件夹中添加 OpenGLTexture.h OpenGLTexture.cpp
3. 在 Hazel_1/vendor 下创建 stb_image 文件夹，并添加 stb_image.h stb_image.cpp
4. 修改 premake5.lua 文件，将 stb_image 文件加入 Hazel_1 的包含目录
5. stb_image.h 内容从 https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image.h 复制
6. 在 Sandbox 中添加 m_TextureShader 着色器来放置纹理
e. 使用 Hazel::Ref 构造 Create() 函数，使用 = 直接传递指针，不需要使用 reset 接管常规指针
e. 直接使用 = 传递指针更加安全，使用 reset 接管得确保未被其他 std::shared_ptr 接管
e. OpenGLTexture.cpp 中设置纹理，有大量注释

# Chenmobenmo 250521
1. 将 LayerStack::~LayerStack() 函数注释掉，程序关闭后 memory 文件不会报错
e. Layer 由 LayerStack 管理，但 LayerStack 由 Application 管理，delete App 时也释放了 Layer 的内存，所以 ~LayerStack() 会重复释放内存，注释掉就可以了
e. 或者将 Layer 改成 Red 智能指针

# Chenmobenmo 250522 P43 混合
1. 在 Sandbox 中添加 m_CheonoTexture 纹理
2. 在 OpenGLTexture2D 中添加通过图像判断 输入的内部格式 GL_RGB8 GL_RGBA8, 输出的数据类型 GL_RGB GL_RGBA
3. 在 OpenGLRendererAPI::Init() 函数中添加启用混合功能函数，在 Application::Application() 中使用
e. 混合公式
   R = (Rsrc * α) + (Rdest * (1 - α)) = Rdest
   G = (Gsrc * α) + (Gdest * (1 - α)) = Gdest
   B = (Bsrc * α) + (Bdest * (1 - α)) = Bdest
   A = (Asrc * α) + (Adest * (1 - α)) = Adest
   Result = Source.RGB * Source.Alpha + Destination.RGB * (1 - Source.Alpha)

   Rsrc, Gsrc, Bsrc, Asrc 是源图像（前景）的 RGBA 值
   Rdest, Gdest, Bdest, Adest 是目标图像（背景）的 RGBA 值
   α 是源图像的 Alpha 通道值（透明度）

# Chenmobenmo 250529 P44 着色器资源文件
1. 创建 Sandbox/assets/shaders/Texture.glsl 文件，将 SandboxApp 中 textureShaderVertexSrc textureShaderFragmentSrc 复制进 Texture.glsl 文件中
2. 在 OpenGLShader.h 中添加 OpenGLShader(const std::string& filepath) ReadFile() PreProcess() Compile() 函数
3. ReadFile() 函数负责读取文件 PreProcess() 分类顶点着色器、片段着色器 Compile() 函数负责着色器上下文
4. OpenGLShader 构造函数重新编辑
5. OpenGLShader.h 文件中添加 glad 文件会报错，选择重命名 unsigned int 为 GLenum 暂时处理报错
e. std::ifstream in(filepath, std::ios::in, std::ios::binary)     以二进制模式 std::ios::binary 打开文件，避免文本转换
   in.seekg(0, std::ios::end)                                     将文件指针移动到文件末尾，0 表示偏移量，std::ifstream::seekg() 用于移动文件指针的位置，std::ios::end 表示文件末尾
   result.resize(in.tellg())                                      获取文件指针位置用来计算文件大小，std::ifstream::tellg() 获取文件指针所在位置
                                                                  resize() 用于设置字符串的逻辑大小，reserve() 用于设置字符串的内存大小；逻辑大小设置后会裁剪当前的大小，再根据后输入的字符串扩容；内存大小设置后无法修改当前大小，如果小于当前大小则自动扩容；resize() 新增元素会初始化，reserve() 新增元素不会初始化
                                                                  resize() 常用于设置字符串或者裁剪字符串，reserve() 常用于预分配内存避免多次扩容，两者结合使用可实现高效的内存管理
   in.seekg(0, std::ios::beg)                                     std::ifstream::beg 表示文件开头
   in.read(&result[0], result.size())                             获取 result 底层字符串数组指针，并规定读取的字节数，将整个内容读取到 result 中
   in.close()                                                     关闭文件，虽然会自动关闭，但手动关闭是个好习惯

e. std::unordered_map<key, value>                                 存储键值对 key-value，通过 key 快速查找 value
   size_t typeTokenLength = strlen(typeToken)                     size_t 取决与系统构架，32位系统：unsigned int，64位系统：unsigned long 或 unsigned long long
                                                                  strlen() 计算字符串长度
   size_t pos = source.find(typeToken, 0)                         std::string::find() 从位置 0 开始，在 source 中查找 typeToken 的首次出现的位置
   size_t eol = source.find_first_of("\r\n", pos)                 std::string::find_first_of() 从位置 pos 开始，在 source 中查找首次出现属于 "\r\n" 集合的 字符的位置
   std::string type = source.substr(begin, eol - begin)           std::string::substr() 从位置 begin 开始，提取长度为 eol - begin 的字符串
   size_t nextLinePos = source.find_first_not_of("\r\n", eol)     std::string::find_first_not_of() 从位置 eol 开始，在 source 中查找首次出现不属于 "\r\n" 集合的 字符的位置

e. shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
   //pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos)得到的是source.size
   //应该得到source.size - nextLinePos才不会超出范围
   //但这里依旧能跑

e. 文件提取，十分实用

# Chenmobenmo 250531 P45 着色器库
1. 将 OpenGLShader::Compile 中 glShaderIDs 的变量由 std::vector<GLenum> 改为 std::array<GLenum, 2>，能更好的限制 glShaderIDs 的大小
2. 在 Shader.h 中添加 ShaderLibrary 类，用于将 Shader 保存在里面并能根据 name 来查找对应的 Shader
3. 在 Shader::Create() 函数和 OpenGLShader 构造函数中添加 name 参数，在 OpenGLShader 构造函数中查找文件名并将文件名当作 name
4. 添加 OpenGLShader::GetName() 函数
5. 将 Sandbox 中 m_TextureShader 去掉并用 ShaderLibrary 实现加载着色器 Load() 和获取着色器 Get()
e. 如果使用 std::vector<> e(size) 构造函数，效果和 std::vector<>::resize(size) 函数效果一样，默认初始化个数为 size 的内容为0的元素，再进行 push_back 则是对 size + 1 进行操作
   如果使用 reserve(size) 因为 reserve() 不会进行初始化，则不会出现上述情况，如果 push_back 超出大小则重新分配内存
e. std::string::rfind() 查找特定子字符串和字符
e. std::unordered_map::find 会返回一个迭代器，如果存在则返回对应的迭代器，如果不存在则返回 end 末尾后的迭代器

# Chenmobenmo 250606 P46 如何构建2D渲染器
1. 介绍了2D游戏引擎未来所需要的
   保持帧率的大量元素的渲染
   一个元素拥有多个纹理，即动画帧
   暂停界面，停止渲染动作
   用户交互
   粒子特效等

# Chenmobenmo 250607 P47 摄像机控制器
1. 在 Hazel_1/src/Hazel 中添加 OrthographicCameraController.h OrthographicCameraController.cpp
2. OrthographicCameraController 负责控制相机
3. 将 Sandbox 中创建控制相机的和控制相机的代码转移到 OrthographicCameraController::OnUpdate 中
4. 添加 OrthographicCameraController::OnEvent 提交鼠标滚轮事件和窗口大小缩放事件
5. 添加 OrthographicCamera::SetProjection 函数重新设置相机位置
e. 通过滚轮来改变缩放级别，以缩放级别的改变 来改变相机视角边界的大小，以达到视图缩小产生相机上升的视觉效果
   因为相机速度是视图速度所产生的视觉效果，所以通过改变视图速度可以达到相机速度不变的目的

# Chenmobenmo 250609 P48 调整大小
1. 添加 Application::OnWindowResize 用来获取窗口大小并设置视口大小与窗口大小相同
2. 添加 Renderer::OnWindowResize 调用 RenderCommand::SetViewport，再通过静态方法调用 RendererAPI::SetViewport，延伸到派生类调用 OpenGLRendererAPI::SetViewport
3. OpenGLRendererAPI::SetViewport 用于定义视口，保持视口大小与窗口大小一致
4. Application 中添加 m_Minimized 来判断窗口是否最小化，如果最小化停止渲染，但不停止 ImGui
5. 添加 OrthographicCameraController::GetZoomLevel 和 OrthographicCameraController::SetZoomLevel 来获取和设置缩放级别
e. 将相机视角边界的纵横比改变为窗口纵横比，如果相机视角纵横比和窗口纵横比不相等则会出现 渲染压缩或者拉伸的现象
   因为改变的是纵横比，当横向操作时渲染不会改变，纵向操作时渲染依旧会等比例缩放
   纵向操作时，因为纵向渲染全部，所以纵向按比例缩小
   如果纵向操作时保持 相机视角纵向大小和窗口纵向大小一致，就可以实现纵向横向操作不会改变渲染大小

# Chenmobenmo 250623 P49 维护
1. GenerateProject.bat 重命名为 Win-GenProjects.bat，修改文件内容
2. 在 GameEngine 文件夹下创建 scripts 文件夹并将 Win-GenProject.bat 文件移动进去
3. 将 Hazel_1\src\Hazel 下的所有 *h *cpp 文件转移到 Core 文件夹中，将 OrthographicCameraController.h OrthographicCameraController.cpp 转移到 Renderer 文件夹中
4. 修改代码中的 include 文件路径
5. 确保代码在 Debug 和 Release 下都能成功运行

# Chenmobenmo 250624 P50 2D渲染准备
1. 在 Core.h 文件中使用预定义宏进行平台检测，只是设想未实施进代码
2. 在 Sandbox/src 下添加 Sandbox2D.h Sandbox2D.cpp，Sandbox2D 中是一个类似于 ExampleLayer 的一个新层，可绘制一个正方形
3. 在 Sandbox/assets/shaders 下添加 FlatColor.glsl 着色器文件
4. 在 Sandbox() 中加入新层 PushLayer(new Sandbox2D())
e. 使用最新 API 接口绘制所需要的函数

# Chenmobenmo 250628 P51 启动我们的二维渲染器
1. 在 Hazel_1/src/Hazel/Renderer 下添加 Renderer2D.h Renderer2D.cpp
2. 将 Sandbox2D::OnAttach() 内的代码转移到 Renderer2D::Init() 内，将 Renderer2D::Init() 函数在 Renderer::Init() 中启用
3. 将 Sandbox2D::OnUpdate() 内关于 OpenGL 的代码转移到 Renderer2D::DrawQuad() 内
4. 这样 Renderer2D 内仅存在设置方块颜色和设置相机，设置 VA 和 Shader 的代码全部封装到 Renderer2D 中
e. const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	//第一个 const 表示返回常量引用，防止外部通过返回值修改内部状态
	//第二个 const 声明常量成员函数，保证函数不修改对象状态 且支持 const 对象调用

# Chenmobenmo 250702 P52 渲染器变换
1. 在 Shader.h 中加入 SetFloat3() SetFloat4() SetMat4() 函数代替 OpenGLShader.h 中的 UploadUniformFloat3() UploadUniformFloat4() UploadUniformMat4()
2. 在 Renderer2D::DrawQuad() 中添加设置物体位置和缩放大小的代码
e. 添加了新的虚函数，不需要 std::synamic_pointer_cast 转换到派生类，OpenGLShader 不会暴露在用户层
   设置缩放大小可以单独设置物体的长宽高缩放大小，也可以设置多个物体

# Chenmobenmo 250704 P53 渲染器纹理