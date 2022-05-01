#pragma once

#include <iberbar/GameEngine/Application.h>
#include <iberbar/Gui/Widgets/Container.h>


class CMainMenu
	: public iberbar::Gui::CContainer
{
public:
	void Initial();

private:
	iberbar::Gui::CXmlParser* m_pXmlParser;
	bool m_bInit;
};


class CTestApplication
	: public iberbar::Game::CApplication
{
public:
	CTestApplication()
		: CApplication()
	{

	}

protected:
	virtual void OnCreated() override;
	virtual void OnDestroy() override;
	virtual void OnUpdate( int64 nElapsedTimeMilliSecond, float nElapsedTimeSecond ) override
	{
	}
	virtual iberbar::CResult ExportAssetsPreInitial() override;
};

