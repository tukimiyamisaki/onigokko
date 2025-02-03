#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>
#include <chrono>



using namespace std;

const int WIDTH = 20;   // 迷路の幅
const int HEIGHT = 20;  // 迷路の高さ

// 迷路を表示する関数
void printMaze(const vector<vector<char>>& maze) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
}

// プレイヤーの位置を更新する関数
bool movePlayer(int& playerX, int& playerY, char move, const vector<vector<char>>& maze) {
    int newX = playerX, newY = playerY;

    switch (move) {
    case 'w': newX--; break;  // 上
    case 's': newX++; break;  // 下
    case 'a': newY--; break;  // 左
    case 'd': newY++; break;  // 右
    default: return false;
    }

    // 範囲外チェック
    if (newX < 0 || newX >= HEIGHT || newY < 0 || newY >= WIDTH) {
        return false;
    }

    // 移動先が壁でないかチェック
    if (maze[newX][newY] == '#') {
        return false;
    }

    // プレイヤーの位置を更新
    playerX = newX;
    playerY = newY;

    return true;
}

// 敵がプレイヤーに向かって十字方向に移動する関数
void moveEnemies(vector<pair<int, int>>& enemies, int playerX, int playerY, const vector<vector<char>>& maze) {
    for (auto& enemy : enemies) {
        int enemyX = enemy.first;
        int enemyY = enemy.second;

        // 水平方向に移動するか、垂直方向に移動するかを決める
        if (abs(enemyX - playerX) > abs(enemyY - playerY)) {
            // プレイヤーが縦方向に遠い場合、上下に移動
            if (enemyX < playerX && maze[enemyX + 1][enemyY] != '#') {
                enemyX++;  // 下に移動
            }
            else if (enemyX > playerX && maze[enemyX - 1][enemyY] != '#') {
                enemyX--;  // 上に移動
            }
        }
        else {
            // プレイヤーが横方向に遠い場合、左右に移動
            if (enemyY < playerY && maze[enemyX][enemyY + 1] != '#') {
                enemyY++;  // 右に移動
            }
            else if (enemyY > playerY && maze[enemyX][enemyY - 1] != '#') {
                enemyY--;  // 左に移動
            }
        }

        enemy = { enemyX, enemyY };
    }
}

// プレイヤーと敵が接触しているか確認する関数
bool isGameOver(int playerX, int playerY, const vector<pair<int, int>>& enemies) {
    for (const auto& enemy : enemies) {
        if (enemy.first == playerX && enemy.second == playerY) {
            return true;
        }
    }
    return false;
}

// 敵をランダムに配置する関数
void placeEnemiesRandomly(vector<pair<int, int>>& enemies, const vector<vector<char>>& maze) {
    for (auto& enemy : enemies) {
        int x, y;
        do {
            x = rand() % HEIGHT;
            y = rand() % WIDTH;
        } while (maze[x][y] == '#' || (x == 0 && y == 0) || (x == HEIGHT - 1 && y == WIDTH - 1));
        enemy = { x, y };
    }
}

// プレイヤーとランダムな敵の位置を入れ替える関数
void swapPlayerWithRandomEnemy(int& playerX, int& playerY, vector<pair<int, int>>& enemies) {
    // ランダムに敵を選択
    int enemyIndex = rand() % enemies.size();

    // 選ばれた敵とプレイヤーの位置を交換
    swap(playerX, enemies[enemyIndex].first);
    swap(playerY, enemies[enemyIndex].second);
}

int main() {
    srand(time(0));

    // 迷路の初期化
    vector<vector<char>> maze(HEIGHT, vector<char>(WIDTH, ' '));
    maze[0][0] = 'S';  // スタート位置
    maze[HEIGHT - 1][WIDTH - 1] = 'G';  // ゴール位置

    // 壁の配置 (簡易的なランダム壁)
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (rand() % 5 == 0 && maze[i][j] == ' ') {
                maze[i][j] = '#';  // 壁
            }
        }
    }


    // 敵の初期位置 (3体) - ランダム配置
    vector<pair<int, int>> enemies(7);
    placeEnemiesRandomly(enemies, maze);

    // プレイヤーの初期位置
    int playerX = 0, playerY = 0;

    // ゲーム開始
    bool swapped = false;  // プレイヤーと敵の位置交換は1度だけ
    while (true) {
        // 迷路を表示
        vector<vector<char>> displayMaze = maze;
        displayMaze[playerX][playerY] = 'P';  // プレイヤーの位置
        for (const auto& enemy : enemies) {
            displayMaze[enemy.first][enemy.second] = 'E';  // 敵の位置
        }
        

        printMaze(displayMaze);
        std::cout << std::flush << std::endl;

        // ゲームオーバーチェック
        if (isGameOver(playerX, playerY, enemies)) {
            cout << "ゲームオーバー! 敵に捕まってしまいました!" << endl;
            break;
        }

        // ゴールチェック
        if (playerX == HEIGHT - 1 && playerY == WIDTH - 1) {
            cout << "おめでとうございます! ゴールに到達しました!" << endl;
            break;
        }

        // プレイヤーの入力
        char move;
        cout << "移動したい方向を入力してください (w: 上, s: 下, a: 左, d: 右): ";
        cin >> move;

        // Qボタンで位置を交換 (1度だけ)
        if (move == 'q' || move == 'p') {
            if (!swapped) {
                swapPlayerWithRandomEnemy(playerX, playerY, enemies);
                swapped = true;  // 1度のみ交換を許可
                cout << "プレイヤーと敵の位置を入れ替えました!" << endl;
            }
            else {
                cout << "位置交換は一度だけです。" << endl;
            }
        }
        else if (!movePlayer(playerX, playerY, move, maze)) {
            cout << "無効な移動です。もう一度試してください。" << endl;
        }


        // 敵を移動させる
        moveEnemies(enemies, playerX, playerY, maze);



       

    }
 
    return 0;
}