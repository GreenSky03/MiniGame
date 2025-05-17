#include "pch.h"
#include "CGameMgr.h"
#include "CMain.h"

MYSQL Conn;                              // MySQL ���� ���� ����ü
MYSQL* ConnPtr = NULL;                  // MySQL �ڵ�
MYSQL_RES* Result;                      // ���� ������ ����� ��� ����ü ������
MYSQL_ROW Row;                  // ���� ������ ����� ���� ���� ������ ��� ����ü



void setColor(unsigned short text) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

void gotoxy(int x, int y)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void cursor() { //Ŀ�� �Ⱥ��̰�
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConsoleCusor;
	ConsoleCusor.bVisible = false;
	ConsoleCusor.dwSize = 1;
	SetConsoleCursorInfo(consoleHandle, &ConsoleCusor);
}

void MineSweeper() {
	CGameMgr::GetInst()->Init();
	while (true)
	{
		CGameMgr::GetInst()->Render();
		CGameMgr::GetInst()->Update();
		if (CGameMgr::GetInst()->Quit()) break;
	}
}

void SIGN_UP()
{

	///////////////////////////SQL ���� Ȯ��///////////////////////////

	MYSQL mysql;
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, "localhost", "root", "1q2w3e4r!", "MINIGAME", 3306, NULL, 0)) {
		cout << "error\n";
	}
	else {
		cout << "success\n";
	}

	mysql_init(&Conn); // MySQL ���� �ʱ�ȭ  

	///////////////////////////SQL ���� ����///////////////////////////

	ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "1q2w3e4r!", "MINIGAME", 3306, (char*)NULL, 0);

	// ���� ��� Ȯ��. null�� ��� ����
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
		return ;
	}

	///////////////////////////ȸ������///////////////////////////

	string UserName, UserId, UserPw;


	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// �ϴ� ���� ���
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	std::cout << "\n\n\n";
	cout << "                ## ##     ####    ## ##   ###  ##           ##  ###  ### ##   \n";
	cout << "               ##   ##     ##    ##   ##    ## ##           ##   ##   ##  ##  \n";
	cout << "               ####        ##    ##        # ## #           ##   ##   ##  ##  \n";
	cout << "                #####      ##    ##  ###   ## ##            ##   ##   ##  ##  \n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << "                   ###     ##    ##   ##   ##  ##           ##   ##   ## ##   \n";
	cout << "               ##   ##     ##    ##   ##   ##  ##           ##   ##   ##      \n";
	cout << "                ## ##     ####    ## ##   ###  ##            ## ##   ####     \n";
	cout << "\n\n\n\n";
	// ������� �����Ͽ� ���� ����� ������� ��Ÿ������ ��
	SetConsoleTextAttribute(hConsole, 15);


	cout << "�̸��� �Է����ּ��� >> ";
	cin >> UserName;
	while (1) {
		cout << "���̵� �Է����ּ��� >> ";
		cin >> UserId;
		////���̵� �ߺ� Ȯ��

		// ID �ߺ� üũ�� ���� SQL ���� ����
		string idCheckQuery = "SELECT ID FROM customer_table WHERE ID = '" + UserId + "'";

		// SQL ���� ����
		mysql_query(&Conn, idCheckQuery.c_str());

		// ����� ����
		MYSQL_RES* idCheckResult = mysql_store_result(&Conn);

		// ��� ���� ������ Ȯ���Ͽ� �ߺ� ���θ� �Ǵ�
		if (mysql_num_rows(idCheckResult) == 0) {
			// ��� ��Ʈ ����
			mysql_free_result(idCheckResult);

			// �ߺ��� ������ �ݺ��� ����
			break;
		}

		cout << "�̹� �����ϴ� ���̵� �Դϴ�!\n";
		mysql_free_result(idCheckResult);
	}

	cout << "��й�ȣ�� �Է����ּ���.";
	cin >> UserPw;

	cout << "������ �������� ȯ���մϴ�!!!\n";
	Sleep(3000);

	//���� �ο� �Ǻ� �� No�� ����
	int No = 0; // ������ ȸ���� ��ϵǸ� NO(ȸ����ȣ)�� ȸ���� �ľ�


	// �� ���̺��� ���ڵ� ������ �������� ���� SQL ���� ����
	string AccountCountQuery = "SELECT COUNT(*) FROM customer_table";
	//�� ���̺� �ִ� ��� Į���� �˻����ּ���

	// SQL ���� ����
	if (mysql_query(&Conn, AccountCountQuery.c_str()) != 0) {
		// SQL ���� ���� �� ���� �߻� �� ���� �޽��� ���
		fprintf(stderr, "SQL �� ���� ����: %s\n", mysql_error(&Conn));
	}
	else {
		// SQL ���� ������ ������ ��� ����� ��������
		MYSQL_RES* result = mysql_use_result(&Conn);

		// ��� �� ��������
		MYSQL_ROW row = mysql_fetch_row(result);

		// ��� ���� �����ϴ� ���
		if (row != NULL) {
			// ���ڵ� ������ ������ ��ȯ�Ͽ� No ������ ����
			No = atoi(row[0]); //atoi()���� ������ ��ȯ

			// ��� ��Ʈ ����
			mysql_free_result(result);
		}
	}
	//���� �ο� +1 =No   ex) �����̺� 5���� ���� �����ϸ� ���� ȸ�����ʹ� NO.6 ����
	No += 1;

	// ȸ�� ������ �����ͺ��̽��� �����ϱ� ���� SQL ���� ���� �� ����
	string insertQuery = "INSERT INTO `MINIGAME`.`customer_table` (`No`, `ID`, `Name`, `Password`) VALUES ('" + to_string(No) + "','" + UserId + "', '" + UserName + "', '" + UserPw + "')";

	if (mysql_query(&Conn, insertQuery.c_str()) != 0) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
	}
}

void LOGIN()
{
	string InputId, InputPw;
	int missing = 0;
	string MemberNo = "NULL";


	mysql_init(&Conn); // MySQL ���� �ʱ�ȭ

	// �����ͺ��̽��� ����
	ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "1q2w3e4r!", "MINIGAME", 3306, (char*)NULL, 0);
	// MySql DB�� �����ϱ� ���� ��
	//  MYSQL ����ü�� ���� �����ͷ� ��, ���� ���� �� ����ü�� �ʱ�ȭ ȣ��Ʈ�� / ����� �̸� / ��й�ȣ / ��Ű�� / ��Ʈ��ȣ / �����Ϳ� ���� 0 ��� C++���� NULL�� �ؼ�

	// ���� ��� Ȯ��. null�� ��� ����
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
		return;
	}

	system("cls");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//�ϴ� ���� ���
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	cout << "\n\n\n";
	cout << "                            ##        ##        ## ##    ####   ###   ##\n";
	cout << "                            ##      ##   ##   ##   ##     ##      ##  ##\n";
	cout << "                            ##      ##   ##   ##          ##     # ## ##\n";
	cout << "                            ##      ##   ##   ##  ###     ##     ## ##\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << "                            ##      ##   ##   ##   ##     ##     ##  ##\n";
	cout << "                            ##       ## ##     ##   ##    ##     ##  ##\n";
	cout << "                            ### ###   ##        ## ##    ####   ###  ##\n";

	SetConsoleTextAttribute(hConsole, 15);
	cout << "\n\n\n\n";


	while (1)
	{
		cout << "\n";
		cout << "���̵� �Է��ϼ��� >> ";
		cin >> InputId;
		cout << "��й�ȣ�� �Է��ϼ��� >> ";
		cin >> InputPw;

		// SQL ���� - customer_table���� ID�� �ִ� ��� Į���� �����ּ���
		string query = "SELECT * FROM customer_table WHERE ID = '" + InputId + "' AND password = '" + InputPw + "'";
		// SQL ������ �����ϰ� ���� ���� Ȯ��
		if (mysql_query(&Conn, query.c_str()) == 0) {
			// ����� ����
			MYSQL_RES* result = mysql_store_result(&Conn);

			// ����� ���� ���
			if (result) {
				// ù ��° ���� ������
				MYSQL_ROW row = mysql_fetch_row(result);

				// ���� �����ϸ� �α��� ����
				if (row) {
					// ��� ��Ʈ ����
					mysql_free_result(result);

					// �α����� ������� ������ �ٽ� �������� SQL ���� ����
					string whoLoginquery = "SELECT Name, NO FROM MINIGAME.customer_table WHERE ID = '" + InputId + "'";

					// SQL ������ �����ϰ� ���� ���� Ȯ��
					if (mysql_query(&Conn, whoLoginquery.c_str()) == 0) {
						// ����� ����
						MYSQL_RES* result = mysql_store_result(&Conn);

						// ����� ���� ���
						if (result != NULL) {
							// ù ��° ���� ������
							MYSQL_ROW row = mysql_fetch_row(result);

							// ���� �����ϸ� ȯ�� �޽��� ���
							if (row != NULL) {
								cout << "ȯ���մϴ�! " << row[0] << "��!\n"; //row[0]���� name�� ����Ǿ� ����
								MemberNo = row[1]; //row[1]���� No�� ����Ǿ� ����

								CMain::GetInst()->SetID(InputId);

								// 3�� ���
								Sleep(3000);

								break;

							}
						}

						// ��� ��Ʈ ����
						mysql_free_result(result);
					}
				}
				else {
					// ��й�ȣ ���� �ִ� 5�� ����
					missing += 1;
					cout << "��й�ȣ�� �����ʽ��ϴ�." << endl;
					cout << "���� ��й�ȣ" << missing << "ȸ �����Դϴ�." << endl;
					if (missing < 5) {
						continue;
					}
					cout << "�α��� 5ȸ ���н� �α����� ���ѵ˴ϴ�." << endl;

					//��� ��Ʈ ��ü
					mysql_free_result(result);

					//���α׷� ����
					cout << "\n";
					cout << "���� : �α��� 5ȸ ����" << endl;
					cout << "\n\n\n\n";
					exit(0);

				}
			}
		}
		else {
			// ���� ������ ���� �߻�
			cout << "SQL query error: " << mysql_error(&Conn);
		}
	}
	cout << "\n";
}

void END() 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// �ϴ� ���� ���
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
	cout << "           #### ##  ###  ##    ##     ###  ##  ##  ###           ##  ##    ## ##   ##  ###\n";
	cout << "           # ## ##   ##  ##     ##      ## ##  ##  ##            ##  ##   ##   ##  ##   ##\n";
	cout << "             ##      ##  ##   ## ##    # ## #  ## ##             ##  ##   ##   ##  ##   ##\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << "             ##      ## ###   ##  ##   ## ##   ## ##              ## ##   ##   ##  ##   ##\n";
	cout << "             ##      ##  ##   ## ###   ##  ##  ## ###              ##     ##   ##  ##   ##\n";
	cout << "             ##      ##  ##   ##  ##   ##  ##  ##  ##              ##     ##   ##  ##   ##\n";
	cout << "            ####    ###  ##  ###  ##  ###  ##  ##  ###             ##      ## ##    ## ##\n";
	cout << "\n\n\n\n";
	// ������� �����Ͽ� ���� ����� ������� ��Ÿ������ ��
	SetConsoleTextAttribute(hConsole, 15);
}

/////////////////////////��� �߰�///////////////////////////

void Record(int _score)
{
	int iScore;
	string Player_ID = CMain::GetInst()->GetID();

	iScore = _score;
	int record_No = 0; // ������ ȸ���� ��ϵǸ� NO(ȸ����ȣ)�� ȸ���� �ľ�

	// �� ���̺��� ���ڵ� ������ �������� ���� SQL ���� ����
	string CountQuery;
	switch (CMain::GetInst()->GetCurMenu())
	{
	case MENU_TYPE::MINESWEEPER:
		CountQuery = "SELECT COUNT(*) FROM minesweeper_game";
		break;
	case MENU_TYPE::TYPINGGAME:
		CountQuery = "SELECT COUNT(*) FROM typing_word_game";
		break;
	}
	//�� ���̺� �ִ� ��� Į���� �˻����ּ���

	// SQL ���� ����
	if (mysql_query(&Conn, CountQuery.c_str()) != 0) {
		// SQL ���� ���� �� ���� �߻� �� ���� �޽��� ���
		fprintf(stderr, "SQL �� ���� ����: %s\n", mysql_error(&Conn));
	}
	else {
		// SQL ���� ������ ������ ��� ����� ��������
		MYSQL_RES* result = mysql_use_result(&Conn);

		// ��� �� ��������
		MYSQL_ROW row = mysql_fetch_row(result);

		// ��� ���� �����ϴ� ���
		if (row != NULL) {
			// ���ڵ� ������ ������ ��ȯ�Ͽ� No ������ ����
			record_No = atoi(row[0]); //atoi()���� ������ ��ȯ

			// ��� ��Ʈ ����
			mysql_free_result(result);
		}
	}
	//���� �ο� +1 =No   ex) �����̺� 5���� ���� �����ϸ� ���� ȸ�����ʹ� NO.6 ����
	record_No += 2;

	string insertQuery;

	switch (CMain::GetInst()->GetCurMenu())
	{
	case MENU_TYPE::MINESWEEPER:
		insertQuery = "INSERT INTO `MINIGAME`.`minesweeper_game` (`No`, `Score`, `Player_ID`) VALUES ('" + to_string(record_No) + "','" + to_string(iScore) + "', '" + Player_ID + "')";
		break;
	case MENU_TYPE::TYPINGGAME:
		insertQuery = "INSERT INTO `MINIGAME`.`typing_word_game` (`No`, `Score`, `Player_ID`) VALUES ('" + to_string(record_No) + "','" + to_string(iScore) + "', '" + Player_ID + "')";
	}

	if (mysql_query(&Conn, insertQuery.c_str()) != 0) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
	}

	cout << "\n";
}

///////////////////////////���ھ� ���� Ȯ��///////////////////////////
void Check_Score()
{


	mysql_init(&Conn); // MySQL ���� �ʱ�ȭ

	// �����ͺ��̽��� ����
	ConnPtr = mysql_real_connect(&Conn, "localhost", "root", "1q2w3e4r!", "MINIGAME", 3306, (char*)NULL, 0);
	// MySql DB�� �����ϱ� ���� ��
	//  MYSQL ����ü�� ���� �����ͷ� ��, ���� ���� �� ����ü�� �ʱ�ȭ ȣ��Ʈ�� / ����� �̸� / ��й�ȣ / ��Ű�� / ��Ʈ��ȣ / �����Ϳ� ���� 0 ��� C++���� NULL�� �ؼ�

	// ���� ��� Ȯ��. null�� ��� ����
	if (ConnPtr == NULL) {
		fprintf(stderr, "Mysql query error:%s", mysql_error(&Conn));
		return;
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//�ϴ� ���� ���
	SetConsoleTextAttribute(hConsole, 14);
	cout << "\n\n\n";
	cout << "\t:::::::::      :::     ::::    ::: :::    ::: ::::::::::: ::::    :::  :::::::: \n";
	cout << "\t:+:    :+:   :+: :+:   :+:+:   :+: :+:   :+:      :+:     :+:+:   :+: :+:    :+: \n";
	cout << "\t+:+    +:+  +:+   +:+  :+:+:+  +:+ +:+  +:+       +:+     :+:+:+  +:+ +:+        \n";
	cout << "\t+#++:++#:  +#++:++#++: +#+ +:+ +#+ +#++:++        +#+     +#+ +:+ +#+ :#:        \n";
	cout << "\t+#+    +#+ +#+     +#+ +#+  +#+#+# +#+  +#+       +#+     +#+  +#+#+# +#+   +#+# \n";
	cout << "\t#+#    #+# #+#     #+# #+#   #+#+# #+#   #+#      #+#     #+#   #+#+# #+#    #+# \n";
	cout << "\t###    ### ###     ### ###    #### ###    ### ########### ###    ####  ########  \n";

	SetConsoleTextAttribute(hConsole, 15);
	cout << "\n\n\n\n";

	setColor(COLOR::BLUE);
	gotoxy(RESOLUTION.x / 4 - 9, RESOLUTION.y / 2 - 1);
	cout << "����ã�� ���� ��� " << endl;
	setColor(COLOR::WHITE);
	gotoxy(RESOLUTION.x / 4 - 13, RESOLUTION.y / 2);
	cout << "�� �� �� �� �� �� �� �� �� �� �� �� ��" << endl;

	// ���� ã�� ���̺� ��� ���� ��û
	const char* mineprintquery = "SELECT * FROM minesweeper_game ORDER BY Score ASC";

	int mine_stat;
	mine_stat = mysql_query(&Conn, mineprintquery);

	if (mine_stat != 0) {
		fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
		return ;
	}

	Result = mysql_store_result(&Conn); //MySQL���� ������ ������ ��� ������ Result�� ����
	gotoxy(RESOLUTION.x / 4 - 9, RESOLUTION.y / 2 + 1);
	cout << "��� ";
	cout << "��� ";
	cout << "���̵�" << endl;
	gotoxy(RESOLUTION.x / 4 - 13, RESOLUTION.y / 2 + 2);
	cout << "�� �� �� �� �� �� �� �� �� �� �� �� ��" << endl;
	int mine_rank = 1;


	// �ݺ��� ������ Row ������ ����Ͽ� ��� ���տ��� �� �྿ �����͸� ó���̶�, NULL�� ���� ������ ��� �ݺ�
	while ((Row = mysql_fetch_row(Result)) != NULL)
	{
		gotoxy(RESOLUTION.x / 4 - 9, RESOLUTION.y / 2 + mine_rank + 2);
		cout << mine_rank << "     ";
		cout << Row[0] << "  ";
		cout << Row[1] << endl;
		mine_rank++;
	}
	setColor(COLOR::MINT);
	gotoxy(RESOLUTION.x - 38, RESOLUTION.y / 2 - 1);
	cout << "Ÿ�ڰ��� ���� ���" << endl;
	setColor(COLOR::WHITE);
	gotoxy(RESOLUTION.x - 42, RESOLUTION.y / 2);
	cout << "�� �� �� �� �� �� �� �� �� �� �� �� ��" << endl;


	// Ÿ�� ���� ���̺� ��� ���� ��û
	const char* typingprintquery = "SELECT * FROM typing_word_game ORDER BY Score ASC";

	int type_stat;
	type_stat = mysql_query(&Conn, typingprintquery);

	if (type_stat != 0) {
		fprintf(stderr, "Mysql query error:%s\n", mysql_error(ConnPtr));
		return ;
	}

	Result = mysql_store_result(&Conn); //MySQL���� ������ ������ ��� ������ Result�� ����
	gotoxy(RESOLUTION.x - 38, RESOLUTION.y / 2 + 1);
	cout << "��� ";
	cout << "��� ";
	cout << "���̵�" << endl;
	gotoxy(RESOLUTION.x - 42, RESOLUTION.y / 2 + 2);
	cout << "�� �� �� �� �� �� �� �� �� �� �� �� ��" << endl;
	int typing_rank = 1;
	int Rank_Count = 0;

	// �ݺ��� ������ Row ������ ����Ͽ� ��� ���տ��� �� �྿ �����͸� ó���̶�, NULL�� ���� ������ ��� �ݺ�
	while ((Row = mysql_fetch_row(Result)) != NULL && typing_rank <= 5)
	{
		gotoxy(RESOLUTION.x - 38, RESOLUTION.y / 2 + typing_rank + 2);
		cout << typing_rank << "     ";
		cout << Row[0] << "  ";
		cout << Row[1] << endl;
		typing_rank++;
	}

	
	gotoxy(RESOLUTION.x / 2 - 20, 26);
	cout << "ESC or ENTER �� ���� �ڷ� �� �� �ֽ��ϴ�" << endl;
	char c;
	while (true) 
	{
		if (_kbhit()) 
		{
			c = _getch();
			if (c == ESC || c== ENTER) 
			{
				return;
			}
		}
	}
	

	mysql_free_result(Result);// MySQL C API���� ����� �޸𸮸� �����ϴ� �Լ�
	mysql_close(ConnPtr); // MySQL �����ͺ��̽� ������ �ݴ� �Լ�
}
// ������ ���
void devsINFO()
{

	cout << "���������������������������������������������������������������������������������������������������";
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "���������������������������������������������������������������������������������������������������" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "��                                                                                                ��" << endl;
	cout << "���������������������������������������������������������������������������������������������������";
	gotoxy(0, 1);
	setColor(MINT);
	cout << "\t  ::::    ::::     :::     :::::::::  ::::::::::     :::::::::  :::   ::: " << endl;
	cout << "\t  +:+: :+:+:+    :+: :+:   :+:    :+: :+:            :+:    :+: :+:   :+: " << endl;
	cout << "\t +:+ +:+:+ +:+  +:+   +:+  +:+    +:+ +:+            +:+    +:+  +:+ +:+  " << endl;
	cout << "\t +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#       +#++:++#+    +#++:   " << endl;
	cout << "\t +#+       +#+ +#+     +#+ +#+    +#+ +#+            +#+    +#+    +#+    " << endl;
	cout << "\t #+#       #+# #+#     #+# #+#    #+# #+#            #+#    #+#    #+#    " << endl;
	cout << "\t ###       ### ###     ### #########  ##########     #########     ###    " << endl;
	setColor(YELLOW);
	gotoxy(0, 8);
	cout << "\t      :::::::::  :::::::::: :::     ::: :::::::::: :::::::::   ::::::::  ::::::::::  " << endl
		<< "\t      +:+    :+: :+:        :+:     :+: :+:        :+:    :+: :+:    :+: :+:        " << endl
		<< "\t      +:+    +:+ +:+        +:+     +:+ +:+        +:+    +:+ +:+        +:+        " << endl
		<< "\t      #++:++#:   +#++:++#   +#+     +:+ +#++:++#   +#++:++#:  +#++:++#++ +#++:++#   " << endl
		<< "\t      +#+    +#+ +#+         +#+   +#+  +#+        +#+    +#+        +#+ +#+        " << endl
		<< "\t      #+#    #+# #+#          #+#+#+#   #+#        #+#    #+# #+#    #+# #+#        " << endl
		<< "\t      ###    ### ##########     ###     ########## ###    ###  ########  ########## ";
	setColor(BLUE);
	gotoxy(0, 17);
	cout << "\t\t\t   ��ǻ�ͼ���Ʈ�����а� |   ���ؿ�   |  20102112" << endl;
	cout << "\t\t\t  �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��" << endl;
	cout << "\t\t\t   ��ǻ�ͼ���Ʈ�����а� |   �輺ö   |  20102049" << endl;
	cout << "\t\t\t  �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��" << endl;
	cout << "\t\t\t   ��ǻ�ͼ���Ʈ�����а� |   ���ؼ�   |  20102078" << endl;
	cout << "\t\t\t  �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��" << endl;
	cout << "\t\t\t   ��ǻ�ͼ���Ʈ�����а� |   ������   |  20102106" << endl;
	cout << "\t\t\t  �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� �� ��" << endl;
	cout << "\t\t\t   ��ǻ�ͼ���Ʈ�����а� |  �庣���  |  22102197" << endl << endl;
	setColor(WHITE);

	
	cout << "\t\t\t       ESC or ENTER �� ���� �ڷ� �� �� �ֽ��ϴ�";
	char c;
	while (true)
	{
		if (_kbhit())
		{
			c = _getch();
			if (c == ESC || c == ENTER)
			{
				return;
			}
		}
	}


}