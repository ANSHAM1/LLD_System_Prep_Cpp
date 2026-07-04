//Problem: Mini Package Manager
//
//Package Format
//
//A package is represented as
//
//PACKAGE |
//PackageName |
//Version |
//SizeMB |
//Dependency1, Dependency2, ... |
//Installed(True / False)
//
//Example
//
//PACKAGE | numpy | 2.0 | 45 | python |
//PACKAGE | opencv | 4.10 | 180 | numpy |
//PACKAGE | torch | 2.7 | 820 | numpy |
//PACKAGE | fastapi | 1.2 | 10 | python |
//PACKAGE | myapp | 1.0 | 12 | torch, opencv, fastapi |

#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<unordered_map>
#include<algorithm>

static std::string strip(std::string s) {
	auto first = s.find_first_not_of(" \t\n\r\f\v");

	if (first == std::string::npos)
		return "";

	auto last = s.find_last_not_of(" \t\n\r\f\v");

	return s.substr(first, last - first + 1);
}

static void split(std::vector<std::string>& results, std::string s, char d) {
	std::istringstream iss(strip(s));
	std::string token;

	while (std::getline(iss, token, d)) {
		results.push_back(strip(token));
	}
}

static void split(std::vector<std::string>& results, std::string s) {
	std::istringstream iss(strip(s));
	std::string token;

	while (std::getline(iss, token)) {
		results.push_back(strip(token));
	}
}

static std::string lower(std::string std) {
	std::transform(std.begin(), std.end(), std.begin(), [](unsigned char c) { return std::tolower(c); });
	return std;
}

struct Package {
	std::string Name;
	float Version;
	float SizeMB;

	Package() {}

	Package(std::string name) : Name(name) {}

	Package(std::string name, float version, float size) :
		Name(name), Version(version), SizeMB(size) {};

	bool operator==(const Package& other) const {
		return Name == other.Name;
	}
};

namespace std {
	template<>
	struct hash<Package> {
		size_t operator()(const Package& p) const {
			return hash<string>()(p.Name);
		}
	};
}

class PacMan {
private:
	std::unordered_map<Package, std::vector<Package>> PacGraph;

	bool checkDependency(std::string pkg) {

		// ------------------
		return 0;
	}

	bool install_package(const std::unordered_map<std::string, std::vector<std::string>>& tempRel, const std::string& pkg) {
		std::string pkgName = lower(pkg);

		if (this->PacGraph.find(pkgName) != this->PacGraph.end()) return 1;

		auto it = tempRel.find(pkgName);
		if (it == tempRel.end() || it->second.size() < 2) return 0;

		Package new_pkg(pkgName, std::stof(it->second[0]), std::stof(it->second[1]));

		std::vector<Package> deps;
		for (size_t i = 2; i < it->second.size(); ++i) {
			bool isInstalled = install_package(tempRel, it->second[i]);
			if (!isInstalled) return 0;
			
			deps.push_back(this->PacGraph.find(it->second[i])->first);
		}

		this->PacGraph[new_pkg] = deps;
		return 1;
	}

public:
	void install_packages(std::string str) {
		std::unordered_map<std::string, std::vector<std::string>> tempRel;

		std::vector<std::string> lines;
		split(lines, str);

		for (auto& line : lines) {
			if (line.size() <= 0) continue;

			std::vector<std::string> tokens;
			split(tokens, line, '|');

			if (tokens.size() != 4 || tokens[1].size() == 0 || tokens[2].size() == 0 || tokens[3].size() == 0) continue;

			std::vector<std::string> filtered = {tokens[2], tokens[3]};

			std::vector<std::string> dep;
			split(dep, tokens[4], ',');

			if (dep.size() != 0) {
				for (auto& d : dep) {
					if (d.size() == 0) continue;
					filtered.push_back(lower(d));
				}
			}
			tempRel[lower(tokens[1])] = filtered;
		}

		for (auto& pkg : tempRel) {
			if (this->PacGraph.find(Package(pkg.first)) != this->PacGraph.end()) continue;
			bool isInstalled = install_package(tempRel, pkg.first);
			if(isInstalled) std::cout << "Package : " << pkg.first << "Installed Successfully" << std::endl;
			else std::cout << "Package Installation Error: " << pkg.first << " -> Missing Dependencies" << std::endl;
		}
	}

	bool uninstall(std::string pkg, bool force = false) {
		std::string pkgName = lower(pkg);

		auto it = this->PacGraph.find(pkgName);
		if (it == this->PacGraph.end()) {
			std::cout << "Package : " << pkgName << " not found" << std::endl;
			return 0;
		}

		bool canUninstall = checkDependency(pkgName);
		if (!canUninstall && !force) {
			std::cout << "Package Unistallation Error: " << pkgName << " -> Other Packages are Dependent on it" << std::endl;
			return 0;
		}

		bool isUninstalled = true;
		for (auto& dep : it->second) {
			isUninstalled = uninstall(dep.Name, force);
		}

		if (isUninstalled) std::cout << "Package : " << pkgName << "Uninstalled Successfully" << std::endl;
		else std::cout << "Package Unistallation Error: " << pkgName << " -> Other Packages are Dependent on it" << std::endl;
	}
};

int main() {
	std::string input = R"(
						PACKAGE | numpy | 2.0 | 45 | python |
						PACKAGE | opencv | 4.10 | 180 | numpy |
						PACKAGE | torch | 2.7 | 820 | numpy |
						PACKAGE | fastapi | 1.2 | 10 | python |
						PACKAGE | myapp | 1.0 | 12 | torch, opencv, fastapi |
							)";

	PacMan PM;
	PM.install_packages(input);
}