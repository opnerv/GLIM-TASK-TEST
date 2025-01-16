#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <afxwin.h>
using namespace std;

struct ClickPoint {
    int x, y;
};

class CDrawCircleProjectDlg : public CDialogEx {
public:
    CDrawCircleProjectDlg(CWnd* pParent = nullptr);
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedButtonReset();
    afx_msg void OnBnClickedButtonRandomMove();

protected:
    HICON m_hIcon;
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg LRESULT OnUpdateCoordinate(WPARAM wParam, LPARAM lParam);

    DECLARE_MESSAGE_MAP()

private:
    vector<ClickPoint> m_clickPoints; // 클릭 지점 저장
    int m_circleRadius;                    // 클릭 지점 원 반지름
    int m_circleThickness;                 // 외접원 선 두께
    bool m_isDragging;                     // 드래그 중 여부
    int m_draggingIndex;                   // 드래그 중인 클릭 지점 인덱스
    CListBox m_coordinatesList;            // 클릭 지점 좌표를 표시할 리스트박스
    atomic<bool> m_randomMoving;      // 랜덤 이동 상태
    thread m_randomMoveThread;        // 랜덤 이동 쓰레드

    void DrawCircle(HDC hdc, int x, int y, int radius, int thickness, bool fill);
    bool CalculateCircumcircle(const ClickPoint& p1, const ClickPoint& p2, const ClickPoint& p3, ClickPoint& center, double& radius);
    void CalculateAndDrawCircumcircle(HDC hdc);
    void StartRandomMove();
};
