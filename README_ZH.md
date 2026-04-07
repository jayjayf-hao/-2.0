# snakeII 中文说明（游戏内保留英文）

## 你会在游戏里看到的英文
- snakeII：游戏名
- State：当前状态
- Score：分数
- Speed：速度
- Input stats(last frame)：上一帧输入统计
- raw：读取到的原始按键数量
- illegal turn filtered：被过滤的非法反向输入数量
- invalid key ignored：被忽略的无效按键数量
- Controls：按键说明
- Message：状态消息
- Hint：提示

## 状态英文对照
- MENU：主菜单
- RUNNING：游戏进行中
- PAUSED：已暂停
- GAME_OVER：游戏结束
- EXIT：退出

## 操作英文对照
- WASD / Arrow : Move：WASD 或方向键移动
- P : Pause/Resume：暂停/继续
- K : Save, L : Load：保存/读取
- +/- : Speed up/down：加速/减速
- M : Back to menu：返回菜单
- Q : Exit：退出游戏

## 说明
- 现在“按空格或方向键开始”等提示只会出现在右侧信息面板，不会再穿过游戏区域。

## 难度怎么切换
- 手动切换：按 `+` 提高难度（速度更快），按 `-` 降低难度（速度更慢）。
- 三档速度：LOW / MEDIUM / HIGH。
- 当前速度会显示在右侧 `Speed` 一行。
- 动态变速：吃到食物、分数上涨后，系统会自动按分数提升速度档位（属于动态难度）。
