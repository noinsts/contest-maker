#include "core/DefaultTemplates.hpp"

std::vector<LanguageTemplate> DefaultTemplates::getAllTemplates() {
	std::vector<LanguageTemplate> languages;
	languages.push_back({
		"Python",
		".py",
		getPythonDefaultTemplate(),
		"",
		true,
	});
	languages.push_back({
		"C++",
		".cpp",
		getCppDefaultTemplate(),
		"",
		true,
	});
	languages.push_back({
		"Java",
		".java",
		getJavaDefaultTemplate(),
		"",
		true,
	});
	return languages;
}

std::string DefaultTemplates::getPythonDefaultTemplate() {
	return R"(def main() -> None:
    # TODO: code
    pass

if __name__ == "__main__":
    main()

)";
}

std::string DefaultTemplates::getJavaDefaultTemplate() {
	return R"(import java.io.*;

public class {CLASS_NAME} {
    private static void solve() {
        // TODO: code
    }

    public static void main(String[] args) throws IOException {
        try (BufferedReader sc = new BufferedReader(new InputStreamReader(System.in))) {
            int tt = Integer.parseInt(sc.readLine());
            while (tt-- > 0) {
                solve();
            }
        }
    }
}
)";
}

std::string DefaultTemplates::getCppDefaultTemplate() {
	return R"(#include <bits/stdc++.h>
using namespace std;

void solve() {
    // TODO: code
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int tt; cin >> tt;

    while (tt--) {
        solve();
    }

    return 0;
}
)";
}