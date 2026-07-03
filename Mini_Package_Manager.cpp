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
#include<unordered_set>
#include<unordered_map>

inline static void hash_combine(std::size_t& seed, std::size_t value) {
	seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

class Package {
private:
	std::vector<std::shared_ptr<Package>> Dependencies;

	std::string Name;

	std::size_t Version;
	std::size_t SizeMB;

public:

	Package(std::string name, std::size_t version, std::size_t size_mb) :
		Name(name), Version(version), SizeMB(size_mb) {
		Dependencies = {};
	}

	bool operator==(const Package& other) const {
		return this->Name == other.Name;
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
	std::unordered_map<std::shared_ptr<Package>, std::vector<std::shared_ptr<Package>>> PacGraph;

	std::unordered_set<std::shared_ptr<Package>> Packages;
	std::unordered_map<std::string, std::vector<std::string>> Deps;


public:
	PacMan() : PacGraph({}), Packages({}), Deps({}) {};

	void insert_packages_input(std::string& packages) {
		Packages.clear();
		Deps.clear();


	}

	void install_package(const std::shared_ptr<Package>& pkg) {
		std::shared_ptr<Package> pkg_lock = pkg;

		auto itr = this->PacGraph.find(pkg_lock);
		if (itr != this->PacGraph.end()) this->PacGraph.erase(itr);

		auto itr = this->PacGraph.find(pkg_lock);
		if (itr != this->PacGraph.end() && itr->first.get()->version() >= pkg.get()->version()) {
			std::cout << "Package conflict: identical or newer version already exists." << std::endl;
		}
		else if (itr != this->PacGraph.end() && itr->first.get()->version() < pkg.get()->version()) {
			std::cout << "Package " << pkg.get()->name() << " version update : from"<< itr->first.get()->version() << " to " << pkg.get()->version() << std::endl;
			this->PacGraph[pkg] = itr->second;
			this->PacGraph.erase(itr->first);

			std::cout << "version updated successfully" << std::endl;
		}
		else {
			this->PacGraph[pkg] = {};
			for (auto& deps : this->Deps[pkg.get()->name()]) {

				this->install_package()
			}
			std::cout << "Package installed successfully : " << pkg.get()->name() << std::endl;
		}
	}

	bool uninstall_package(const std::string& pkg_name, bool force = false) {
		std::cout << "uninstalling package : " << pkg_name << std::endl;

		std::cout << "package uninstalled successfully " << pkg_name << std::endl;
	}

	const std::vector<std::string> read_package(const std::string& pkg) {

	}
};