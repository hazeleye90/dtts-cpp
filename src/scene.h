#pragma once
class Scene
{
    friend class SceneManager;
public:
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void free() = 0;
    virtual bool complete() = 0;

private:
    SceneManager* mSceneManager;
};

class SceneManager
{

public:
    void goTo(Scene *s)
    {
        if (mScene)
            mScene->free();
        mScene = s;
        mScene->mSceneManager = this;
    }
    Scene *getScene() { return mScene; }

private:
    Scene *mScene = nullptr;
};