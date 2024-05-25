set_project("mfight")
set_basename("mfight")
set_version("0.1.0")

set_languages("c++20")
set_targetdir("build")
set_policy("build.warning", true)

add_requires("entt 3.x", "libsdl 2.x", "libsdl_ttf 2.x", "fmt 10.x", "spdlog 1.x")

target("main")
	set_default(true)
	set_kind("binary")
	add_files("src/*.cpp", "src/**/*.cpp")
	add_includedirs("include/")

	add_packages("entt", "libsdl", "libsdl_ttf", "fmt", "spdlog")
	set_warnings("allextra")
	if is_mode("release") then
		set_strip("all")
		set_optimize("faster")
		add_defines("NDEBUG")
	end

	if is_mode("debug") then
		set_optimize("none")
		set_symbols("debug")
		add_defines("DEBUG")
	end
