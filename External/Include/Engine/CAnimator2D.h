#pragma once
#include "CComponent.h"

#include "CTexture.h"

class CAnim;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnim*>    m_mapAnim;
    CAnim*                  m_CurAnim;
    bool                    m_bRepeat;

public:
    virtual void finaltick() override;
       
    void UpdateData();
    static void Clear();

public:
    void Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _LeftTop, Vec2 _vSliceSize,
                 Vec2 _OffsetSize, Vec2 _Background, int _FrmCount, float _FPS);

    void CreateFrm(const wstring& _strKey, Ptr<CTexture> _AtlasTex, vector<tAnimFrm> _vecAnimFrm);

    CAnim* FindAnim(const wstring& _strAnimName);
    void Play(const wstring& _strAnimName, bool _bRepeat = true);
    void Stop();

    void SaveAnimations(const wstring& _strRelativePath);
    void LoadAnimation(const wstring& _strRelativePath);

    map<wstring, CAnim*> GetAnimList();
    CAnim* GetCurAnim() { return m_CurAnim; }


    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CAnimator2D);

public:
    CAnimator2D();
    CAnimator2D(const CAnimator2D& _OriginAnimator);
    ~CAnimator2D();
};

