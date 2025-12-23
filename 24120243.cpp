#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class CPlayer;
class CPlayerState;
class CTeam;

class CPlayerState {
public:
	virtual void applyState(CPlayer*) = 0;
};

class StrongState : public CPlayerState {
public:
	void applyState(CPlayer*) override;
};

class NormalState :public CPlayerState {
public:
	void applyState(CPlayer*) override;
};

class TiredState : public CPlayerState {
public:
	void applyState(CPlayer*) override;
};

class CPlayer {
private:
	string m_ID;
	string m_Name;
	int m_Health;
	int m_Spike;
	int m_Movement;
	int m_Set;
	vector<string> m_Position;
public:
	CPlayerState* currentState;
	CPlayer();
	void Input(string);
	void Output();
	bool operator==(const CPlayer&);
	vector<string> getPosition();
	void setHealth(int);
	void setSpike(int);
	void setMovement(int);
	void setSet(int);
	void updateState();
	int sumPower();
};

CPlayer::CPlayer()
{
	m_Health = m_Spike = m_Movement = m_Set = 0;
	currentState = nullptr;
}

void CPlayer::Input(string line)
{
	stringstream ss(line);

	ss >> m_ID;

	string temp;
	while (ss >> temp && !isdigit(temp[0])) {
		if (!m_Name.empty()) {
			m_Name += " ";
		}
		m_Name += temp;
	}

	m_Health = stoi(temp);

	ss >> m_Spike >> m_Movement >> m_Set;

	string pos;
	while (ss >> pos) {
		m_Position.push_back(pos);
	}
	if (!currentState) {
		if (m_Health > 90) currentState = new StrongState();
		else if (m_Health <= 50) currentState = new TiredState();
		else currentState = new NormalState();
	}
}

void CPlayer::Output()
{
	cout << m_ID << " " << m_Name << " " << m_Health << " " << m_Spike << " " << m_Movement << " " << m_Set << " ";
	for (auto& i : m_Position) {
		cout << i << " ";
	}
}

bool CPlayer::operator==(const CPlayer& p)
{
	if (m_ID == p.m_ID && m_Name == p.m_Name && m_Health == p.m_Health &&
		m_Movement == p.m_Movement && m_Spike == p.m_Spike && m_Set == p.m_Set &&
		m_Position == p.m_Position && currentState && p.currentState) return true;
	return false;
}

vector <string> CPlayer::getPosition()
{
	return m_Position;
}

void CPlayer::setHealth(int x)
{
	m_Health += x;
}

void CPlayer::setSpike(int x)
{
	m_Spike += x;
}

void CPlayer::setMovement(int x)
{
	m_Movement += x;
}

void CPlayer::setSet(int x)
{
	m_Set += x;
}

void CPlayer::updateState()
{
	currentState->applyState(this);
}

int CPlayer::sumPower()
{
	int s = m_Health + m_Spike + m_Movement + m_Set;
	return s;
}

void StrongState::applyState(CPlayer* p)
{
	p->setHealth(10);
	p->setSpike(10);
	p->setMovement(10);
	p->setSet(10);
}

void NormalState::applyState(CPlayer* p)
{

}

void TiredState::applyState(CPlayer* p)
{
	p->setHealth(-10);
	p->setSpike(-10);
	p->setMovement(-10);
	p->setSet(-10);
}

class CTeam {
private:
	vector <CPlayer> Team;
public:
	void Input();
	void Output();
	void RemovePlayerByPosition();
	void UpdatePlayerStatsBasedOnHealth();
	void FindTeam(int, int, vector<CPlayer>&, vector<vector<CPlayer>>&);
	void ArrangeThreeTeams();
	void FindStrongestTeam();
	void SubstitutePlayer();
};

void CTeam::Input()
{
	string line;
	while (getline(cin, line)) {
		if (line.empty()) continue;
		CPlayer p;
		p.Input(line);
		Team.push_back(p);
	}
}

void CTeam::Output()
{
	for (auto& i : Team) {
		i.Output();
		cout << endl;
	}
}

void CTeam::RemovePlayerByPosition()
{
	cin.clear();
	cout << "Nhap vao vi tri: ";
	string posWantToDel;
	cin >> posWantToDel;
	for (auto& c : posWantToDel) {
		c = toupper(c);
	}
	for (auto i = Team.begin(); i != Team.end();) {
		vector<string> pos = i->getPosition();
		bool flag = false;
		for (auto& j : pos) {
			if (j == posWantToDel) {
				flag = true;
				i->Output();
				cout << endl;
				i = Team.erase(i);
				break;
			}
		}
		if (!flag) {
			++i;
		}
	}
}

bool isATeam(vector<CPlayer> t) 
{
	bool OH = false, S = false, OPP = false, L = false, MB = false;
	if (t.size() != 6) return false;
	for (auto&i : t) {
		vector<string> pos = i.getPosition();
		for (auto& p : pos) {
			if (p == "OH") OH = true;
			if (p == "S") S = true;
			if (p == "OPP") OPP = true;
			if (p == "L") L = true;
			if (p == "MB") MB = true;
		}
	}
	return OH && S && OPP && L && MB;
}

void CTeam::FindTeam(int i, int size, vector<CPlayer> &cur, vector<vector<CPlayer>> &ans)
{
	if (cur.size() == 6) {
		if (isATeam(cur)) ans.push_back(cur);
		return;
	}
	if (i == size) return;
	cur.push_back(Team[i]);
	FindTeam(i + 1, size, cur, ans);
	cur.pop_back();

	FindTeam(i + 1, size, cur, ans);
}

void CTeam::ArrangeThreeTeams() 
{
	vector<vector<CPlayer>> ans;
	vector<CPlayer> cur;
	FindTeam(0, Team.size(), cur, ans);

	cout << "3 doi hinh co the lap thanh 1 doi la: ";
	int cnt = 0;
	for (auto& t : ans) {
		if (cnt >= 3) break;
		cout << "Doi hinh " << cnt + 1 << ":\n";
		for (auto& p : t) {
			cout << "- ";
			p.Output();
			cout << "\n";
		}
		cnt++;
	}
	if (cnt == 0) cout << "Khong xep duoc doi hinh nao du 5 vi tri!\n";
}

void CTeam::UpdatePlayerStatsBasedOnHealth()
{
	for (auto& p : Team) {
		p.updateState();
	}
}

void CTeam::FindStrongestTeam() 
{
	vector<vector<CPlayer>> ans;
	vector<CPlayer> cur;
	FindTeam(0, Team.size(), cur, ans);
	if (ans.empty()) {
		cout << "Khong co doi hinh nao! \n";
		return;
	}
	int max = 0, index = 0; int i = 0;
	for (auto& t : ans) {
		int sum = 0;
		for (auto& p : t) {
			sum += p.sumPower();
		}
		if (sum > max) {
			max = sum;
			index = i;
		}
		i++;
	}
	cout << "Doi hinh co suc manh lon nhat la: \n";
	for (auto &p: ans[index]) {
		cout << " -";
		p.Output();
		cout << endl;
	}
}

void SubstituteFor(vector<CPlayer> t, vector<bool> inteam, string pos) 
{
	vector<CPlayer> ans;
	int index = 0;
	for (auto& p : t) {
		if (inteam[index] == false) {
			vector<string> curPos = p.getPosition();
			for (auto& subpos : curPos) {
				if (subpos == pos) {
					ans.push_back(p);
				}
			}
		}
		index++;
	}
	if (ans.empty()) cout << "Khong co du bi cho vi tri " << pos << "! \n";
	else {
		cout << "Du bi cho vi tri " << pos << " la: \n";
		for (auto& p : ans) {
			cout << "- ";
			p.Output();
			cout << "\n";
		}
	}
}

void CTeam::SubstitutePlayer()
{
	vector<vector<CPlayer>> ans;
	vector<CPlayer> cur;
	FindTeam(0, Team.size(), cur, ans);
	if (ans.empty()) {
		cout << "Khong co doi hinh nao! \n";
		return;
	}
	int max = 0, index = 0; int i = 0;
	for (auto& t : ans) {
		int sum = 0;
		for (auto& p : t) {
			sum += p.sumPower();
		}
		if (sum > max) {
			max = sum;
			index = i;
		}
		i++;
	}
	vector<bool> inTeam;
	for (auto& p : Team) {
		for (auto& pp : ans[index]) {
			if (pp == p) {
				inTeam.push_back(true);
			}
			else inTeam.push_back(false);
		}
	}
	//du bi cho OH
	SubstituteFor(Team, inTeam, "OH");
	//du bi cho S
	SubstituteFor(Team, inTeam, "S");
	//du bi cho OPP
	SubstituteFor(Team, inTeam, "OPP");
	//du bi cho L
	SubstituteFor(Team, inTeam, "L");
	//du bi cho MB
	SubstituteFor(Team, inTeam, "MB");
}
int main()
{
	freopen("INPUT.txt", "r", stdin);
	CTeam t;
	t.Input();
	freopen("CON", "r", stdin);
	//t.RemovePlayerByPosition();
	//t.UpdatePlayerStatsBasedOnHealth();
	//freopen("OUTPUT.txt", "w", stdout);
	//t.Output();
	//t.ArrangeThreeTeams();
	//t.FindStrongestTeam();
	//t.SubstitutePlayer();
	return 0;
}
