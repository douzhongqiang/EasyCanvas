基于Qt QGraphicsView的简单作图软件 <br />
- 开发环境使用VS2015和Qt5.13，64位。
- 如果您使用VS的其他版本编译需要自行编译PythonQt

软件绿色版（旧版本）下载地址：

链接：https://pan.baidu.com/s/1r2zKmt4ID_7p0V8Fm7e7cQ 提取码：m6n1

视频演示地址：https://www.bilibili.com/video/BV18K411L7Ca/

**新版本 V2.0下载地址：**

链接：https://pan.baidu.com/s/1vEJ1T2txrGtSjlBNHip-tg 提取码：94ds

**注意：** 

1. 这里使用的是64位的FFmpeg库，如果编译的时候报错，请编译成64位的版本或者自行替换成32位的FFMpeg库。

2. **由于DLL库文件太大，会导致Github仓库克隆失败，可以使用如下链接下载DLL库**

链接：https://pan.baidu.com/s/1B9hN8fEKvBQN7X-HqBlLZw   提取码：ubuq

3. **如果Git仓库克隆失败，可以使用如下链接获取源码，后可直接更新代码。**

链接：https://pan.baidu.com/s/1qn-L1Vc3cFSUdf8Rx5YLiA   提取码：0rsf

4. 源码中未提供Python库环境，如果运行失败可以安装Python3.8。或者将绿色版的 **Lib** 目录拷贝到生成的程序的可执行文件所在目录。**（PythonQt::Init()）** 程序闪退，可按此步骤解决。

# 1. 基本功能

1. 画布设置，可以修改画布的尺寸、颜色信息
2. 添加图元

- 画笔绘制
- 矩形
- 椭圆/圆
- 图片
- 自定义音频元素
- 文字

所有的元素可以自由改变尺寸、位置、旋转以及自己的其他属性信息。

3. 方案管理

   - 新建空画布
   - 保存方案
   - 加载方案
   - 保存为文件

4. Python命令

   更多命令详见： [ https://www.douzhq.cn/easycanvas_cmd/ ]( https://www.douzhq.cn/easycanvas_cmd/ )

5. 节点树同步显示节点信息
6. 撤销/重做功能
   

# 2. 模块介绍

- **AudioVideoCore**  音频解码和播放
- **Utils** 公共工具
- **CustomWidgets**  自定义基础Widget
- **CustomControls**  自定义基本控件
- **NDNodeAttribute**  节点和属性定义
- **NodeAttributeControl** 属性控件
- **thirdLibs** 第三方库
- **EasyCanvas**  Main App



# 3. 关于我

作者： 不会飞的纸飞机

个人博客主页： https://www.douzhq.cn

CSDN主页： https://blog.csdn.net/douzhq

