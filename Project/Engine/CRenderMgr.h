#pragma once
#include "singleton.h"

#include "CTexture.h"

class CCamera;
class CGameObject;
class CStructuredBuffer;
class CLight2D;
class CText;

class CRenderMgr :
    public CSingleton<CRenderMgr>
{
    SINGLE(CRenderMgr);

private:
    vector<CCamera*>        m_vecCam;
    CCamera*                m_EditorCam;

    Ptr<CTexture>           m_PostProcessTex;

    CStructuredBuffer*      m_Light2DBuffer;    // 2차원 광원 구조화 버퍼
    vector<CLight2D*>       m_vecLight2D;

    list<tDebugShapeInfo>   m_DbgShapeInfo;
    CGameObject*            m_pDebugObj;
    bool                    m_DebugPosition;

    vector<Ptr<CTexture>>   m_vecNoiseTex;

    CText*                  m_Text;

    // render function pointer
    typedef void(CRenderMgr::* RENDER_FUNC)(void);
    RENDER_FUNC     m_RenderFunc;

public:
    void RegisterCamera(CCamera* _Cam, int _Idx);
    void AddDebugShapeInfo(const tDebugShapeInfo& _Info) { m_DbgShapeInfo.push_back(_Info); }

    void SetDebugPosition(bool _IsValid) { m_DebugPosition = _IsValid; }
    bool IsDebugPosition() { return m_DebugPosition; }

    void RegisterLight2D(CLight2D* _Light2D) { m_vecLight2D.push_back(_Light2D); }

    void CopyRenderTargetToPostProcessTarget();

    Ptr<CTexture> GetPostProcessTex() { return m_PostProcessTex; }

    void RegisterEditorCamera(CCamera* _Cam) { m_EditorCam = _Cam; }
    void ClearCamera() { m_vecCam.clear(); }

    void ActiveEditorMode(bool _bActive)
    {
        if (_bActive)
            m_RenderFunc = &CRenderMgr::render_editor;
        else
            m_RenderFunc = &CRenderMgr::render_play;
    }

public:
    void init();
    void tick();

private:
    void render_play();
    void render_editor();
    void render_debug();
    
    // 리소스 바인딩
    void UpdateData();
    
    // 리소스 클리어
    void Clear();
};

