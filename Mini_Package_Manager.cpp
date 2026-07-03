//Problem: Mini Package Manager

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
#include<stdexcept>
#include<memory>
#include<string>
#include<unordered_map>
#include<unordered_set>

inline static void hash_combine(std::size_t& seed, std::size_t value) {
	seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

class Package {
private:
	std::vector<std::string> Dependencies;

	std::string Name;

	std::size_t Version;
	std::size_t SizeMB;

public:

	Package(std::string name, std::size_t version, std::size_t size_mb) :
		Name(name), Version(version), SizeMB(size_mb) {
		this->Dependencies = {};
	}

	Package(std::string name, std::size_t version, std::size_t size_mb, std::vector<std::string>& dep) :
		Name(name), Version(version), SizeMB(size_mb), Dependencies(dep) {}

	bool operator==(const Package& other) const {
		return this->Name == other.Name;
	}

	bool operator==(const std::string& pkg_name) const {
		return this->Name == pkg_name;
	}

	std::string name() const {
		return this->Name;
	}

	std::size_t version() const {
		return this->Version;
	}

	std::size_t size() const {
		return this->SizeMB;
	}

	const std::vector<std::string>& dependencies() const {
		return this->Dependencies;
	}
};

namespace std {
	template<>
	struct hash<Package> {
		std::size_t operator()(const Package& p) const noexcept {
			std::size_t seed = 0;

			hash_combine(seed, std::hash<std::string>{}(p.name()));

			return seed;
		}
	};
}

class PacMan {
private:
	std::unordered_map<std::shared_ptr<Package>, std::unordered_set<std::shared_ptr<Package>>> PacGraph;
	std::unordered_set<std::shared_ptr<Package>> Packages;

public:
	PacMan() : PacGraph({}), Packages({}) {}

	PacMan(std::string& pkgs) {
		Packages.clear();
		// ----
	}

	bool install_package(const std::shared_ptr<Package>& pkg) {
		auto itr = this->PacGraph.find(pkg);
		if (itr != this->PacGraph.end() && itr->first.get()->version() >= pkg.get()->version()) {
			std::cout << "Package conflict: identical or newer version already exists." << std::endl;
			return true;
		}
		else {
			std::cout << "Installing Package and its Dependencies" << std::endl;
			auto& idep = itr->first->dependencies();
			for (auto& dep : pkg.get()->dependencies()) {
				auto& dep_itr = this->Packages.find(dep);
				if (dep.find() != this->PacGraph.end() && itr->first.get()->version() >= PKG.get()->version()) {
					std::cout << "Package conflict: identical or newer version already exists." << std::endl;
					return true;
				}
				std::shared_ptr<Package> PKG_D = std::make_shared<Package>(new Package(dep_vec[0], dep_vec[1], dep_vec[]));
				this->install_package()
			}
			this->PacGraph[pkg] = {};
		}
	}

	const std::vector<std::string> read_package(const std::string& pkg) {

	}
};