-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes(os.getenv("CommonLibSSE-NG"))

-- set project
set_project("Journeyman")
set_version("1.0.0")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra", "error")
set_defaultmode("releasedbg")

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- set policies
set_policy("package.requires_lock", true)

-- set configs
-- set configs
set_config("skyrim_vr", false)
set_config("skyrim_ae", false)
set_config("skyrim_se", true)
set_config("skse_xbyak", false)

-- targets
target("Journeyman")
    -- add dependencies to target
    add_deps("commonlibsse-ng")

    -- add commonlibsse-ng plugin
    add_rules("commonlibsse-ng.plugin", {
        name = "Journeyman",
        author = "Simon Magus and Colinswrath and Newrite (as Backporter)",
        description = "Journeyman is a lightweight, highly compatible fast travel overhaul that restricts the player from fast traveling unless they have prepared a travel pack."
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

    -- copy build files to MODS or GAME paths (remove this if not needed)
    after_build(function(target)
        local copy = function(env, ext)
            for _, env in pairs(env:split(";")) do
                if os.exists(env) then
                    local plugins = path.join(env, ext, "SKSE/Plugins")
                    os.mkdir(plugins)
                    os.trycp(target:targetfile(), plugins)
                    os.trycp(target:symbolfile(), plugins)
                end
            end
        end
        if os.getenv("XSE_TES5_MODS_PATH") then
            copy(os.getenv("XSE_TES5_MODS_PATH"), target:name())
        elseif os.getenv("XSE_TES5_GAME_PATH") then
            copy(os.getenv("XSE_TES5_GAME_PATH"), "Data")
        end
    end)
