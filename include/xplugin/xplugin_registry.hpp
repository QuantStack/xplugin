/***************************************************************************
* Copyright (c) 2023, Dr. Thorsten Beier                                   *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#ifndef XPLUGIN_REGISTRY_HPP
#define XPLUGIN_REGISTRY_HPP

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <filesystem>

#include <xplugin/xshared_library.hpp>


namespace xp
{

    // get so/dll/dylib 
    inline std::string get_library_extension()
    {
        #if defined(_WIN32)
            return ".dll";
        #elif defined(__APPLE__)
            return ".dylib";
        #else
            return ".so";
        #endif
    }

    template<class FACTORY>
    class xplugin_registry
    {
        public:
        using create_plugin_factory_type = FACTORY * (*)();

        xplugin_registry() = default;

        void scan_directory(const std::filesystem::path& path);
        

        std::unordered_set<std::string> plugin_names();

        std::unique_ptr<FACTORY> create_factory(const std::string & name);

        std::size_t size() const;

        void close_all();

        private:
        std::unordered_map<std::string, xshared_library> m_open_libraries;
        std::unordered_map<std::string, std::filesystem::path> m_locations;
    };

    template<class FACTORY>
    void xplugin_registry<FACTORY>::scan_directory(const std::filesystem::path& path)
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.path().extension() == get_library_extension())
            {
                std::string name = entry.path().stem().string();

                // remove lib prefix
                if (name.substr(0,3) == "lib"){
                    name = name.substr(3);
                }
                m_locations[name] = entry.path();
            }
        }
    }

    template<class FACTORY>
    std::unordered_set<std::string> xplugin_registry<FACTORY>::plugin_names()
    {
        std::unordered_set<std::string> res;
        for (const auto& [key, value] : m_locations)
        {
            res.insert(key);
        }
        return res;
    }

    template<class FACTORY>
    std::unique_ptr<FACTORY> xplugin_registry<FACTORY>::create_factory(const std::string & name)
    {
        auto find_res = m_locations.find(name);
        if (find_res == m_locations.end()){
            throw std::runtime_error("Could not find plugin factory for " + name);
        }

        xshared_library * library_ptr = nullptr;
        auto open_find_res = m_open_libraries.find(name);
        if (open_find_res != m_open_libraries.end())
        {
            library_ptr = &open_find_res->second;
        }
        else  
        {
            xunix_shared_library library(find_res->second);
            auto [iter, inserted] = m_open_libraries.emplace(name, std::move(library));
            library_ptr = &(iter->second);
        }

        auto factory = library_ptr->find_symbol<create_plugin_factory_type>("create_plugin_factory")();
        return std::unique_ptr<FACTORY>(factory);
    }

    template<class FACTORY>
    std::size_t xplugin_registry<FACTORY>::size() const
    {
        return m_locations.size();
    }

    template<class FACTORY>
    void xplugin_registry<FACTORY>::close_all()
    {
        for(auto & [key, value] : m_open_libraries){
            value.close();
        }
        m_open_libraries.clear();
    }
}

#endif // XPLUGIN_REGISTRY_HPP