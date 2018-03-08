#include<Windows.h>
#include"resource.h"
//#include<stdlib.h>
#include<stdio.h>
#define UWM_MSG (WM_USER+100)


BOOL CALLBACK _ProcWinMain(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK _ProWin(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
int _Enable(int i);

int RadioBox=0;
HWND hWndStatic;
SYSTEMTIME lpSystemTime;
SYSTEMTIME lpEndTime;
char c_Time[100];
HINSTANCE g_hInstance;
char stTextTick[100];
double flCloseTime;
double dbStart;
HWND g_hWnd;
int Num = 0;
char st_End[30];
int Index;


int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	g_hInstance = hInstance;
	DialogBoxParamA(hInstance, MAKEINTRESOURCEA(IDD_DIALOG1), NULL,_ProcWinMain , NULL);
	ExitProcess(NULL);

}

BOOL CALLBACK _ProcWinMain(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
		
	case WM_INITDIALOG:
	{
		g_hWnd = hWnd;
		SendMessageA(hWnd, WM_SETICON, IDI_ICON1, (LPARAM)LoadIconA(g_hInstance, MAKEINTRESOURCE(IDI_ICON1)));
		SetClassLongA(hWnd, GCL_HICONSM, (long)LoadIconA(g_hInstance, MAKEINTRESOURCE(IDI_ICON1)));
		GetLocalTime(&lpSystemTime);
		wsprintfA(c_Time, "%4d年%2d月%2d日%2d:%2d", lpSystemTime.wYear, lpSystemTime.wMonth,lpSystemTime.wDay,lpSystemTime.wHour,lpSystemTime.wMinute);
		hWndStatic = GetDlgItem(hWnd, IDC_STATICTIME);
		SetWindowTextA(hWndStatic,c_Time);
		SetTimer(hWnd, 1, 1500, NULL);

		CheckDlgButton(hWnd, IDC_RADIO1, BST_CHECKED);
		char stTemp[] = "2016";
		wsprintf(stTemp, "%d", lpSystemTime.wYear);
		SendDlgItemMessageA(hWnd, IDC_EDITY,WM_SETTEXT, 0,(LPARAM)stTemp );
		wsprintf(stTemp, "%d", lpSystemTime.wMonth);
		SendDlgItemMessageA(hWnd, IDC_EDITM, WM_SETTEXT, 0, (LPARAM)stTemp);
		wsprintf(stTemp, "%d", lpSystemTime.wDay);
		SendDlgItemMessageA(hWnd, IDC_EDITD, WM_SETTEXT, 0, (LPARAM)stTemp);
		wsprintf(stTemp, "%d", lpSystemTime.wHour);
		SendDlgItemMessageA(hWnd, IDC_EDITH, WM_SETTEXT, 0, (LPARAM)stTemp);
		wsprintf(stTemp, "%d", lpSystemTime.wMinute);
		SendDlgItemMessageA(hWnd, IDC_EDITMI, WM_SETTEXT, 0, (LPARAM)stTemp);
		
		_Enable(RadioBox);

		
		EnableWindow(GetDlgItem(hWnd, IDCANCEL), FALSE);
		
		sprintf_s(stTextTick, 100, "本次已启动:%0.2lf小时", ((double)GetTickCount() / (1000.0 * 60.0 * 60.0)));
		SendDlgItemMessageA(hWnd, IDC_STATICTICKCOUNT, WM_SETTEXT,NULL , (LPARAM)stTextTick);

		char szText1[] = "小时";
		char szText2[] = "分钟";
		SendDlgItemMessageA(hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)szText1);
		SendDlgItemMessageA(hWnd, IDC_COMBO1, CB_ADDSTRING, 0, (LPARAM)szText2);
	
		SendDlgItemMessageA(hWnd, IDC_COMBO1, CB_SETCURSEL, 0, 0);

		NOTIFYICONDATA nti;
		nti.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		nti.hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_ICON1));
		nti.hWnd = hWnd;
		nti.uCallbackMessage= UWM_MSG;
		Shell_NotifyIcon(NIM_ADD, &nti);

		break;
		}
	case WM_TIMER:
	{
		if (wParam == 1)
		{
			GetLocalTime(&lpSystemTime);
			wsprintfA(c_Time, "%4d年%2d月%2d日%2d:%2d", lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute);
			GetDlgItem(hWnd, IDC_STATICTIME);
			SetWindowTextA(hWndStatic, c_Time);
			sprintf_s(stTextTick, 100, "本次已启动:%0.1f小时", (GetTickCount() / (3600000.0)));
			SendDlgItemMessageA(hWnd, IDC_STATICTICKCOUNT, WM_SETTEXT, NULL, (LPARAM)stTextTick);
		}
		else if (wParam == 2&&flCloseTime!=0)
		{
			if ((GetTickCount() - dbStart) == flCloseTime)
			{
				KillTimer(hWnd, 2);
				flCloseTime++;
				
				DialogBoxParamA(NULL, MAKEINTRESOURCEA(IDD_DIALOG2), hWnd, _ProWin, NULL);
			
			}
			

		}
		else if (wParam == 10)
		{
			GetLocalTime(&lpSystemTime);
			if ((lpSystemTime.wYear==lpEndTime.wYear) &&(lpSystemTime.wMonth==lpEndTime.wMonth) && (lpSystemTime.wDay==lpEndTime.wDay)&&(lpSystemTime.wHour==lpEndTime.wHour)&&(lpSystemTime.wMinute==lpEndTime.wMinute))
			{
				DialogBoxParamA(NULL, MAKEINTRESOURCEA(IDD_DIALOG2), hWnd, _ProWin, NULL);
			}
			break;
		}
	
		break;
	}
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == ID_OK)
		{
			if (RadioBox == 0)
			{
				char stCloseTime[4];
				GetDlgItemText(hWnd, IDC_EDIT1, stCloseTime, 4);
				flCloseTime = atof(stCloseTime);
				if (flCloseTime <= 0)
				{
					MessageBox(hWnd, "输入错误，请重新输入", "错误", MB_OK);
				}
				else
				{
					EnableWindow(GetDlgItem(hWnd, IDC_RADIO1), FALSE);
					EnableWindow(GetDlgItem(hWnd, IDC_RADIO2), FALSE);
					Index = SendDlgItemMessageA(hWnd, IDC_COMBO1, CB_GETCURSEL, 0, 0);
					EnableWindow(GetDlgItem(hWnd, IDC_EDIT1), FALSE);
					EnableWindow(GetDlgItem(hWnd, IDC_COMBO1), FALSE);
					dbStart = GetTickCount();
					char stTextCaption[20];
					if (Index == 0)
					{
						sprintf_s(stTextCaption, 20, "%0.1f小时后关机", flCloseTime);
						SetWindowText(hWnd, stTextCaption);
						flCloseTime = flCloseTime * 60 * 60 * 1000;
					}
					else if (Index == 1)
					{
						sprintf_s(stTextCaption, 20, "%0.1f分钟后关机", flCloseTime);
						SetWindowText(hWnd, stTextCaption);
						flCloseTime = flCloseTime * 60 * 1000;
					}

					SetTimer(hWnd, 2, 1000, NULL);
					EnableWindow(GetDlgItem(hWnd, ID_OK), FALSE);
					EnableWindow(GetDlgItem(hWnd, ID_CANCEL), TRUE);
				}
			}
			else if (RadioBox == 1)
			{
				
				char stTemp[50] ;
				GetDlgItemTextA(hWnd, IDC_EDITY, stTemp, 5);
				lpEndTime.wYear = atoi(stTemp);
				GetDlgItemTextA(hWnd, IDC_EDITM, stTemp, 3);
				lpEndTime.wMonth = atoi(stTemp);
				GetDlgItemTextA(hWnd, IDC_EDITD, stTemp, 3);
				lpEndTime.wDay = atoi(stTemp);
				GetDlgItemTextA(hWnd, IDC_EDITH, stTemp, 3);
				lpEndTime.wHour = atoi(stTemp);
				GetDlgItemTextA(hWnd, IDC_EDITMI, stTemp, 3);
				lpEndTime.wMinute = atoi(stTemp);

				if (lpEndTime.wYear >= lpSystemTime.wYear&&lpEndTime.wMonth <= 12 && lpEndTime.wDay <= 31 && lpEndTime.wHour <= 24 && lpEndTime.wMinute <= 60)
				{
					if (lpEndTime.wYear == lpSystemTime.wYear&&lpEndTime.wMonth== lpSystemTime.wMonth&&lpEndTime.wDay == lpSystemTime.wDay&&lpEndTime.wHour== lpSystemTime.wHour&&lpEndTime.wMinute == lpSystemTime.wMinute)
					{
						MessageBox(hWnd, "您直接关机就好了", "错误，请重新输入", MB_OK);
						break;
					}
					else
					{
						EnableWindow(GetDlgItem(hWnd, IDC_RADIO1), FALSE);
						EnableWindow(GetDlgItem(hWnd, IDC_RADIO2), FALSE);
						EnableWindow(GetDlgItem(g_hWnd, IDC_EDITM), FALSE);
						EnableWindow(GetDlgItem(g_hWnd, IDC_EDITY), FALSE);
						EnableWindow(GetDlgItem(g_hWnd, IDC_EDITD), FALSE);
						EnableWindow(GetDlgItem(g_hWnd, IDC_EDITH), FALSE);
						EnableWindow(GetDlgItem(g_hWnd, IDC_EDITMI), FALSE);
						EnableWindow(GetDlgItem(hWnd, ID_OK), FALSE);
						EnableWindow(GetDlgItem(hWnd, ID_CANCEL), TRUE);

						wsprintf(stTemp, "您已设置%4d年%2d月%2d日%2d小时%2d分钟为关机时间", lpEndTime.wYear, lpEndTime.wMonth, lpEndTime.wDay, lpEndTime.wHour, lpEndTime.wMinute);
						SetWindowTextA(hWnd, stTemp);

						SetTimer(hWnd, 10, 6000, NULL);
						
					}
					
				
				}
				else
				{
					MessageBox(hWnd, "输入错误，请重新输入", "错误", MB_OK);
				}
				
				break;
				
			}
			
		}
			
		
		
		else if (LOWORD(wParam) == ID_CANCEL)
			{
				KillTimer(hWnd, 2);
				KillTimer(hWnd, 10);
				EnableWindow(GetDlgItem(hWnd, ID_OK), TRUE);
				EnableWindow(GetDlgItem(hWnd, ID_CANCEL), FALSE);
				flCloseTime = 0;
				EnableWindow(GetDlgItem(hWnd, IDC_RADIO1), TRUE);
				EnableWindow(GetDlgItem(hWnd, IDC_RADIO2), TRUE);
				
				
				_Enable(RadioBox);
				SetWindowText(hWnd, "定时关机");		
			}
		else if (LOWORD(wParam) == IDC_RADIO1)
		{
			RadioBox = 0;
			_Enable(0);
		}
		else if (LOWORD(wParam) == IDC_RADIO2)
		{
			RadioBox = 1;
			_Enable(1);
		}

		break;
	}
	case WM_CLOSE:
		KillTimer(hWnd, 1);
		EndDialog(hWnd, NULL);
		break;
	case UWM_MSG:
		switch (lParam)
		{
		case WM_LBUTTONDOWN:
			ShowWindow(hWnd, SW_SHOW);
			break;
		}
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_MINIMIZE)
		{
			ShowWindow(hWnd, SW_HIDE);
		}
		else
			return FALSE;
			break;
	default:
		return FALSE;
	}
	return TRUE;
}



BOOL CALLBACK  _ProWin(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_CLOSE:
	{
		SendMessageA(g_hWnd, WM_COMMAND, ID_CANCEL, NULL);
		EndDialog(hWnd, NULL);
		break;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == ID_NO)
		{
			Num = 0;
			SendMessageA(g_hWnd, WM_COMMAND, ID_CANCEL, NULL);
			ShowWindow(g_hWnd, SW_SHOW);
			EndDialog(hWnd, NULL);
		}
		break;
	}
	case WM_INITDIALOG:
	{
		SetTimer(hWnd, 3, 1000, NULL);
		return FALSE;
		break;
	}
	case WM_TIMER:
	{
		if (Num >= 9)
		{
			MessageBeep(-1);
		}
		if (Num >= 10)
		{
			HANDLE hToken;
			TOKEN_PRIVILEGES tkp;
			if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))//获取令牌 
			{
				LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);//获取相应要进行操作的权限的标志符 
				tkp.PrivilegeCount = 1;//操作个数 
				tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;//权限提升赋值 
				AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);//令牌修改 
				ExitWindowsEx(EWX_SHUTDOWN, 0);
			}
			
		}
		else
		{
			Num++;
			wsprintfA(st_End, "距离关机还有%d秒", 10 - Num);
			SetWindowTextA(hWnd, st_End);
		}
		break;
	}
	default:
		return FALSE;

	}

	return TRUE;
}

int _Enable(int i)
{
	if (i == 1)
	{
		EnableWindow(GetDlgItem(g_hWnd, IDC_EDITM), TRUE);
		EnableWindow(GetDlgItem(g_hWnd, IDC_EDITY), TRUE);
		EnableWindow(GetDlgItem(g_hWnd, IDC_EDITD), TRUE);
		EnableWindow(GetDlgItem(g_hWnd, IDC_EDITH), TRUE);
		EnableWindow(GetDlgItem(g_hWnd, IDC_EDITMI), TRUE);

		EnableWindow(GetDlgItem(g_hWnd, IDC_EDIT1), FALSE);
		EnableWindow(GetDlgItem(g_hWnd, IDC_COMBO1), FALSE);
		
	}
	if (i == 0)
	{
		EnableWindow(GetDlgItem(g_hWnd, IDC_EDITM), FALSE);
		EnableWindow(GetDlgItem(g_hWnd, IDC_EDITY), FALSE);
		EnableWindow(GetDlgItem(g_hWnd, IDC_EDITD), FALSE);
		EnableWindow(GetDlgItem(g_hWnd, IDC_EDITH), FALSE);
		EnableWindow(GetDlgItem(g_hWnd, IDC_EDITMI), FALSE);
		EnableWindow(GetDlgItem(g_hWnd, IDC_EDIT1), TRUE);
		EnableWindow(GetDlgItem(g_hWnd, IDC_COMBO1), TRUE);
	}
	return 0;
}