#include <iostream>
#include <vector>

using namespace std;

int main() {
  string a, b;
  cin >> a >> b;
  int ans = 0;
  int n = b.size();
  string s = b + "$" + a;
  vector<int> pi(s.size());
  for (int i = 1; i < s.size(); ++i) {
    int j = pi[i - 1];
    while (j != 0 && s[i] != s[j])
      j = pi[j - 1];
    pi[i] = s[i] == s[j] ? j + 1 : 0;
    if (pi[i] == n)
      ans++;
  }
  cout << ans;
}