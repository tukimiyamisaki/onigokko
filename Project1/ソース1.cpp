#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>
#include <chrono>



using namespace std;

const int WIDTH = 20;   // ���H�̕�
const int HEIGHT = 20;  // ���H�̍���

// ���H��\������֐�
void printMaze(const vector<vector<char>>& maze) {
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            cout << maze[i][j] << " ";
        }
        cout << endl;
    }
}

// �v���C���[�̈ʒu���X�V����֐�
bool movePlayer(int& playerX, int& playerY, char move, const vector<vector<char>>& maze) {
    int newX = playerX, newY = playerY;

    switch (move) {
    case 'w': newX--; break;  // ��
    case 's': newX++; break;  // ��
    case 'a': newY--; break;  // ��
    case 'd': newY++; break;  // �E
    default: return false;
    }

    // �͈͊O�`�F�b�N
    if (newX < 0 || newX >= HEIGHT || newY < 0 || newY >= WIDTH) {
        return false;
    }

    // �ړ��悪�ǂłȂ����`�F�b�N
    if (maze[newX][newY] == '#') {
        return false;
    }

    // �v���C���[�̈ʒu���X�V
    playerX = newX;
    playerY = newY;

    return true;
}

// �G���v���C���[�Ɍ������ď\�������Ɉړ�����֐�
void moveEnemies(vector<pair<int, int>>& enemies, int playerX, int playerY, const vector<vector<char>>& maze) {
    for (auto& enemy : enemies) {
        int enemyX = enemy.first;
        int enemyY = enemy.second;

        // ���������Ɉړ����邩�A���������Ɉړ����邩�����߂�
        if (abs(enemyX - playerX) > abs(enemyY - playerY)) {
            // �v���C���[���c�����ɉ����ꍇ�A�㉺�Ɉړ�
            if (enemyX < playerX && maze[enemyX + 1][enemyY] != '#') {
                enemyX++;  // ���Ɉړ�
            }
            else if (enemyX > playerX && maze[enemyX - 1][enemyY] != '#') {
                enemyX--;  // ��Ɉړ�
            }
        }
        else {
            // �v���C���[���������ɉ����ꍇ�A���E�Ɉړ�
            if (enemyY < playerY && maze[enemyX][enemyY + 1] != '#') {
                enemyY++;  // �E�Ɉړ�
            }
            else if (enemyY > playerY && maze[enemyX][enemyY - 1] != '#') {
                enemyY--;  // ���Ɉړ�
            }
        }

        enemy = { enemyX, enemyY };
    }
}

// �v���C���[�ƓG���ڐG���Ă��邩�m�F����֐�
bool isGameOver(int playerX, int playerY, const vector<pair<int, int>>& enemies) {
    for (const auto& enemy : enemies) {
        if (enemy.first == playerX && enemy.second == playerY) {
            return true;
        }
    }
    return false;
}

// �G�������_���ɔz�u����֐�
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

// �v���C���[�ƃ����_���ȓG�̈ʒu�����ւ���֐�
void swapPlayerWithRandomEnemy(int& playerX, int& playerY, vector<pair<int, int>>& enemies) {
    // �����_���ɓG��I��
    int enemyIndex = rand() % enemies.size();

    // �I�΂ꂽ�G�ƃv���C���[�̈ʒu������
    swap(playerX, enemies[enemyIndex].first);
    swap(playerY, enemies[enemyIndex].second);
}

int main() {
    srand(time(0));

    // ���H�̏�����
    vector<vector<char>> maze(HEIGHT, vector<char>(WIDTH, ' '));
    maze[0][0] = 'S';  // �X�^�[�g�ʒu
    maze[HEIGHT - 1][WIDTH - 1] = 'G';  // �S�[���ʒu

    // �ǂ̔z�u (�ȈՓI�ȃ����_����)
    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j) {
            if (rand() % 5 == 0 && maze[i][j] == ' ') {
                maze[i][j] = '#';  // ��
            }
        }
    }


    // �G�̏����ʒu (3��) - �����_���z�u
    vector<pair<int, int>> enemies(7);
    placeEnemiesRandomly(enemies, maze);

    // �v���C���[�̏����ʒu
    int playerX = 0, playerY = 0;

    // �Q�[���J�n
    bool swapped = false;  // �v���C���[�ƓG�̈ʒu������1�x����
    while (true) {
        // ���H��\��
        vector<vector<char>> displayMaze = maze;
        displayMaze[playerX][playerY] = 'P';  // �v���C���[�̈ʒu
        for (const auto& enemy : enemies) {
            displayMaze[enemy.first][enemy.second] = 'E';  // �G�̈ʒu
        }
        

        printMaze(displayMaze);
        std::cout << std::flush << std::endl;

        // �Q�[���I�[�o�[�`�F�b�N
        if (isGameOver(playerX, playerY, enemies)) {
            cout << "�Q�[���I�[�o�[! �G�ɕ߂܂��Ă��܂��܂���!" << endl;
            break;
        }

        // �S�[���`�F�b�N
        if (playerX == HEIGHT - 1 && playerY == WIDTH - 1) {
            cout << "���߂łƂ��������܂�! �S�[���ɓ��B���܂���!" << endl;
            break;
        }

        // �v���C���[�̓���
        char move;
        cout << "�ړ���������������͂��Ă������� (w: ��, s: ��, a: ��, d: �E): ";
        cin >> move;

        // Q�{�^���ňʒu������ (1�x����)
        if (move == 'q' || move == 'p') {
            if (!swapped) {
                swapPlayerWithRandomEnemy(playerX, playerY, enemies);
                swapped = true;  // 1�x�̂݌���������
                cout << "�v���C���[�ƓG�̈ʒu�����ւ��܂���!" << endl;
            }
            else {
                cout << "�ʒu�����͈�x�����ł��B" << endl;
            }
        }
        else if (!movePlayer(playerX, playerY, move, maze)) {
            cout << "�����Ȉړ��ł��B������x�����Ă��������B" << endl;
        }


        // �G���ړ�������
        moveEnemies(enemies, playerX, playerY, maze);



       

    }
 
    return 0;
}