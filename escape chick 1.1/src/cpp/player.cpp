#include "myheader.h"

bool player_move(key_msg& keyMsg) {
    if (keyMsg.key == 'A' && maze[player.x][player.y - 1] != Wall) {         // ����a������߲���ǽ
        erase(player.y * 14 + 150, player.x * 14 + 80);                      // ����ԭλ�����ͼƬ
        player.y--;                                                          // ����������
    } else if (keyMsg.key == 'D' && maze[player.x][player.y + 1] != Wall) {  // ����d�����ұ߲���ǽ
        erase(player.y * 14 + 150, player.x * 14 + 80);
        player.y++;
    } else if (keyMsg.key == 'W' && maze[player.x - 1][player.y] != Wall) {  // ����w�����Ϸ�����ǽ
        erase(player.y * 14 + 150, player.x * 14 + 80);
        player.x--;
    } else if (keyMsg.key == 'S' && maze[player.x + 1][player.y] != Wall) {  // ����s�����·�����ǽ
        erase(player.y * 14 + 150, player.x * 14 + 80);
        player.x++;
    } else  // ��֪�������Ϲ������ʲô��ֵĶ���
        return false;
    return true;
}
