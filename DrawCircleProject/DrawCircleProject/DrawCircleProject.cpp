#include "pch.h"
#include "framework.h"
#include "DrawCircleProject.h"  // 프로젝트의 주 헤더 파일
#include "DrawCircleProjectDlg.h"   // 대화 상자 클래스 정의를 포함하는 헤더 파일

BEGIN_MESSAGE_MAP(CDrawCircleProjectApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CDrawCircleProjectApp::CDrawCircleProjectApp() {}   // 애플리케이션 클래스 생성자: 초기화 작업을 수행할 수 있는 위치

CDrawCircleProjectApp theApp;

BOOL CDrawCircleProjectApp::InitInstance() {    // 프로그램이 시작될 때 호출되며, 애플리케이션의 기본 초기화를 수행
    CWinApp::InitInstance();    // 부모 클래스(CWinApp)의 초기화 작업을 호출

    CDrawCircleProjectDlg dlg;
    m_pMainWnd = &dlg;  // m_pMainWnd: 프로그램의 메인 창을 지정하는 포인터
    dlg.DoModal();  // 대화 상자를 모달(Modal)로 표시

    return FALSE;   // 대화 상자가 닫힌 후 프로그램 종료
}
