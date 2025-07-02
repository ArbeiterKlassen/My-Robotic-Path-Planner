# My Robotic-Path-Planner
## 项目概述
 本项目基于MFC（Microsoft Foundation Classes）实现了一个机器人路径规划系统，支持地图加载、路径搜索和结果展示。适用于机器人导航、游戏AI寻路等场景。
 ### 主要功能
 - 支持随机生成地图或者从本地及网络导入地图文件
 - 显示机器人搜索过程及最短路径
 - 通过堆优化的Djikstra ALgorithm算法对比Floyd多源最短路、Bellman—Flod和SPFA算法实现最优路径搜索 。
## 运行环境
- 开发工具：Visual Studio 2022
- 依赖库：MFC，CSocket，WinINet，OpenCV
- 操作系统：Windows 7/8/8.1/10/11/Server 2016及以上
## 使用方法
### 1.环境配置
- 打开Windows的“**编辑系统环境变量**”页面，点击“**环境变量**”，在用户变量旁点击“**编辑**”，将自己下载的opencv路径添加到“**path**”中并应用，如**C:\Users\HP\Downloads\opencv\build\x64\vc16\bin**，样式必须与例示一致。
- 使用Visual Studio打开**Robot_Path_Planning_MFC_OpenCV.sln**，右键主项目打开**属性**，点击**VC++目录**，点击包含目录右侧箭头，点击编辑，将路径改成下载的opencv中的**include**文件，例如**C:\Users\HP\Downloads\opencv\build\include**。
- 再对**库目录**进行同样的操作，路径改为**lib**文件，例如                                                                                                         **C:\Users\HP\Downloads\opencv\build\x64\vc16\lib**。
- 再点击**属性页**中**链接器**下的**输入**，找到**附加依赖项**，将其改为**lib**文件夹中自己下载版本的opencv名称，例如        **opencv_world4110d.lib**。
***注意***：附加依赖项中的文件是末尾带“**d**”的那一个。
                  完成以上操作后，opencv的环境配置就做好了。



### 2.编译运行
1.使用Visual Studio打开**Robot_Path_Planning_MFC_OpenCV.sln**；
2.编译并运行。
### 3.使用说明
- **创建地图**
点击“**从本地打开**”可导入系统中的地图文件，或者点击“**从网络导入**”通过“**IP**”和“**Port**”在网络上导入地图文件，还可以点击“**继续但不指定地图**”按钮，以用户自己输入的“**长**”、“**宽**”、“**高**”随机生成地图。点击“**确认并开始**”即可跳转至地图与路径规划页面。
- **查看地图**
点击下方“**上一页**”和“**下一页**”按钮可查看不同层次的地图。
- **设置起点终点**
在操作界面中的“**起点X**”、“**起点Y**”和“**起点Z**”等框内可输入起点坐标和终点坐标。（注：起点为绿色方块，终点为蓝色方块）
- **运行路径规划**
点击“**开始计算**”按钮，程序将计算出从设置的起点到终点的最优路径并在右上方显示出计算过程，再点击“**显示结果**”按钮，程序会将计算出的最优路径可视化并显示在地图中。
- **更改地图**
你可以在更改下方输入你需要的墙的坐标，点击“**添加墙**”以生成墙，再次点击“**开始计算**”即可获得更改后的最优路径。（注：生成的墙为红色方块）
- **导出结果**
 选择你想要的导出结果，比如点击“**导出为.csv**”，选择合适的位置再点击“**确定**”，即可导出计算出的最优路径。
 ## 算法实现
 核心代码在```Operation_Page.cpp```中
 
路径规划:
 ````C++
 std::priority_queue<Node> to;
//to.push(Node(x_now,y_now))
to.push(Node(x_now, y_now, z_now, _SET));
Graph[Start_z][Start_y][Start_x] = Vmap[Start_z][Start_y][Start_x];
int dis;
int tx, ty, tz;
while (!to.empty()) {
	tx = to.top().x;
	ty = to.top().y;
	tz = to.top().z;
	draw_box_color(tx, ty, tz, WAY_COLOR);
	to.pop();
	if (visited[tz][ty][tx])continue;
	visited[tz][ty][tx] = true;
	if (Vmap[tz][ty][tx - 1] > 0 && !visited[tz][ty][tx - 1]) {//LEFT
		to.push(Node(tx - 1, ty, tz, _SET));
		draw_box_color(tx - 1, ty, tz, WAY_COLOR);
	}
	if (Vmap[tz][ty][tx + 1] > 0 && !visited[tz][ty][tx + 1]) {//RIGHT
		to.push(Node(tx + 1, ty, tz, _SET));
		draw_box_color(tx + 1, ty, tz, WAY_COLOR);
	}
	if (Vmap[tz][ty - 1][tx] > 0 && !visited[tz][ty - 1][tx - 1]) {//DOWN
		to.push(Node(tx, ty - 1, tz, _SET));
		draw_box_color(tx, ty - 1, tz, WAY_COLOR);
	}
	if (Vmap[tz][ty + 1][tx] > 0 && !visited[tz][ty + 1][tx + 1]) {//UP
		to.push(Node(tx, ty + 1, tz, _SET));
		draw_box_color(tx, ty + 1, tz, WAY_COLOR);
	}
	if (Vmap[tz - 1][ty][tx] > 0 && !visited[tz - 1][ty][tx]) {//FRONT
		to.push(Node(tx, ty, tz - 1, _SET));
		draw_box_color(tx, ty, tz - 1, WAY_COLOR);
	}
	if (Vmap[tz + 1][ty][tx] > 0 && !visited[tz + 1][ty][tx]) {//BACK
		to.push(Node(tx, ty, tz + 1, _SET));
		draw_box_color(tx, ty, tz + 1, WAY_COLOR);
	}

	//到当前点的距离是自己格上下左右四侧dis的最小值+自己格的vmap
	dis = min(Graph[tz][ty][tx + 1], Graph[tz][ty][tx - 1]);
	dis = min(dis, Graph[tz][ty - 1][tx]);
	dis = min(dis, Graph[tz][ty + 1][tx]);
	dis = min(dis, Graph[tz - 1][ty][tx]);
	dis = min(dis, Graph[tz + 1][ty][tx]);
	int face = 0;
	if (dis == Graph[tz][ty - 1][tx])face = _DOWN;
	if (dis == Graph[tz][ty + 1][tx])face = _UP;
	if (dis == Graph[tz][ty][tx - 1])face = _LEFT;
	if (dis == Graph[tz][ty][tx + 1])face = _RIGHT;
	if (dis == Graph[tz - 1][ty][tx])face = _BACK;
	if (dis == Graph[tz + 1][ty][tx])face = _FRONT;
	if (dis == INF)dis = 0;
	dis += Vmap[tz][ty][tx];
	Graph[tz][ty][tx] = dis;
	Face[tz][ty][tx] = face;
	Info_append("(" + std::to_string(tx) + "," + std::to_string(ty) + "," + std::to_string(tz) + "), " + std::to_string(Graph[tz][ty][tx]));
	if (tx == End_x && ty == End_y && tz == End_z)break;
}
````
路径绘制:
````C++
std::queue<Node> pathlist;
pathlist.push(Node(End_x, End_y, End_z, Face[End_z][End_y][End_x]));
int fx, fy, fz;
fx = End_x;
fy = End_y;
fz = End_z;
Info_clear();
std::string ret = std::to_string(Graph[End_z][End_y][End_x]);
Info_append("从源点到终点的距离为：" + ret);
Info_append("路径：");
Info_append("(" + std::to_string(fx) + "," + std::to_string(fy) + "," + std::to_string(fz) + ") ");
while (!pathlist.empty()) {
	int now = pathlist.front().from;
	draw_box_color(fx, fy, fz, KEYWAY_COLOR);
	switch (now)
	{
	case _DOWN:fy--; break;
	case _UP:fy++; break;
	case _LEFT:fx--; break;
	case _RIGHT:fx++; break;
	case _BACK:fz--; break;
	case _FRONT:fz++; break;
	case _SET:break;
	default:
		break;
	}
	pathlist.push(Node(fx, fy, fz, Face[fz][fy][fx]));
	Info_append("(" + std::to_string(fx) + "," + std::to_string(fy) + "," + std::to_string(fz) + ") ");
	pathlist.pop();
	if (fx == Start_x && fy == Start_y && fz == Start_z)break;
}
draw_box_color(Start_x, Start_y, Start_z, START_COLOR);
draw_box_color(End_x, End_y, End_z, END_COLOR);
CVMat_to_Pic(Gmap, IDC_STATIC_PIC);
Out_G = Graph;
Out_V = visited;
Out_F = Face;
GetDlgItem(IDC_BUTTON2)->EnableWindow(true);
GetDlgItem(IDC_BUTTON5)->EnableWindow(true);
GetDlgItem(IDC_BUTTON11)->EnableWindow(true);
return;
````
# 示例截图
1.一般结果
![一般结果](https://uy.wzznft.com/i/2025/07/02/gpxcnk.png)
2.添加墙后
![添加墙后](https://uy.wzznft.com/i/2025/07/02/gpxeqs.png)
# 未来优化
- 基于启发式搜索，GA，剪枝，模拟退火等思路对算法效率进一步优化
- 考虑有单向路“悬崖”的情况，借用哈密顿图/欧拉图进行处理
- 算法目前使用三维Vector和二维Mat实现存图读图，在实际应用中（例如嵌入式）可能存在空间不足的情况，可以考虑使通过循环利用优化空间
# 贡献与反馈
欢迎提交问题与反馈以助我们进一步优化。
邮箱1 [arbeiterklassen@163.com](https://arbeiterklassen@163.com/)
邮箱2 [2268602695@qq.com](2268602695@qq.com)

