#include "pch.h"

#include "CMain.h"

#pragma comment(lib, "libmySQL.lib")




int main() {

    ///////////////////////////SQL ���� Ȯ��///////////////////////////
    MYSQL mysql;
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql, "localhost", "root", "1q2w3e4r!", "MINIGAME", 3306, NULL, 0)) {
        cout << "error\n";
    }
    else {
        cout << "success\n";
    }


    cursor();
    system("mode con: cols=100 lines=30 | title �̴ϰ���õ��");
    PlaySound(TEXT("main.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    // ���콺 Ŭ�� �� �巡�׷� ���� ���� ����
    DWORD consoleModePrev;
    HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
    GetConsoleMode(handle, &consoleModePrev);
    SetConsoleMode(handle, consoleModePrev & ~ENABLE_QUICK_EDIT_MODE);

    CMain::GetInst()->Init();
    while (true) {
        CMain::GetInst()->Render();
        CMain::GetInst()->Update();
    }
}