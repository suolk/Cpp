#include "myheader.h"

bool player_move(key_msg& keyMsg) {
    if (keyMsg.key == 'A' && maze[player.x][player.y - 1] != Wall) {         // 键入a，且左边不是墙
        erase(player.y * 14 + 150, player.x * 14 + 80);                      // 擦除原位置玩家图片
        player.y--;                                                          // 玩家坐标更改
    } else if (keyMsg.key == 'D' && maze[player.x][player.y + 1] != Wall) {  // 键入d，且右边不是墙
        erase(player.y * 14 + 150, player.x * 14 + 80);
        player.y++;
    } else if (keyMsg.key == 'W' && maze[player.x - 1][player.y] != Wall) {  // 键入w，且上方不是墙
        erase(player.y * 14 + 150, player.x * 14 + 80);
        player.x--;
    } else if (keyMsg.key == 'S' && maze[player.x + 1][player.y] != Wall) {  // 键入s，且下方不是墙
        erase(player.y * 14 + 150, player.x * 14 + 80);
        player.x++;
    } else  // 不知道玩家又瞎键入了什么奇怪的东西
        return false;
    return true;
}
