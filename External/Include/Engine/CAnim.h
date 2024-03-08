#pragma once
#include "CEntity.h"

#include "CTexture.h"


class CAnimator2D;

class CAnim :
    public CEntity
{
private:
    CAnimator2D*        m_Animator;
    vector<tAnimFrm>    m_vecFrm;
    int                 m_CurFrmIdx;
    bool                m_bFinish;
    float               m_AccTime;

    Ptr<CTexture>       m_AtlasTex;

public:
    void finaltick();
    void UpdateData();

    bool IsFinish() { return m_bFinish; }
    void Reset()
    {
        m_bFinish = false; 
        m_CurFrmIdx = 0;
        m_AccTime = 0.f;
    }

    static void Clear();
    void Create(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, Vec2 _vLeftTop, Vec2 _vSliceSize,
                Vec2 _vOffset, Vec2 _vBackground, int _FrmCount, float _FPS);

    void CreateFrm(CAnimator2D* _Animator, Ptr<CTexture> _Atlas, vector<tAnimFrm> _vecAnimFrm);

    bool Save(const wstring& _FilePath, const wstring& _strKey);
    bool Load(const wstring& _FilePath);


    void SaveToFile(FILE* _File);
    void LoadFromFile(FILE* _File);

    CLONE(CAnim);

public:
    CAnim();
    CAnim(const CAnim& _OriginAnim);
    ~CAnim();

    friend class CAnimator2D;
};

