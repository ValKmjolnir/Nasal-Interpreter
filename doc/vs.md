# Create VS project | 创建 VS 工程

## First | 首先

Make sure you are using VS 2022.

确保你使用的是 VS 2022。

## How to Create VS project

1. Get code from this repo using `git`.

2. In Visual Studio, click `File`->`New`->`Project From Existing Code...`.

3. Select `Visual C++`->`Next`->choose `project file location`->write the `project name` at ease->`Finish`.

4. Remove cpp files in `module` to avoid compilation problems.(they should be compiled to dynamic libraries)

5. Click `Source Files` in `Search Solution Explorer` at left, right click `main.cpp`, compile.

## 如何创建VS工程

1. 用`git`从这个仓库获取代码。

2. 在VS的界面，点击文件(F)->新建(N)->从现有代码创建项目(E)。

3. 选择创建`Visual C++`项目->下一步->项目文件位置选择你下载的代码存放的文件夹->填项目名称，随便写->完成。

4. 从项目中去掉 `module` 里的cpp文件，以防止编译错误。(那些本应该编译到动态库)

5. 点开左侧解决方案资源管理器中的`Source Files`，右键点击`main.cpp`，编译。
