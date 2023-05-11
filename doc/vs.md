# Create VS project | 创建 VS 工程

## First | 首先

We give a __CMakeLists.txt__ for you to create new VS project from it.

我们为你提供了 __CMakeLists.txt__ 用于创建新的 VS 工程。

If you are using this way, you will __not need__ to continue reading.

如果你使用的是这种方式，下面的内容你就 __不需要__ 再读了。

Creating VS project from a CMake file is the __simplest__ way.

从 CMake 文件创建 VS 工程是 __最简单__ 的方式。

## How to Create VS project

Make sure you are using VS 2022. You may not get the dynamic libraries by using this way to compile.

1. Get code from this repo using `git`.

2. In Visual Studio, click `File`->`New`->`Project From Existing Code...`.

3. Select `Visual C++`->`Next`->choose `project file location`->write the `project name` at ease->`Finish`.

4. Remove cpp files in `module` to avoid compilation problems.(they should be compiled to dynamic libraries)

5. Click `Source Files` in `Search Solution Explorer` at left, right click `main.cpp`, compile.

## 如何创建VS工程

确保你使用的是 VS 2022。这个方式可能无法生成一些动态库。

1. 用 `git` 从这个仓库获取代码。

2. 在VS的界面，点击文件(F)->新建(N)->从现有代码创建项目(E)。

3. 选择创建 `Visual C++` 项目->下一步->项目文件位置选择你下载的代码存放的文件夹->填项目名称，随便写->完成。

4. 从项目中去掉 `module` 里的cpp文件，以防止编译错误。(那些本应该编译到动态库)

5. 点开左侧解决方案资源管理器中的 `Source Files`，右键点击 `main.cpp`，编译。
