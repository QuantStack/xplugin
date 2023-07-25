#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <filesystem>

#include <iostream>

#include <xplugin/xshared_library.hpp>


namespace xp
{

    // get so/dll/dylib 

    inline std::string get_library_extension(){
        #if defined(_WIN32)
            return ".dll";
        #elif defined(__APPLE__)
            return ".dylib";
        #else
            return ".so";
        #endif
    }

    template<class FACTORY>
    class xplugin_registry{

        public:
            using create_plugin_factory_type = FACTORY * (*)();


            xplugin_registry() = default;

            void scan_directory(const std::filesystem::path& path){
                for (const auto& entry : std::filesystem::directory_iterator(path)){
                    if (entry.path().extension() == get_library_extension()){
                        std::string name = entry.path().stem().string();

                        // remove lib prefix
                        if (name.substr(0,3) == "lib"){
                            name = name.substr(3);
                        }
                        m_factories[name] = entry.path();
                    }
                }
            }

            std::vector<std::string> plugin_names(){
                std::vector<std::string> res;
                for (const auto& [key, value] : m_factories){
                    res.push_back(key);
                }
                return res;
            }

            std::unique_ptr<FACTORY> create_factory(const std::string & name){
                auto find_res = m_factories.find(name);
                if (find_res == m_factories.end()){
                    throw std::runtime_error("Could not find plugin factory for " + name);
                }

                // check if already open
                xshared_library * library = nullptr;
                auto open_find_res = m_open_libraries.find(name);
                if (open_find_res != m_open_libraries.end()){
                    // already open
                    library = open_find_res->second;
                }
                else  
                {
                    // not yet open
                    library = xshared_library::open(find_res->second);
                    m_open_libraries[name] = library;
                }

                auto factory = library->find_symbol<create_plugin_factory_type>("create_plugin_factory")();
                return std::unique_ptr<FACTORY>(factory);
            }

        private:
            std::unordered_map<std::string, xshared_library*> m_open_libraries;
            std::unordered_map<std::string, std::filesystem::path> m_factories;
    };

}