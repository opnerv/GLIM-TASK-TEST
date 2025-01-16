#include "pch.h"
#include "framework.h"
#include "DrawCircleProject.h"
#include "DrawCircleProjectDlg.h"
#include "afxdialogex.h"
#include <cmath>
using namespace std;

CDrawCircleProjectDlg::CDrawCircleProjectDlg(CWnd* pParent) // 대화 상자의 생성자: 초기화 작업
    : CDialogEx(IDD_DRAWCIRCLEPROJECT_DIALOG, pParent),
    m_circleRadius(20), // 클릭 지점 원의 기본 반지름
    m_circleThickness(2),   // 외접원의 기본 두께
    m_isDragging(false),    // 드래그 상태 플래그 초기화
    m_draggingIndex(-1),    // 드래그 중인 클릭 지점 인덱스 초기화
    m_randomMoving(false) { // 랜덤 이동 플래그 초기화
}

void CDrawCircleProjectDlg::DoDataExchange(CDataExchange* pDX) {    // 대화 상자 데이터 교환 함수: 컨트롤과 멤버 변수 간의 데이터 교환
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDrawCircleProjectDlg, CDialogEx)
    ON_WM_PAINT()   // 화면을 다시 그릴 때 호출됨
    ON_WM_LBUTTONDOWN() // 마우스 왼쪽 버튼 클릭 시 호출
    ON_WM_MOUSEMOVE()   // 마우스 이동 시 호출됨
    ON_WM_LBUTTONUP()   // 마우스 왼쪽 버튼을 놓을 때 호출됨
    ON_BN_CLICKED(IDC_BUTTON_RESET, &CDrawCircleProjectDlg::OnBnClickedButtonReset) // OnBnClickedButtonReset() 함수에서 처리
    ON_BN_CLICKED(IDC_BUTTON_RANDOM_MOVE, &CDrawCircleProjectDlg::OnBnClickedButtonRandomMove)  // OnBnClickedButtonRandomMove() 함수에서 처리
    ON_MESSAGE(WM_USER + 1, &CDrawCircleProjectDlg::OnUpdateCoordinate) // OnUpdateCoordinate(WPARAM wParam, LPARAM lParam) 함수에서 처리

END_MESSAGE_MAP()

BOOL CDrawCircleProjectDlg::OnInitDialog() {
    CDialogEx::OnInitDialog();  // 기본 대화 상자 초기화 수행

    SetWindowText(_T("Draw Circle Project"));   // 대화 상자의 제목을 "Draw Circle Project"로 설정

    m_coordinatesList.SubclassDlgItem(IDC_LIST_COORDINATES, this);  // 리스트박스를 쉽게 제어 가능

    SetDlgItemInt(IDC_EDIT_RADIUS, m_circleRadius); // 반지름 기본값 설정
    SetDlgItemInt(IDC_EDIT_THICKNESS, m_circleThickness);   // 가장자리 두께 기본값 설정

    return TRUE;    // TRUE를 반환하면 Windows가 기본 포커스를 설정
}

void CDrawCircleProjectDlg::OnLButtonDown(UINT nFlags, CPoint point) {
    // 반지름 입력 값 가져오기
    BOOL isValidRadius = FALSE;
    int radius = GetDlgItemInt(IDC_EDIT_RADIUS, &isValidRadius);

    if (isValidRadius && radius > 0) {
        m_circleRadius = radius;
    }

    // 두께 입력 값 가져오기
    BOOL isValidThickness = FALSE;
    int thickness = GetDlgItemInt(IDC_EDIT_THICKNESS, &isValidThickness);

    if (isValidThickness && thickness > 0) {
        m_circleThickness = thickness;
    }

    if (m_clickPoints.size() < 3) { // 클릭 지점이 3개 미만인 경우만 추가
        m_clickPoints.push_back({ point.x, point.y });  // 클릭한 위치(point.x, point.y)를 m_clickPoints에 추가

        // 클릭 좌표를 문자열로 변환하여 리스트박스에 추가
        CString strCoordinate;
        strCoordinate.Format(_T("(%d, %d)"), point.x, point.y);
        m_coordinatesList.AddString(strCoordinate);

        Invalidate();   // 새로운 클릭 지점을 화면에 표시
    }

    else {  // 클릭 지점이 이미 3개인 경우
        for (size_t i = 0; i < m_clickPoints.size(); ++i) {
            if (abs(point.x - m_clickPoints[i].x) <= m_circleRadius &&
                abs(point.y - m_clickPoints[i].y) <= m_circleRadius) {  // 거리 조건으로 원 내부인지 판단
                m_isDragging = true;    // 클릭 지점이 원 내부에 있으면 m_isDragging을 true로 설정
                m_draggingIndex = static_cast<int>(i);  // 드래그 중인 클릭 지점의 인덱스를 m_draggingIndex에 저장
                break;
            }
        }
    }
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CDrawCircleProjectDlg::OnBnClickedButtonReset() {
    m_clickPoints.clear();  // 클릭 지점 리스트를 초기화
    m_coordinatesList.ResetContent(); // 리스트박스 초기화
    Invalidate();   // 대화 상자를 다시 그리도록 요청
}

void CDrawCircleProjectDlg::OnMouseMove(UINT nFlags, CPoint point) {
    if (m_isDragging && m_draggingIndex >= 0) { // 현재 드래그 상태(m_isDragging)가 활성화되어 있는지 확인
        m_clickPoints[m_draggingIndex] = { point.x, point.y };  // 드래그 위치(point)를 클릭 지점 벡터(m_clickPoints)의 드래그 중인 지점에 업데이트

        CString strCoordinate;
        strCoordinate.Format(_T("(%d, %d)"), point.x, point.y);
        m_coordinatesList.DeleteString(m_draggingIndex); // 기존 항목 삭제
        m_coordinatesList.InsertString(m_draggingIndex, strCoordinate); // 새로운 좌표 삽입

        Invalidate();   // 화면을 갱신하여 원의 새로운 위치를 반영
    }
    CDialogEx::OnMouseMove(nFlags, point);  // 부모 클래스의 OnMouseMove를 호출하여 추가적인 마우스 이동 처리가 가능하도록 한다
}

void CDrawCircleProjectDlg::OnLButtonUp(UINT nFlags, CPoint point) {
    if (m_isDragging && m_draggingIndex >= 0) {
        m_clickPoints[m_draggingIndex] = { point.x, point.y };  // 최종 좌표 업데이트

        // 리스트박스 항목 업데이트
        CString strCoordinate;
        strCoordinate.Format(_T("(%d, %d)"), point.x, point.y);
        m_coordinatesList.DeleteString(m_draggingIndex); // 기존 항목 삭제
        m_coordinatesList.InsertString(m_draggingIndex, strCoordinate); // 새로운 좌표 삽입

        m_isDragging = false;
        m_draggingIndex = -1;
    }
    CDialogEx::OnLButtonUp(nFlags, point);
}

void CDrawCircleProjectDlg::DrawCircle(HDC hdc, int x, int y, int radius, int thickness, bool fill) {
    HPEN hPen = CreatePen(PS_SOLID, thickness, RGB(0, 0, 255)); // 파란색 펜 생성
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);  // 새 펜을 선택하고 기존 펜 저장

    HBRUSH hBrush = fill ? CreateSolidBrush(RGB(255, 0, 0)) : (HBRUSH)GetStockObject(NULL_BRUSH);   // 채울 경우 빨간색 브러시
    HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);  // 새 브러시 선택하고 기존 브러시 저장

    Ellipse(hdc, x - radius, y - radius, x + radius, y + radius);   // 중심 좌표와 반지름에 따라 원 그리기

    SelectObject(hdc, hOldPen); // 이전 펜 복원
    SelectObject(hdc, hOldBrush);   // 이전 브러시 복원

    DeleteObject(hPen); // 생성한 GDI 객체 삭제
    if (fill) {
        DeleteObject(hBrush);
    }
}

bool CDrawCircleProjectDlg::CalculateCircumcircle(const ClickPoint& p1, const ClickPoint& p2, const ClickPoint& p3, ClickPoint& center, double& radius) {   // 외접원 계산 함수
    // 세 점의 외적 및 기하학적 계산
    double dA = p1.x * (p2.y - p3.y) - p1.y * (p2.x - p3.x) + (p2.x * p3.y - p3.x * p2.y);

    if (abs(dA) < 1e-6) { // 세 점이 일직선인 경우 외접원 없음
        return false;
    }

    // 세 점의 제곱합 계산. 외접원의 중심 좌표 계산에 사용
    double x1Sq = p1.x * p1.x + p1.y * p1.y;
    double x2Sq = p2.x * p2.x + p2.y * p2.y;
    double x3Sq = p3.x * p3.x + p3.y * p3.y;

    // 외접원의 중심 계산
    double xC = (x1Sq * (p2.y - p3.y) + x2Sq * (p3.y - p1.y) + x3Sq * (p1.y - p2.y)) / (2 * dA);    
    double yC = (x1Sq * (p3.x - p2.x) + x2Sq * (p1.x - p3.x) + x3Sq * (p2.x - p1.x)) / (2 * dA);

    // 계산된 중심 좌표를 center에 저장
    center.x = static_cast<int>(xC);
    center.y = static_cast<int>(yC);

    radius = sqrt((p1.x - xC) * (p1.x - xC) + (p1.y - yC) * (p1.y - yC));   // 외접원의 반지름 계산

    return true;
}

void CDrawCircleProjectDlg::CalculateAndDrawCircumcircle(HDC hdc) { // 외접원 계산 및 그리기 함수
    if (m_clickPoints.size() < 3) { // 클릭 지점이 3개 미만일 경우 외접원을 계산할 수 없으므로 리턴
        return;
    }

    // 외접원의 중심 좌표와 반지름을 저장할 변수
    ClickPoint center;
    double radius;

    if (CalculateCircumcircle(m_clickPoints[0], m_clickPoints[1], m_clickPoints[2], center, radius)) {  // 외접원을 계산 (성공 시 true 반환)
        // 내부 채우지 않고 가장자리 두께만 적용
        DrawCircle(hdc, center.x, center.y, static_cast<int>(radius), m_circleThickness, false);
    }
}

// OnPaint 수정
void CDrawCircleProjectDlg::OnPaint() { // 화면에 클릭 지점 원과 외접원을 그리는 작업을 수행
    CPaintDC dc(this);
    HDC hdc = dc.GetSafeHdc();  // HDC 핸들을 가져옴

    for (const auto& point : m_clickPoints) {   // 클릭 지점 원 그리기
        DrawCircle(hdc, point.x, point.y, m_circleRadius, 1, true);
    }

    if (m_clickPoints.size() == 3) {    // 세 번째 클릭 이후 외접원 그리기
        CalculateAndDrawCircumcircle(hdc);
    }
}

void CDrawCircleProjectDlg::OnBnClickedButtonRandomMove() { // "Random Move" 버튼이 클릭되었을 때 호출되는 이벤트 핸들러
    StartRandomMove();  // 랜덤 이동 작업을 수행하는 StartRandomMove 함수를 호출
}

void CDrawCircleProjectDlg::StartRandomMove() {
    m_randomMoving = true;  // 랜덤 이동 동작이 실행 중임을 나타내는 플래그

    m_randomMoveThread = std::thread([this]() { // 별도의 스레드를 생성하여 랜덤 이동 작업을 비동기로 수행. UI가 멈추지 않고 계속 작동하도록 보장
        CRect clientRect;
        GetClientRect(&clientRect); // 대화 상자의 클라이언트 영역 가져오기

        int padding = m_circleRadius; // 원이 창 밖으로 나가지 않도록 여유 공간
        int leftLimit = clientRect.left + padding;
        int topLimit = clientRect.top + padding;
        int rightLimit = clientRect.right - padding;
        int bottomLimit = clientRect.bottom - padding;

        for (int i = 0; i < 10 && m_randomMoving; ++i) {    // 10번 반복하며 클릭 지점을 랜덤으로 이동
            for (size_t j = 0; j < m_clickPoints.size(); ++j) {
                m_clickPoints[j].x = rand() % (rightLimit - leftLimit) + leftLimit; // 랜덤 좌표를 클라이언트 영역 안에 생성
                m_clickPoints[j].y = rand() % (bottomLimit - topLimit) + topLimit;

                CString strCoordinate;  
                strCoordinate.Format(_T("(%d, %d)"), m_clickPoints[j].x, m_clickPoints[j].y);   // 리스트박스 항목 업데이트

                PostMessage(WM_USER + 1, j, reinterpret_cast<LPARAM>(new CString(strCoordinate)));  // PostMessage를 사용해 메인 스레드에서 안전하게 UI를 업데이트
            }
            Invalidate(); // 화면 갱신
            this_thread::sleep_for(std::chrono::milliseconds(500)); // 0.5초 대기
        }
        m_randomMoving = false; // 랜덤 이동이 완료되면 플래그를 false로 설정하여 종료를 나타냄
        });

    m_randomMoveThread.detach();    // 스레드를 분리하여 메인 스레드와 독립적으로 실행되도록 설정
}

LRESULT CDrawCircleProjectDlg::OnUpdateCoordinate(WPARAM wParam, LPARAM lParam) {   // 랜덤 이동 중에 리스트박스의 좌표를 업데이트하는 역할
    int index = static_cast<int>(wParam);   // 업데이트할 클릭 지점의 인덱스
    CString* pCoordinate = reinterpret_cast<CString*>(lParam);  // 좌표 문자열 (동적으로 생성된 객체)

    if (index >= 0 && index < m_clickPoints.size()) {   // 인덱스가 유효한 경우 리스트박스를 업데이트
        m_coordinatesList.DeleteString(index); // 기존 항목 삭제
        m_coordinatesList.InsertString(index, *pCoordinate); // 새로운 좌표 삽입
    }

    delete pCoordinate; // 동적 할당 해제
    return 0;
}

