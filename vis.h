#pragma once
void vis(bool firstControl) {
	cout << "按F1更改控制方法" << endl;
	cout << "***************当前控制方法******************" << endl;
	if (firstControl)
	{
		cout << "W		前进" << endl;
		cout << "S		后退" << endl;
		cout << "A		左移" << endl;
		cout << "D		右移" << endl;
		cout << "Q		下降" << endl;
		cout << "E		上升" << endl;
	}
	else
	{
		cout << "W		上升" << endl;
		cout << "S		下降" << endl;
		cout << "A		左移" << endl;
		cout << "D		右移" << endl;
		cout << "Q		前进" << endl;
		cout << "E		后退" << endl;
	}

	cout << "2		移动减速" << endl;
	cout << "3		移动加速" << endl;
	cout << "4		转向减速" << endl;
	cout << "5		转向加速" << endl;
	cout << "通过英文状态下的空格改变朝向模式（默认队列朝向）" << endl;
	cout << "中心朝向下支持点击选中leader以及移动，支持随时leader更替(这可能就是生活吧=-=)" << endl;
	cout << "转向控制是以leader当前朝向为准" << endl;
	cout << "***************leader移动******************" << endl;
	cout << "up			前进" << endl;
	cout << "down		后退" << endl;
	cout << "left		左转" << endl;
	cout << "right		右转" << endl;
	cout << "*********************************\n\n\n" << endl;
}
