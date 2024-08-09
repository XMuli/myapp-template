

## \*.ui 单独编译为 ui_\*.h 文件于 CMake

```cmake
list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "ui/setting")  # 添加此行

# 将单独的 .ui 文件编译为 ui_*.h 文件
execute_process(COMMAND "C:/Qt/5.15.2/msvc2019_64/bin/uic.exe" -o "ui_sub_general.h" "sub_general.ui")
execute_process(COMMAND "C:/Qt/5.15.2/msvc2019_64/bin/uic.exe" -o "ui_sub_update.h" "sub_update.ui")
execute_process(COMMAND "C:/Qt/5.15.2/msvc2019_64/bin/uic.exe" -o "ui_sub_about.h" "sub_about.ui")
```



## Qt Design Guide

可解决设计：

1. 底部和右侧弹簧没有扩展的效果（拉伸比例单独设置为10，高度可能为1，扩展策略）
2. QTabWidget 的子page 中，直接放入widget+弹簧后布局（不要直接塞入一整个总的widget， 运行效果上下拉伸，不会置顶而是居中间）
3. 最底部检查更新的widget，弹簧高度设置为 1（避免 general 偏更高）；以及其它高度不对可设置为 Fix(实际 步骤1 效果更佳)

![image-20240809122245213](./assets/image-20240809122245213.png)

![image-20240809122251844](./assets/image-20240809122251844.png)



- **拉升比例为 1 也行**，10 的话若是某些组合控件，则会有一定高度，导致是会有总的最小高度是会增高的，

![image-20240809153051696](./assets/image-20240809153051696.png)