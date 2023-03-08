# weatherForecastApp

## 1. 项目来源
B站up主，大轮明王讲Qt

视频链接：https://www.bilibili.com/video/BV1D841147zt/?spm_id_from=333.788&vd_source=3fe012baa05e0a85a632ab8a4b77f14a

## 2. 项目预览
![image](https://user-images.githubusercontent.com/51503853/223753450-1e56a2ad-2fed-4ea2-af75-03722e8dea8c.png)

## 3. 注意事项
### 3.1 项目的编译环境是minGW，版本为Qt5
我尝试过Qt5.9.8和5.9.12都可以正常运行。MSVC不能正常运行程序，这是由于程序里的中文字符串都是用双引号直接括起来的，如 `"暴雪"` 。而使用MSVC，字符串需要 `QString::fromLocal8Bit("暴雪")` 才能正常显示，否则为乱码。所以程序在使用MSVC编译时，中文字符无法正常识别。如果希望使用MSVC编译，需要手动修改程序内的所有中文字符串格式。

### 3.2 项目代码与视频中的代码不完全一致
项目的资源和软件功能与up主视频中的一样，但我在写代码时有部分功能换了其他方法实现，例如`weatherTool`类中，视频中的json解析是写在头文件中的，我是在cpp文件中实现的。

视频中高低温绘图功能是使用事件过滤器实现的，而在本项目代码中我是在`TempLabel`类中，重写`paintEvent`事件实现的。事件过滤器实现的代码我保留在了`MainWindow`类的最后，需要的朋友可以参考。

还有一些其他小细节就不具体阐述了。

## 3.3 致谢
项目代码如果对大家的学习有帮助，请点击B站链接给视频点赞投币支持一下up主。
