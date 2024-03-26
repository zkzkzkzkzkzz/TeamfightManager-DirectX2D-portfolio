#pragma once

class CGamer;
class CLevel;

class LobbyLevel
{
private:
	map<wstring, CGamer*> m_gamer;

public:
	static void CreateTempLevel();
};
