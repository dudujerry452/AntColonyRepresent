-------------------------------------------------------------------
本演示程序基于Qt6.6开发。
-------------------------------------------------------------------


-------------------------------------------------------------------
使用方法：
-------------------------------------------------------------------
模拟开始前的操作：
0.界面：鼠标左键可拖动界面，滚轮可缩放界面
1.添加节点：左键空白区域，此时节点个数、蚂蚁数目会自动更新为节点数量
2.设置参数：左侧输入栏中填入数字，可以修改蚂蚁数目但不能修改节点数目
3.开始模拟：左侧“开始计算”按钮

模拟开始后，左侧“开始计算”按钮变为“进行下一步”按钮：
1.下一次迭代：左侧“进行下一步“按钮
2.自动运行：点击“自动”按钮以自动运行
3.速度：修改”速度“空白栏以修改运行速度（倍数，默认为1）

运行过程：
圆圈代表节点，方框代表蚂蚁。
运行时圆圈之间会出现蓝色线条和红色线条。蓝色线条的透明度代表费洛蒙浓度，出现后逐渐消失的红色线条代表着本轮迭代到达终点时的最优路径，即本轮费洛蒙浓度增加的路径。

注意的问题：
开始模拟后没有办法重置节点和进度，如需请重启程序。
开始模拟后请不要修改蚂蚁数量m，其余均可修改。
不要向输入栏中输入非数字字符。

-------------------------------------------------------------------

-------------------------------------------------------------------
2023.11.27 zzy

enjoy the algorithm!
-------------------------------------------------------------------


